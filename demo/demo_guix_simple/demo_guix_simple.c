/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "guix_adaptor.h"

#include "guix_simple_resources.h"
#include "guix_simple_specifications.h"

GX_WINDOW *pHelloScreen;

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
static int guix_main(UINT(*drv_setup)(GX_DISPLAY*))
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(MAIN_DISPLAY, drv_setup,
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);


    /* create the hello world screen */
    gx_studio_named_widget_create("simple_window", (GX_WIDGET *) root, (GX_WIDGET **) &pHelloScreen);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
    return 0;
}

int main(int argc, char* argv[])
{
    struct gui_screen screen;

    screen.do_init = guix_main;
    screen.pixel_format = MAIN_DISPLAY_COLOR_FORMAT;
    screen.pixel_width = 2;
    screen.screen_height = MAIN_DISPLAY_Y_RESOLUTION;
    screen.screen_width = MAIN_DISPLAY_X_RESOLUTION;
    return guix_simulate_start(&screen);
}

