#include "ui.h"
#include <string>

// Abstraction for the simulator and the ESP32
// Must use this in the simulator (as the mutex is defined here),
// optional in the ESP32 (can use lvgl_port_lock/unlock directly)
#ifdef CONFIG_IDF_TARGET_ESP32
#include "esp_lvgl_port.h"

#define ACQUIRE_LOCK(x) lvgl_port_lock(x)
#define RELEASE_LOCK() lvgl_port_unlock()

#else
#include <mutex>
std::recursive_mutex lvgl_port_mutex;

#define ACQUIRE_LOCK(x) lvgl_port_mutex.try_lock()
#define RELEASE_LOCK() lvgl_port_mutex.unlock()
#endif

class LogsScreen {
  public:
    static lv_obj_t *addNewLine(const std::string &message) {
        if (ui_logspanel != nullptr && ACQUIRE_LOCK(0)) {
            lv_obj_t *ui_logstext = lv_label_create(ui_logspanel);
            lv_obj_set_width(ui_logstext, 447);
            lv_obj_set_height(ui_logstext, LV_SIZE_CONTENT); /// 1
            lv_label_set_text(ui_logstext, message.c_str());
            lv_obj_set_style_text_font(ui_logstext, &ui_font_terminus16regular, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(ui_logstext, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(ui_logstext, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_scroll_to_y(ui_logspanel, LV_COORD_MAX, LV_ANIM_OFF);
            RELEASE_LOCK();
            return ui_logstext;
        }

        return nullptr;
    }

    static bool addLogLine(const std::string &message, const int max_lines = 100) {
        bool lineAdded = addNewLine(message) != nullptr;

        if (lineAdded && lv_obj_get_child_cnt(ui_logspanel) > max_lines && ACQUIRE_LOCK(0)) {
            lv_obj_del(lv_obj_get_child(ui_logspanel, 0));
            RELEASE_LOCK();
            return true;
        }

        return false;
    }
};