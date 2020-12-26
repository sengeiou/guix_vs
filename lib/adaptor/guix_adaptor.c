#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

#if defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#elif defined(_WIN32)
#include <SDL.h>
#include <SDL_render.h>
#else
#error "Unknown operation system"
#endif

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "gx_system_rtos_bind.h"

#include "guix_adaptor.h"
#include "list.h"

#define MAX_EVENTS 48
#define OS_TICKS_PER_SECOND 100

#define GUIX_INPUT_STATE_RELEASED 0
#define GUIX_INPUT_STATE_PRESSED  1


struct guix_event {
    struct list_head node;
    GX_EVENT event_data;        /* the GUIX event structure */
};

struct guix_event_queue {
    struct list_head pending;
    struct list_head free;
    SDL_cond* cond;
    SDL_mutex* mutex;
};

struct guix_os {
    struct guix_event_queue queue;
    SDL_Thread* thread;
    SDL_mutex* mutex;
    SDL_TimerID timer;
    VOID(*entry)(ULONG);
    bool timer_actived;
    struct guix_event event[0];
};

struct gui_input {
    GX_POINT current;
    GX_POINT last;
    int state;
    int drag_min;
};

struct gui_adaptor {
    const struct gui_screen *screen;
    SDL_Window* win;
    SDL_Renderer* render;
    SDL_Texture* texture;
};


static struct guix_os* guix_os;
static struct gui_adaptor* gui_adaptor;

static void guix_exit_sync(void);

static int convert_pixel_format(const struct gui_screen *screen)
{
    switch (screen->pixel_format) {
    case GX_COLOR_FORMAT_565RGB:
        return SDL_PIXELFORMAT_RGB565;
    case GX_COLOR_FORMAT_565BGR:
        return SDL_PIXELFORMAT_BGR565;
    case GX_COLOR_FORMAT_24RGB:
        return SDL_PIXELFORMAT_RGB24;
    case GX_COLOR_FORMAT_24BGR:
        return SDL_PIXELFORMAT_BGR24;
    default:
        return -1;
    }
}

static int guix_load_environment(struct gui_adaptor* adaptor)
{
    const struct gui_screen *screen = adaptor->screen;

    if (SDL_Init(SDL_INIT_EVERYTHING)){
        printf("SDL init failed: %s\n", SDL_GetError());
        return -EINVAL;
    }

    adaptor->win = SDL_CreateWindow("GUIX Simulator",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, 
                                    screen->screen_width, 
                                    screen->screen_height, 
                                    SDL_WINDOW_SHOWN);
    if (!adaptor->win) {
        printf("SDL create window failed: %s\n", SDL_GetError());
        return -EINVAL;
    }

    adaptor->render = SDL_CreateRenderer(adaptor->win, -1, 
        SDL_RENDERER_ACCELERATED);
    if (!adaptor->render) {
        printf("SDL create renderer failed: %s\n", SDL_GetError());
        return -EINVAL;
    }

    adaptor->texture = SDL_CreateTexture(adaptor->render, 
                                        convert_pixel_format(screen),
                                        SDL_TEXTUREACCESS_STREAMING,
                                        screen->screen_width,
                                        screen->screen_height);
    if (!adaptor->texture) {
        printf("create texture failed: %s\n", SDL_GetError());
        return -EINVAL;
    }

    SDL_SetTextureBlendMode(adaptor->texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(adaptor->render, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(adaptor->render);
    SDL_RenderCopy(adaptor->render, adaptor->texture, NULL, NULL);
    SDL_RenderPresent(adaptor->render);
    return 0;
}

static void guix_draw(struct GX_CANVAS_STRUCT* canvas, GX_RECTANGLE* area)
{
    struct gui_adaptor *adaptor = canvas->gx_canvas_display->gx_display_driver_data;
    const struct gui_screen* screen = adaptor->screen;
    uint8_t *buffer = (uint8_t *)canvas->gx_canvas_memory;
    GX_RECTANGLE limit;
    GX_RECTANGLE client;
    
    gx_utility_rectangle_define(&limit, 0, 0,
        canvas->gx_canvas_x_resolution - 1,
        canvas->gx_canvas_y_resolution - 1);

    if (gx_utility_rectangle_overlap_detect(&limit, area, &client)) {
        SDL_Rect rect;
        ULONG offset;

        rect.x = client.gx_rectangle_left;
        rect.y = client.gx_rectangle_top;
        rect.w = client.gx_rectangle_right - client.gx_rectangle_left + 1;
        rect.h = client.gx_rectangle_bottom - client.gx_rectangle_top + 1;
        offset = client.gx_rectangle_top * canvas->gx_canvas_x_resolution
            + client.gx_rectangle_left;
        offset *= screen->pixel_width;
        //SDL_LockTexture(adaptor->texture, &rect, NULL, NULL);
        SDL_UpdateTexture(adaptor->texture, &rect, buffer+offset,
            screen->screen_width * screen->pixel_width);
        SDL_RenderClear(adaptor->render);
        SDL_RenderCopy(adaptor->render, adaptor->texture, NULL, NULL);
        SDL_RenderPresent(adaptor->render);
        //SDL_UnlockTexture(adaptor->texture);
    }
}

static UINT guix_driver_setup(GX_DISPLAY* display)
{
    struct gui_adaptor* adaptor = gui_adaptor;

    switch (adaptor->screen->pixel_format) {
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
        _gx_display_driver_565rgb_setup(display, adaptor, guix_draw);
        break;
    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
        _gx_display_driver_24xrgb_setup(display, adaptor, guix_draw);
        break;
    default:
        return GX_FALSE;
    }

    return GX_TRUE;
}

static void guix_press_event_post(struct gui_input* input,
    GX_EVENT* event)
{
    event->gx_event_type = GX_EVENT_PEN_DOWN;
    event->gx_event_payload.gx_event_pointdata = input->current;
    _gx_system_event_send(event);
    input->last = input->current;
    input->state = GUIX_INPUT_STATE_PRESSED;
}

static void guix_release_event_post(struct gui_input* input,
    GX_EVENT* event)
{
    event->gx_event_type = GX_EVENT_PEN_UP;
    event->gx_event_payload.gx_event_pointdata = input->current;
    _gx_system_event_send(event);
    input->last = input->current;
    input->state = GUIX_INPUT_STATE_RELEASED;
}

static void guix_drag_event_post(struct gui_input* input,
    GX_EVENT* event)
{
    int x, y;
    
    x = abs(input->current.gx_point_x - input->last.gx_point_x);
    y = abs(input->current.gx_point_y - input->last.gx_point_y);
    if (x > input->drag_min || y > input->drag_min) {
        event->gx_event_type = GX_EVENT_PEN_DRAG;
        event->gx_event_payload.gx_event_pointdata = input->current;
        input->last = input->current;
        _gx_system_event_fold(event);
    }
}

static void guix_input_event_post(struct gui_input* input,
    GX_EVENT* event, int state)
{
    if (state == GUIX_INPUT_STATE_PRESSED) {
        if (input->state == GUIX_INPUT_STATE_PRESSED)
            guix_drag_event_post(input, event);
        else
            guix_press_event_post(input, event);
    } else {
        guix_release_event_post(input, event);
    }
}

int guix_simulate_start(const struct gui_screen *gui)
{
    struct gui_adaptor *adaptor;
    struct gui_input input;
    GX_POINT *point;
    GX_EVENT event;
    int ret;

    if (!gui)
        return -EINVAL;

    if (!gui->do_init)
        return -EINVAL;

    if (convert_pixel_format(gui) < 0) {
        printf("Error***: Invalid pixel format\n");
        return -EINVAL;
    }

    adaptor = calloc(1, sizeof(*adaptor));
    if (!adaptor) {
        printf("Error***: No more memory!");
        return -ENOMEM;
    }

    adaptor->screen = gui;
    ret = guix_load_environment(adaptor);
    if (ret)
        goto _destory;

    gui_adaptor = adaptor;
    ret = gui->do_init(guix_driver_setup);
    if (ret)
        goto _destory;

    event.gx_event_sender = 0;
    event.gx_event_target = 0;
    event.gx_event_display_handle = (ULONG)adaptor;
    point = &event.gx_event_payload.gx_event_pointdata;
    memset(&input, 0, sizeof(input));

    while (true) {
        SDL_Event e;
        if (!SDL_PollEvent(&e))
            continue;

        switch (e.type) {
        case SDL_QUIT:
            ret = 0;
            goto _destory;
        case SDL_MOUSEBUTTONDOWN:
            input.current.gx_point_x = e.motion.x;
            input.current.gx_point_y = e.motion.y;
            guix_input_event_post(&input, &event, GUIX_INPUT_STATE_PRESSED);
            break;
        case SDL_MOUSEBUTTONUP:
            input.current.gx_point_x = e.motion.x;
            input.current.gx_point_y = e.motion.y;
            guix_input_event_post(&input, &event, GUIX_INPUT_STATE_RELEASED);
            break;
        case SDL_MOUSEMOTION:
            if (input.state == GUIX_INPUT_STATE_PRESSED) {
                input.current.gx_point_x = e.motion.x;
                input.current.gx_point_y = e.motion.y;
                guix_input_event_post(&input, &event, input.state);
            }
            break;
        default:
            break;
        }
    }

_destory:
    guix_exit_sync();
    SDL_DestroyWindow(adaptor->win);
    SDL_DestroyRenderer(adaptor->render);
    SDL_DestroyTexture(adaptor->texture);
    free(adaptor);
    SDL_Quit();
    return ret;
}

static int guix_os_thread(void *parg)
{
    _gx_system_thread_entry((ULONG)parg);
    return 0;
}

static Uint32 guix_os_timer(Uint32 interval, void *param)
{
    (void)param;
    if (!guix_os)
        return interval;
    _gx_system_timer_expiration(0);
    return interval;
}

static void guix_exit_sync(void)
{
    struct guix_os *os = guix_os;
    GX_EVENT exit = {.gx_event_type = GX_EVENT_TERMINATE, }; 
    int status;

    gx_generic_event_post(&exit);
    SDL_WaitThread(os->thread, &status);
    printf("Thread returned value: %d\n", status);
    if (os->timer) {
        if (!SDL_RemoveTimer(os->timer))
            printf("Error***: Remove timer failed!\n");
        os->timer = 0;
    }

    if (os->queue.mutex) {
        SDL_DestroyMutex(os->queue.mutex);
        os->queue.mutex = NULL;
    }

    if (os->queue.cond) {
        SDL_DestroyCond(os->queue.cond);
        os->queue.cond = NULL;
    }

    if (os->mutex) {
        SDL_DestroyMutex(os->mutex);
        os->mutex = NULL;
    }

    guix_os = NULL;
    free(os);
}

VOID gx_generic_rtos_initialize(VOID)
{
    struct guix_os *os;

    if (guix_os)
        return;

    os = calloc(1, sizeof(struct guix_os) +
                    MAX_EVENTS * sizeof(struct guix_event));
    if (!os)
        return;

    INIT_LIST_HEAD(&os->queue.pending);
    INIT_LIST_HEAD(&os->queue.free);
    for (int i = 0; i < MAX_EVENTS; i++)
        list_add_tail(&os->event[i].node, &os->queue.free);

    os->mutex = SDL_CreateMutex();
    os->queue.mutex = SDL_CreateMutex();
    os->queue.cond  = SDL_CreateCond();
    guix_os = os;
}

UINT gx_generic_thread_start(VOID(*guix_thread_entry)(ULONG))
{
    struct guix_os *os = guix_os;

    if (!os)
        return GX_FAILURE;
    os->entry = guix_thread_entry;
    os->thread = SDL_CreateThread(guix_os_thread, "guix-thread", NULL);
    //SDL_DetachThread(os->thread);
    return GX_SUCCESS;
}

UINT gx_generic_event_post(GX_EVENT *event_ptr)
{
    struct guix_event_queue *queue = &guix_os->queue;
    struct guix_event *event;

    SDL_LockMutex(queue->mutex);
    if (list_empty(&queue->free)) {
        SDL_UnlockMutex(queue->mutex);
        return GX_FAILURE;
    }

    event = (struct guix_event*)queue->free.next;
    list_del(&event->node);
    event->event_data = *event_ptr;

    /* insert this event into fifo queue */
    if (!list_empty(&queue->pending)) {
        list_add_tail(&event->node, &queue->pending);
    } else {
        list_add_tail(&event->node, &queue->pending);
        SDL_CondSignal(queue->cond);
    }
 
    SDL_UnlockMutex(queue->mutex);
    return GX_SUCCESS;
}

UINT gx_generic_event_fold(GX_EVENT *event_ptr)
{
    struct guix_event_queue *queue = &guix_os->queue;
    struct guix_event *event;
    struct list_head *pos;

    SDL_LockMutex(queue->mutex);
    list_for_each(pos, &queue->pending) {
        event = (struct guix_event*)pos;
        if (event->event_data.gx_event_type == event_ptr->gx_event_type) {
            /* found matching event, update it and return */
            event->event_data.gx_event_payload.gx_event_ulongdata =
                    event_ptr->gx_event_payload.gx_event_ulongdata;
            SDL_UnlockMutex(queue->mutex);
            return GX_SUCCESS;
        }
    }

    SDL_UnlockMutex(queue->mutex);
    gx_generic_event_post(event_ptr);
    return GX_SUCCESS;
}

UINT gx_generic_event_pop(GX_EVENT *put_event, GX_BOOL wait)
{
    struct guix_event_queue *queue = &guix_os->queue;

    if (!wait) {
        if (list_empty_careful(&queue->pending))
            return GX_FAILURE;
    }

    SDL_LockMutex(queue->mutex);
    while (list_empty(&queue->pending))
        SDL_CondWait(queue->cond, queue->mutex);

    struct guix_event* event = (struct guix_event*)queue->pending.next;
    *put_event = event->event_data;

    /* link this event holder back into free list */
    list_del(&event->node);
    list_add(&event->node, &queue->free);
    SDL_UnlockMutex(queue->mutex);
    return GX_SUCCESS;
}

VOID gx_generic_event_purge(GX_WIDGET *target)
{
    struct guix_event_queue* queue = &guix_os->queue;
    struct guix_event *event;
    struct list_head *pos;
    GX_BOOL purge;

    SDL_LockMutex(queue->mutex);
    list_for_each(pos, &queue->pending) {
        event = (struct guix_event *)pos;
        purge = GX_FALSE;
        if (event->event_data.gx_event_target) {
            if (event-> event_data.gx_event_target == target)
                purge = GX_TRUE;
            else
                gx_widget_child_detect(target, event->event_data.gx_event_target, &purge);

            if (purge) {
                list_del(pos);
                list_add(pos, &queue->free);
            }
        }
    }
    SDL_UnlockMutex(queue->mutex);
}

VOID gx_generic_timer_start(VOID)
{
    struct guix_os *os = guix_os;

    if (!os->timer_actived) {
        os->timer_actived = true;
        os->timer = SDL_AddTimer(GX_SYSTEM_TIMER_MS,
            guix_os_timer, NULL);
    }
}

VOID gx_generic_timer_stop(VOID)
{
    struct guix_os *os = guix_os;

    if (os->timer_actived) {
        os->timer_actived = false;
        SDL_RemoveTimer(os->timer);
        os->timer = 0;
    }
}

VOID gx_generic_system_mutex_lock(VOID)
{
    SDL_LockMutex(guix_os->mutex);
}

VOID gx_generic_system_mutex_unlock(VOID)
{
    SDL_UnlockMutex(guix_os->mutex);
}

ULONG gx_generic_system_time_get(VOID)
{
    return(ULONG)SDL_GetTicks();
}

VOID *gx_generic_thread_identify(VOID)
{
    return (VOID *)SDL_ThreadID();
}

VOID gx_generic_time_delay(int ticks)
{
    SDL_Delay(ticks * 1000 / OS_TICKS_PER_SECOND);
}
