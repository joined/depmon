#include "display/monitor.h"
#include "indev/keyboard.h"
#include "indev/mouse.h"
#include "indev/mousewheel.h"
#include "lvgl.h"
#include "sdl/sdl.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ui_interaction.hpp>
#include <unistd.h>

void SDLLogCallback(const char *buffer) {
    SDL_Log(buffer);
}

static int timer_thread(void *data) {
    SDL_Log("Starting timer thread\n");
    while (true) {
        ACQUIRE_LOCK();
        lv_timer_handler();
        RELEASE_LOCK();
        SDL_Delay(5);
    }

    return 0;
}

void lvgl_sdl_init() {
    setvbuf(stdout, NULL, _IONBF, 0); // Disable stdout buffering, so that logs appear instantly

    lv_init();
    lv_log_register_print_cb(SDLLogCallback);

    /* Add a display
     * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/

    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf[SDL_HOR_RES * 100];                       /*Declare a buffer for 10 lines*/
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, SDL_HOR_RES * 100); /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = SDL_HOR_RES;
    disp_drv.ver_res = SDL_VER_RES;
    lv_disp_drv_register(&disp_drv);

    /* Add the mouse as input device
     * Use the 'mouse' driver which reads the PC's mouse*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv); /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    // TODO At least one axis seems inverted, fix it (wrap sdl_mouse_read?)
    indev_drv.read_cb = sdl_mouse_read; /*This function will be called periodically (by the library) to get the mouse
                                           position and state*/
    lv_indev_drv_register(&indev_drv);

    sdl_init();
}

int main(void) {
    lvgl_sdl_init();

    ui_init();

    SDL_CreateThread(timer_thread, "timer", NULL);

    SDL_Log("Starting main loop\n");

    // TODO Crashes after a bit with 50ms delay. Why?
    // Log suggest we're not locking properly.
    while (true) {
        LogsScreen::addLogLine("Hello world " + std::to_string(SDL_GetTicks()));
        SDL_Delay(50);
    }
}
