#ifndef UI_INTERACTION_HPP
#define UI_INTERACTION_HPP

#include <mutex>
#include <string>

#include "ui.h"

std::recursive_mutex lvgl_mutex;

class LogsScreen {
  public:
    static lv_obj_t *addNewLine(const std::string &message) {
        if (ui_logspanel != nullptr) {
            const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
            lv_obj_t *ui_logstext = lv_label_create(ui_logspanel);
            lv_obj_set_width(ui_logstext, 447);
            lv_obj_set_height(ui_logstext, LV_SIZE_CONTENT); /// 1
            lv_label_set_text(ui_logstext, message.c_str());
            lv_obj_set_style_text_font(ui_logstext, &ui_font_terminus16regular, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(ui_logstext, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(ui_logstext, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_scroll_to_y(ui_logspanel, LV_COORD_MAX, LV_ANIM_OFF);
            return ui_logstext;
        }

        return nullptr;
    }

    static bool addLogLine(const std::string &message, const int max_lines = 100) {
        bool lineAdded = addNewLine(message) != nullptr;

        if (lineAdded && lv_obj_get_child_cnt(ui_logspanel) > max_lines) {
            const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
            lv_obj_del(lv_obj_get_child(ui_logspanel, 0));
            return true;
        }

        return false;
    }
};

#endif // UI_INTERACTION_HPP
