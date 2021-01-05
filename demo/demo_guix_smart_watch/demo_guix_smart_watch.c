/* This is a small demo of the high-performance GUIX graphics framework. */
#include<windows.h>
#include<winbase.h>
#include "demo_guix_smart_watch.h"

#include "guix_adaptor.h"

#define SCREEN_STACK_SIZE  5
#define SCRATCHPAD_PIXELS  MAIN_SCREEN_X_RESOLUTION * MAIN_SCREEN_Y_RESOLUTION * 2
#define ANIMATION_ID_MENU_SLIDE 1

/* Define prototypes.   */
VOID start_guix(VOID);
VOID root_window_draw(GX_WINDOW *root);
UINT root_window_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr);
VOID sys_time_update();
VOID clock_update();


#ifdef CONFIG_GUIX_BINRES
GX_CONST GX_THEME* Main_Screen_theme_table[MAIN_SCREEN_THEME_TABLE_SIZE];
GX_CONST GX_STRING* Main_Screen_language_table[MAIN_SCREEN_LANGUAGE_TABLE_SIZE];
GX_PIXELMAP** Main_Screen_theme_default_pixelmap_table;
#endif

extern GX_STUDIO_DISPLAY_INFO guix_smart_watch_display_table[];

//TX_BYTE_POOL       memory_pool;
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];
GX_PIXELMAP_BUTTON home_button;

/* Define menu screen list. */
GX_WIDGET *menu_screen_list[] = {
    (GX_WIDGET *)&main_screen.main_screen_menu_window_1,
    (GX_WIDGET *)&menu_window_2,
    (GX_WIDGET *)&menu_window_3,
    GX_NULL
};

/* Define current visble menus screen index.  */
INT              current_menu = 0;

GX_WINDOW_ROOT  *root;
GX_WIDGET       *current_screen;

const GX_CHAR *day_names[7] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

const GX_CHAR *month_names[12] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

GX_ANIMATION slide_animation;
GX_ANIMATION animation[2];

/*************************************************************************************/
VOID *memory_allocate(ULONG size)
{
	return res_alloc((size_t)size);
}

/*************************************************************************************/
VOID memory_free(VOID *mem)
{
    res_free(mem);
}


/*************************************************************************************/
static int guix_main(UINT(*drv_setup)(GX_DISPLAY *))
{
    GX_RECTANGLE size;

    /* Initialize GUIX. */
    gx_system_initialize();

    /* Assign memory alloc/free functions. */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

#ifdef CONFIG_GUIX_BINRES
    GX_THEME* theme;
    GX_STRING** language;
    int ret;

    /* Load gui resource */
    ret = guix_binres_load("guix_binres.bin", 0, &theme, &language);
    if (ret)
        return ret;

    Main_Screen_theme_default_pixelmap_table = theme->theme_pixelmap_table;
    Main_Screen_theme_table[0] = theme;
    guix_smart_watch_display_table[0].theme_table = Main_Screen_theme_table;
    guix_smart_watch_display_table[0].language_table =
        (GX_CONST GX_STRING**)language;
#endif

    gx_studio_display_configure(MAIN_SCREEN, drv_setup, 
        LANGUAGE_ENGLISH, MAIN_SCREEN_THEME_DEFAULT, &root);

    /* Create main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);
    gx_studio_named_widget_create("left_home_button", (GX_WIDGET *)root, GX_NULL);
    gx_studio_named_widget_create("right_home_button", (GX_WIDGET *)root, GX_NULL);
    current_screen = (GX_WIDGET *)&main_screen;

    /* Create menu screens. */
    gx_studio_named_widget_create("menu_window_2", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("menu_window_3", GX_NULL, GX_NULL);

    /* Create message screen. */
    gx_studio_named_widget_create("msg_screen", GX_NULL, GX_NULL);
    msg_list_children_create(&msg_screen.msg_screen_msg_list);

    /* Create message send screen. */
    gx_studio_named_widget_create("msg_send_screen", GX_NULL, GX_NULL);

    /* Create keyboard screen. */
    gx_studio_named_widget_create("keyboard_screen", GX_NULL, GX_NULL);
    gx_single_line_text_input_style_add(&keyboard_screen.keyboard_screen_input_field, GX_STYLE_CURSOR_ALWAYS | GX_STYLE_CURSOR_BLINK);
    keyboard_children_create();

    /* Create weather screens. */
    gx_studio_named_widget_create("weather_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("weather_LosAngeles", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("weather_SanFrancisco", GX_NULL, GX_NULL);
    future_weather_list_children_create((GX_WIDGET *)&weather_screen.weather_screen_weather_info, CITY_NEW_YORK);
    future_weather_list_children_create((GX_WIDGET *)&weather_SanFrancisco.weather_SanFrancisco_weather_info, CITY_SAN_FRANCISCO);
    future_weather_list_children_create((GX_WIDGET *)&weather_LosAngeles.weather_LosAngeles_weather_info, CITY_LOS_ANGELES);

    /* Create healthy screens.  */
    gx_studio_named_widget_create("healthy_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("healthy_pace", GX_NULL, GX_NULL);
    gx_widget_style_add(&healthy_screen.healthy_screen_heart_rate, GX_STYLE_TEXT_COPY);
    gx_widget_style_add(&healthy_pace.healthy_pace_pace, GX_STYLE_TEXT_COPY);

    /* Create calculator screen. */
    gx_studio_named_widget_create("calculator_screen", GX_NULL, GX_NULL);

    /* Create game screen. */
    gx_studio_named_widget_create("game_screen", GX_NULL, GX_NULL);
    game_list_children_create(&game_screen.game_screen_game_list);

    /* Create clock screen. */
    gx_studio_named_widget_create("clock_screen", GX_NULL, GX_NULL);
    time_list_children_create(&clock_screen.clock_screen_time_list);

    /* Create clock add screen. */
    gx_studio_named_widget_create("clock_add_screen", GX_NULL, GX_NULL);
    city_list_children_create(&clock_add_screen.clock_add_screen_city_list);

    /* Create alarm screen. */
    gx_studio_named_widget_create("alarm_screen", GX_NULL, GX_NULL);
    alarm_list_children_create(&alarm_screen.alarm_screen_alarm_list);

    /* Create alarm add screen. */
    gx_studio_named_widget_create("alarm_add_screen", GX_NULL, GX_NULL);

    /* Create stopwatch screen. */
    gx_studio_named_widget_create("stopwatch_screen", GX_NULL, GX_NULL);
    gx_widget_style_add(&stopwatch_screen.stopwatch_screen_prompt_micro_second, GX_STYLE_TEXT_COPY);
    gx_widget_style_add(&stopwatch_screen.stopwatch_screen_prompt_minute, GX_STYLE_TEXT_COPY);
    gx_widget_style_add(&stopwatch_screen.stopwatch_screen_prompt_second, GX_STYLE_TEXT_COPY);

    /* Create contact screen. */
    gx_studio_named_widget_create("contact_screen", GX_NULL, GX_NULL);
    contact_list_children_create(&contact_screen.contact_screen_contact_list);

    /* Create contact information screen. */
    gx_studio_named_widget_create("contact_info_screen", GX_NULL, GX_NULL);

    /* Create contact information list.  */
    gx_studio_named_widget_create("contact_info_list", GX_NULL, GX_NULL);

    /* Attach contact information list to contact information screen. */
    gx_widget_back_attach((GX_WIDGET *)&contact_info_screen.contact_info_screen_contact_info_window, (GX_WIDGET *)&contact_info_list);
    size = contact_info_screen.contact_info_screen_contact_info_window.gx_widget_size;
    gx_widget_shift((GX_WIDGET *)&contact_info_list, size.gx_rectangle_left, size.gx_rectangle_top, GX_FALSE);

    /* Create contact information edit screen. */
    gx_studio_named_widget_create("contact_info_edit_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("contact_info_edit_list", GX_NULL, GX_NULL);
    gx_widget_back_attach((GX_WIDGET *)&contact_info_edit_screen.contact_info_edit_screen_edit_window, (GX_WIDGET *)&contact_info_edit_list);
    size = contact_info_edit_screen.contact_info_edit_screen_edit_window.gx_widget_size;
    gx_widget_shift((GX_WIDGET *)&contact_info_edit_list, size.gx_rectangle_left, size.gx_rectangle_top, GX_FALSE);

    /* Set text input cursor height. */
    gx_text_input_cursor_height_set(&contact_info_edit_list.contact_info_edit_list_firstname.gx_single_line_text_input_cursor_instance, 22);
    gx_text_input_cursor_height_set(&contact_info_edit_list.contact_info_edit_list_lastname.gx_single_line_text_input_cursor_instance, 22);
    gx_text_input_cursor_height_set(&contact_info_edit_list.contact_info_edit_list_mobile.gx_single_line_text_input_cursor_instance, 22);
    gx_text_input_cursor_height_set(&contact_info_edit_list.contact_info_edit_list_office.gx_single_line_text_input_cursor_instance, 22);
    gx_text_input_cursor_height_set(&contact_info_edit_list.contact_info_edit_list_email.gx_single_line_text_input_cursor_instance, 22);
    gx_text_input_cursor_height_set(&contact_info_edit_list.contact_info_edit_list_address.gx_single_line_text_input_cursor_instance, 22);

    /* Create about screen.  */
    gx_studio_named_widget_create("about_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("about_nested_window", GX_NULL, GX_NULL);

    /* Create map screen. */
    gx_studio_named_widget_create("map_screen", GX_NULL, GX_NULL);
    gx_text_input_cursor_height_set(&map_screen.map_screen_text_input.gx_single_line_text_input_cursor_instance, 22);

    /* Create setting screen.  */
    gx_studio_named_widget_create("settings_screen", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("settings_language", GX_NULL, GX_NULL);

    /* Create slide animation control block.  */
    gx_animation_create(&slide_animation);
    gx_animation_create(&animation[0]);
    gx_animation_create(&animation[1]);

    /* Create QR window */
    gx_studio_named_widget_create("qrcode", GX_NULL, GX_NULL);

    /* Set root window draw function.  */
    gx_widget_draw_set((GX_WIDGET *)root, root_window_draw);
    gx_widget_event_process_set((GX_WIDGET *)root, root_window_event_handler);

    /* Create home button in root window. */
    gx_utility_rectangle_define(&size, 600, 20, 622, 37);
    gx_pixelmap_button_create(&home_button, "", (GX_WIDGET *)root, GX_PIXELMAP_ID_MENU_ICON_HOME, GX_PIXELMAP_ID_MENU_ICON_HOME, GX_NULL,
        GX_STYLE_ENABLED, ID_HOME, &size);
    gx_widget_draw_set(&home_button, custom_pixelmap_button_draw);
    gx_widget_fill_color_set(&home_button, GX_COLOR_ID_GRAY, GX_COLOR_ID_BLUE, GX_COLOR_ID_GRAY);

    /* Add text copy style to time display prompts. */
    gx_widget_style_add(&main_screen.main_screen_hour, GX_STYLE_TEXT_COPY);
    gx_widget_style_add(&main_screen.main_screen_minute, GX_STYLE_TEXT_COPY);

    /* Show the root window.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
	return 0;
}

int main(int argc, char* argv[])
{
    struct gui_screen screen;

    screen.do_init = guix_main;
    screen.pixel_format = MAIN_SCREEN_COLOR_FORMAT;
    screen.pixel_width = 2;
    screen.screen_height = MAIN_SCREEN_Y_RESOLUTION;
    screen.screen_width = MAIN_SCREEN_X_RESOLUTION;
    return guix_simulate_start(&screen);
}

/*************************************************************************************/
VOID root_window_draw(GX_WINDOW *root)
{
GX_PIXELMAP *p_map = GX_NULL;
INT          x_pos;

    /* Call default root window draw. */
    gx_window_draw(root);

    gx_context_pixelmap_get(GX_PIXELMAP_ID_SMART_WATCH_FRAME, &p_map);

    if (p_map)
    {
        /* Draw smart watch frame.  */
        x_pos = (MAIN_SCREEN_X_RESOLUTION - p_map->gx_pixelmap_width) >> 1;
        gx_context_fill_color_set(GX_COLOR_ID_GRAY);
        gx_canvas_pixelmap_draw(x_pos, 0, p_map);
    }

    gx_context_pixelmap_get(GX_PIXELMAP_ID_MS_AZURE_LOGO_SMALL, &p_map);

    if (p_map)
    {
        /* Draw logo. */
        gx_canvas_pixelmap_draw(15, 15, p_map);
    }

    gx_context_pixelmap_get(GX_PIXELMAP_ID_MENU_ICON_HOME, &p_map);
}

/*************************************************************************************/
UINT root_window_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_HOME, GX_EVENT_CLICKED):
        if (current_screen != (GX_WIDGET *)&main_screen)
        {
            /* Switch to main screen. */
            screen_switch((GX_WIDGET *)window, (GX_WIDGET *)&main_screen);
        }
        break;

    default:
        return gx_window_root_event_process((GX_WINDOW_ROOT *)window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID menu_indication_button_update()
{
INT         index;
GX_WIDGET   *widget;

    /* Search current visible menu index. */
    for (index = 0; index < 3; index++)
    {
        widget = menu_screen_list[index];

        if (widget->gx_widget_status & GX_STATUS_VISIBLE)
        {
            current_menu = index;
            break;
        }
    }

    gx_system_widget_find(ID_BTN_MENU_0, GX_SEARCH_DEPTH_INFINITE, &widget);
    gx_widget_fill_color_set(widget, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
    gx_widget_style_remove(widget, GX_STYLE_BUTTON_PUSHED);

    gx_system_widget_find(ID_BTN_MENU_1, GX_SEARCH_DEPTH_INFINITE, &widget);
    gx_widget_fill_color_set(widget, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
    gx_widget_style_remove(widget, GX_STYLE_BUTTON_PUSHED);

    gx_system_widget_find(ID_BTN_MENU_2, GX_SEARCH_DEPTH_INFINITE, &widget);
    gx_widget_fill_color_set(widget, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
    gx_widget_style_remove(widget, GX_STYLE_BUTTON_PUSHED);


    switch (current_menu)
    {
    case 0:
        gx_system_widget_find(ID_BTN_MENU_0, GX_SEARCH_DEPTH_INFINITE, &widget);
        break;
    case 1:
        gx_system_widget_find(ID_BTN_MENU_1, GX_SEARCH_DEPTH_INFINITE, &widget);
        break;
    case 2:
        gx_system_widget_find(ID_BTN_MENU_2, GX_SEARCH_DEPTH_INFINITE, &widget);
        break;
    }
    gx_widget_fill_color_set(widget, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
    gx_widget_style_add(widget, GX_STYLE_BUTTON_PUSHED);
}

/*************************************************************************************/
UINT main_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        /* Set current time. */
        clock_update();

        /* Start a timer to update time. */
        gx_system_timer_start(&main_screen, CLOCK_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);
        return template_main_event_handler(window, event_ptr);

    case GX_SIGNAL(ID_MESSAGE, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&msg_screen);
        break;
    case GX_SIGNAL(ID_WEATHER, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&weather_screen);
        break;
    case GX_SIGNAL(ID_CALCULATOR, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&calculator_screen);
        break;
    case GX_SIGNAL(ID_HEALTHY, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&healthy_screen);
        break;

    case GX_SIGNAL(ID_GAMES, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&game_screen);
        break;
    case GX_SIGNAL(ID_CLOCK, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&clock_screen);
        break;
    case GX_SIGNAL(ID_CONTACTS, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&contact_screen);
        break;
    case GX_SIGNAL(ID_ABOUT, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&about_screen);
        break;
    case GX_SIGNAL(ID_MAP, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&map_screen);
        break;
    case GX_SIGNAL(ID_SETTINGS, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET *)&settings_screen);
        break;
    case GX_SIGNAL(ID_QR, GX_EVENT_CLICKED):
        screen_switch(window->gx_widget_parent, (GX_WIDGET*)&qrcode);
        break;

    case GX_SIGNAL(ID_BTN_MENU_0, GX_EVENT_RADIO_SELECT):
        screen_list_slide(menu_screen_list, (GX_WIDGET *)&main_screen.main_screen_menu_window, SLIDE_RIGHT);
        break;

    case GX_SIGNAL(ID_BTN_MENU_1, GX_EVENT_RADIO_SELECT):
        if (current_menu == 0)
        {
            screen_list_slide(menu_screen_list, (GX_WIDGET *)&main_screen.main_screen_menu_window, SLIDE_LEFT);
        }
        else
        {
            screen_list_slide(menu_screen_list, (GX_WIDGET *)&main_screen.main_screen_menu_window, SLIDE_RIGHT);
        }
        break;

    case GX_SIGNAL(ID_BTN_MENU_2, GX_EVENT_RADIO_SELECT):
        screen_list_slide(menu_screen_list, (GX_WIDGET *)&main_screen.main_screen_menu_window, SLIDE_LEFT);
        break;
    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == CLOCK_TIMER)
        {
            clock_update();
        }
        break;
    }
    return gx_window_event_process(window, event_ptr);
}

/*************************************************************************************/
VOID start_slide_animation(GX_WINDOW *window)
{
GX_ANIMATION_INFO slide_animation_info;

    memset(&slide_animation_info, 0, sizeof(GX_ANIMATION_INFO));
    slide_animation_info.gx_animation_parent = (GX_WIDGET *)window;
    slide_animation_info.gx_animation_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_HORIZONTAL;
    slide_animation_info.gx_animation_id = ANIMATION_ID_MENU_SLIDE;
    slide_animation_info.gx_animation_frame_interval = 1;
    slide_animation_info.gx_animation_slide_screen_list = menu_screen_list;

    gx_animation_drag_enable(&slide_animation, (GX_WIDGET *)window, &slide_animation_info);
}

/*************************************************************************************/
UINT menu_window_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        start_slide_animation(window);
        return gx_widget_event_process(window, event_ptr);

    case GX_EVENT_HIDE:
        gx_animation_drag_disable(&slide_animation, (GX_WIDGET *)window);
        return gx_widget_event_process(window, event_ptr);

    case GX_EVENT_ANIMATION_COMPLETE:
        menu_indication_button_update();
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID clock_update()
{
#ifdef WIN32
    GX_CHAR hour_string_buffer[MAX_TIME_TEXT_LENGTH + 1];
    GX_CHAR minute_string_buffer[MAX_TIME_TEXT_LENGTH + 1];
    GX_CHAR am_pm_buffer[MAX_TIME_TEXT_LENGTH + 1];
    GX_CHAR date_string_buffer[20];
    GX_STRING string;

    SYSTEMTIME local_time;
    GetLocalTime(&local_time);

    if (local_time.wSecond & 0x1)
    {
        gx_widget_fill_color_set(&main_screen.main_screen_lower_dot, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
        gx_widget_fill_color_set(&main_screen.main_screen_upper_dot, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
    }
    else
    {
        gx_widget_fill_color_set(&main_screen.main_screen_lower_dot, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
        gx_widget_fill_color_set(&main_screen.main_screen_upper_dot, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY, GX_COLOR_ID_GRAY);
    }

    if (local_time.wHour < 12)
    {
        sprintf(hour_string_buffer, "%02d", local_time.wHour);
        sprintf(minute_string_buffer, "%02d", local_time.wMinute);
        memcpy(am_pm_buffer, "AM", 3);
    }
    else
    {
        sprintf(hour_string_buffer, "%02d", local_time.wHour);
        sprintf(minute_string_buffer, "%02d", local_time.wMinute);
        memcpy(am_pm_buffer, "PM", 3);
    }

    string.gx_string_ptr = hour_string_buffer;
    string.gx_string_length = string_length_get(hour_string_buffer, MAX_TIME_TEXT_LENGTH);
    gx_prompt_text_set_ext(&main_screen.main_screen_hour, &string);

    string.gx_string_ptr = minute_string_buffer;
    string.gx_string_length = string_length_get(minute_string_buffer, MAX_TIME_TEXT_LENGTH);
    gx_prompt_text_set_ext(&main_screen.main_screen_minute, &string);

    string.gx_string_ptr = am_pm_buffer;
    string.gx_string_length = string_length_get(am_pm_buffer, MAX_TIME_TEXT_LENGTH);
    gx_prompt_text_set_ext(&main_screen.main_screen_am_pm, &string);

    sprintf(date_string_buffer, "%s, %s %02d", day_names[local_time.wDayOfWeek], month_names[local_time.wMonth - 1], local_time.wDay);

    string.gx_string_ptr = date_string_buffer;
    string.gx_string_length = string_length_get(date_string_buffer, sizeof(date_string_buffer) - 1);
    gx_prompt_text_set_ext(&main_screen.main_screen_date, &string);

#else
#endif
}

/*************************************************************************************/
VOID root_home_button_draw(GX_PIXELMAP_BUTTON *widget)
{
    if (widget->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        gx_pixelmap_button_draw(widget);
    }
}

/*************************************************************************************/
VOID screen_switch(GX_WIDGET *parent, GX_WIDGET *new_screen)
{
    gx_widget_detach(current_screen);
    gx_widget_attach(parent, new_screen);
    current_screen = new_screen;
}

/*************************************************************************************/
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
{
    UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}