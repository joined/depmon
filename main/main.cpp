#include "ArduinoJson.h"
#include "esp_http_client.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "lcd.h"
#include "ui_interaction.hpp"

static void app_main_display(void) {
    if (lvgl_port_lock(0)) {
        ui_init();
        lvgl_port_unlock();
    }
}

extern "C" void app_main(void) {
    app_lcd_touch_lvgl_init();

    /* Show LVGL objects */
    app_main_display();

    while (true) {
        LogsScreen::addLogLine("Test " + std::to_string(esp_timer_get_time() / 1000)); 
        vTaskDelay(50);
    }
}