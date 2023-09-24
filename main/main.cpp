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
    /* LCD HW initialization */
    ESP_ERROR_CHECK(app_lcd_init());

    /* Touch initialization */
    ESP_ERROR_CHECK(app_touch_init());

    /* LVGL initialization */
    ESP_ERROR_CHECK(app_lvgl_init());

    /* Show LVGL objects */
    app_main_display();
}