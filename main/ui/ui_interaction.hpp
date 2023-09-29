#ifndef UI_INTERACTION_HPP
#define UI_INTERACTION_HPP

#include <mutex>
#include <string>

#include "ui.h"

using namespace std;

recursive_mutex lvgl_mutex;

// TODO Extract each screen to a separate file
class LogsScreen {
  public:
    static bool switchTo() {
        if (ui_logs != nullptr) {
            const lock_guard<recursive_mutex> lock(lvgl_mutex);
            lv_disp_load_scr(ui_logs);
            return true;
        }
        return false;
    }

    static lv_obj_t *addNewLine(const string &message) {
        if (ui_logspanel != nullptr) {
            const lock_guard<recursive_mutex> lock(lvgl_mutex);
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

    static bool addLogLine(const string &message, const int max_lines = 100) {
        bool lineAdded = addNewLine(message) != nullptr;

        if (lineAdded && lv_obj_get_child_cnt(ui_logspanel) > max_lines) {
            const lock_guard<recursive_mutex> lock(lvgl_mutex);
            lv_obj_del(lv_obj_get_child(ui_logspanel, 0));
            return true;
        }

        return false;
    }
};

class DeparturesScreen {
  public:
    static bool switchTo() {
        if (ui_departures != nullptr) {
            const lock_guard<recursive_mutex> lock(lvgl_mutex);
            lv_disp_load_scr(ui_departures);
            return true;
        }
        return false;
    }
    static bool addRandomDepartureItem() {
        const lock_guard<recursive_mutex> lock(lvgl_mutex);

        if (ui_departurespanel != nullptr) {
            const lock_guard<recursive_mutex> lock(lvgl_mutex);
            lv_obj_t *newItem = ui_departureitem_create(ui_departurespanel);

            lv_obj_t *line = ui_comp_get_child(newItem, UI_COMP_DEPARTUREITEM_LINE);
            lv_obj_t *destination = ui_comp_get_child(newItem, UI_COMP_DEPARTUREITEM_DIRECTION);
            lv_obj_t *time_label = ui_comp_get_child(newItem, UI_COMP_DEPARTUREITEM_TIME);

            if (rand() % 2 == 0) {
                lv_label_set_text(line, "M41");
                lv_label_set_text(destination, "Sonnenallee");
                lv_label_set_text(time_label, "2'");
            } else {
                lv_label_set_text(line, "U7");
                lv_label_set_text(destination, "U Rudow");
                lv_label_set_text(time_label, "10'");
            }

            return true;
        }

        return false;
    }
};

#endif // UI_INTERACTION_HPP
