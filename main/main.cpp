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

extern "C" void app_main(void)
{
    app_lcd_touch_lvgl_init();

    /* Show LVGL objects */
    app_main_display();
}