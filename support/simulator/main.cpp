// If IntelliSense is broken, use the "Pick Project Environment" command to switch to the "Simulator" environment.
#include <SDL2/SDL.h>
#include "lvgl.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include "display/monitor.h"
#include "indev/mouse.h"
#include "indev/mousewheel.h"
#include "indev/keyboard.h"
#include "sdl/sdl.h"
#include <ui.h>

#define N_DEPARTURES 6
lv_obj_t *departure_items[N_DEPARTURES];

// TODO Maybe set custom tick to `SDL_GetTicks()` to avoid this
static int tick_thread(void *data)
{
  while (1)
  {
    SDL_Delay(5);
    lv_tick_inc(5);
  }

  return 0;
}

static int timer_thread(void *data)
{
  while (1)
  {
    SDL_Delay(5);
    lv_timer_handler();
  }

  return 0;
}

int main(void)
{
  setvbuf(stdout, NULL, _IONBF, 0); // Disable stdout buffering, so that logs appear instantly

  lv_init();

  /* Add a display
   * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/

  static lv_disp_draw_buf_t disp_buf;
  static lv_color_t buf[SDL_HOR_RES * 10];                       /*Declare a buffer for 10 lines*/
  lv_disp_draw_buf_init(&disp_buf, buf, NULL, SDL_HOR_RES * 10); /*Initialize the display buffer*/

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);           /*Basic initialization*/
  disp_drv.flush_cb = sdl_display_flush; /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
  disp_drv.draw_buf = &disp_buf;
  disp_drv.hor_res = SDL_HOR_RES;
  disp_drv.ver_res = SDL_VER_RES;
  // disp_drv.disp_fill = monitor_fill;      /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
  // disp_drv.disp_map = monitor_map;        /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
  lv_disp_drv_register(&disp_drv);

  /* Add the mouse as input device
   * Use the 'mouse' driver which reads the PC's mouse*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv); /*Basic initialization*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = sdl_mouse_read; /*This function will be called periodically (by the library) to get the mouse position and state*/
  lv_indev_drv_register(&indev_drv);

  sdl_init();

  SDL_CreateThread(tick_thread, "tick", NULL);
  SDL_CreateThread(timer_thread, "timer", NULL);

  ui_init();

  lv_obj_clear_flag(ui_departureitem1, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_departureitem2, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_departureitem3, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_departureitem4, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_departureitem5, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_departureitem6, LV_OBJ_FLAG_HIDDEN);

  while (1)
  {
  }
}
