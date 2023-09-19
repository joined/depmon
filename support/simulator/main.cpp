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

#define N_DEPARTURES 6
#define RANDOM_REFRESH_INTERVAL 1000

std::recursive_mutex lvgl_mutex;
lv_obj_t *departure_items[N_DEPARTURES];

void randomDeparturesUpdate(lv_timer_t *timer)
{
  for (int i = 0; i < N_DEPARTURES; i++)
  {
    lv_obj_t *line = ui_comp_get_child(departure_items[i], UI_COMP_DEPARTUREITEM_DEPARTURE0LINE);
    lv_obj_t *destination = ui_comp_get_child(departure_items[i], UI_COMP_DEPARTUREITEM_DEPARTURE0DIRECTION);
    lv_obj_t *time_label = ui_comp_get_child(departure_items[i], UI_COMP_DEPARTUREITEM_DEPARTURE0TIME);

    lv_obj_clear_flag(departure_items[i], LV_OBJ_FLAG_HIDDEN);

    if (rand() % 2 == 0)
    {
      lv_label_set_text(line, "M41");
      lv_label_set_text(destination, "Sonnenallee");
      lv_label_set_text(time_label, "2'");
    }
    else
    {
      lv_label_set_text(line, "U7");
      lv_label_set_text(destination, "U Rudow");
      lv_label_set_text(time_label, "10'");
    }
  }
}

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

  SDL_CreateThread(timer_thread, "timer", NULL);

  ui_init();

  departure_items[0] = ui_departureitem1;
  departure_items[1] = ui_departureitem2;
  departure_items[2] = ui_departureitem3;
  departure_items[3] = ui_departureitem4;
  departure_items[4] = ui_departureitem5;
  departure_items[5] = ui_departureitem6;

  lv_timer_create(randomDeparturesUpdate, RANDOM_REFRESH_INTERVAL, NULL);

  while (1)
  {
  }
}
