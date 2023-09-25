#include "lvgl.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <mutex>
#include "display/monitor.h"
#include "indev/mouse.h"
#include "indev/mousewheel.h"
#include "indev/keyboard.h"
#include "sdl/sdl.h"
#include <ui.h>

std::recursive_mutex lvgl_mutex;

static int timer_thread(void *data)
{
  while (1)
  {
    SDL_Delay(5);
    {
      const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
      lv_timer_handler();
    }
  }

  return 0;
}

void lvgl_sdl_init()
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

  SDL_CreateThread(timer_thread, "timer", NULL);
}

void wait_for_quit()
{
  while (true)
  {
    // Get the next event
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        // Break out of the loop on quit
        break;
      }
    }
  }
}

int main(void)
{
  lvgl_sdl_init();

  // Cannot use demo widgets :(
  ui_init();

  wait_for_quit();
}
