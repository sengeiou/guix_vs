#ifndef GUIX_SIM_H_
#define GUIX_SIM_H_

#include "gx_display.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gui_screen {
    int (*do_init)(UINT(*drv_setup)(GX_DISPLAY*));
    uint32_t screen_width;
    uint32_t screen_height;
    uint32_t pixel_format;
    uint32_t pixel_width;
};

int guix_simulate_start(const struct gui_screen* gui);

#ifdef __cplusplus
}
#endif
#endif /* GUIX_SIM_H_ */