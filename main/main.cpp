#include "lcd.h"
#include "esp_wifi.h"
#include "esp_http_client.h"
#include "ArduinoJson.h"
#include "ui.h"

static void app_main_display(void)
{
    /* Task lock */
    lvgl_port_lock(0);

    ui_init();

    /* Task unlock */
    lvgl_port_unlock();
}

bool addLogLine(const std::string &message)
{
  if (ui_logspanel != nullptr)
  {
    lvgl_port_lock(100);
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
    lvgl_port_unlock();
    return true;
  }

  return false;
}

extern "C" void app_main(void)
{
    app_lcd_touch_lvgl_init();

    /* Show LVGL objects */
    app_main_display();

    for (int i = 0; i < 100; i++)
    {
      addLogLine("Test " + std::to_string(i));
      vTaskDelay(100);
    }
}