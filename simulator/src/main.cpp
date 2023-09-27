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
#include <string>

std::recursive_mutex lvgl_mutex;

static int timer_thread(void *data)
{
  SDL_Log("Starting timer thread\n");
  while (1)
  {
    {
      const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
      lv_timer_handler();
    }
    SDL_Delay(5);
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
  indev_drv.read_cb = sdl_mouse_read; /*This function will be called periodically (by the library) to get the mouse position and state*/
  lv_indev_drv_register(&indev_drv);

  sdl_init();
}

bool addLogLine(const std::string &message)
{
  if (ui_logspanel != nullptr)
  {
    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    lv_obj_t *ui_logstext = lv_label_create(ui_logspanel);
    lv_obj_set_width(ui_logstext, 447);
    lv_obj_set_height(ui_logstext, LV_SIZE_CONTENT); /// 1
    lv_label_set_text(ui_logstext, message.c_str());
    lv_obj_set_style_text_font(ui_logstext, &ui_font_terminus16regular, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_logstext, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_logstext, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (lv_obj_get_child_cnt(ui_logspanel) > 100)
    {
      lv_obj_t *first_child = lv_obj_get_child(ui_logspanel, 0);
      lv_obj_del(first_child);
    }

    lv_obj_scroll_to_y(ui_logspanel, LV_COORD_MAX, LV_ANIM_OFF);
    return true;
  }

  return false;
}

void addRandomLogLine(lv_timer_t *timer) {
  addLogLine("Hello world");
}

int main(void)
{
  lvgl_sdl_init();

  ui_init();

  SDL_CreateThread(timer_thread, "timer", NULL);

  SDL_Log("Starting main loop\n");

  lv_timer_create(addRandomLogLine, 1000, NULL);

  while (true) {}
}
