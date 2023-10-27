#pragma once

#include "lvgl.h"
#include <mutex>
#include <string>

using namespace std;

recursive_mutex lvgl_mutex;

LV_FONT_DECLARE(montserrat_regular_16);
LV_FONT_DECLARE(montserrat_regular_28);
LV_FONT_DECLARE(montserrat_regular_96);

namespace Color {
const lv_color_t black = lv_color_hex(0x000000);
const lv_color_t white = lv_color_hex(0xFFFFFF);
const lv_color_t yellow = lv_color_hex(0xFFFF00);
} // namespace Color

static lv_style_selector_t DEFAULT_SELECTOR = (uint32_t)LV_PART_MAIN | (uint16_t)LV_STATE_DEFAULT;

class Screen {
  public:
    bool switchTo(lv_scr_load_anim_t anim_type = LV_SCR_LOAD_ANIM_NONE, uint32_t time = 0, uint32_t delay = 0) {
        if (screen != nullptr) {
            const lock_guard<recursive_mutex> lock(lvgl_mutex);
            lv_scr_load_anim(screen, anim_type, time, delay, false);
            return true;
        }
        return false;
    }

  protected:
    lv_obj_t *screen = nullptr;

    lv_obj_t *createPanel(lv_scroll_snap_t snap_type) {
        const lock_guard<recursive_mutex> lock(lvgl_mutex);
        lv_obj_t *panel = lv_obj_create(screen);
        lv_obj_set_width(panel, 447);
        lv_obj_set_height(panel, 250);
        lv_obj_set_y(panel, 5);
        lv_obj_set_align(panel, LV_ALIGN_CENTER);
        lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_set_scroll_snap_y(panel, snap_type);
        lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);
        lv_obj_set_style_radius(panel, 0, DEFAULT_SELECTOR);
        lv_obj_set_style_bg_color(panel, Color::yellow, DEFAULT_SELECTOR);
        lv_obj_set_style_bg_opa(panel, 51, DEFAULT_SELECTOR);
        lv_obj_set_style_border_width(panel, 0, DEFAULT_SELECTOR);
        lv_obj_set_style_pad_hor(panel, 5, DEFAULT_SELECTOR);
        lv_obj_set_style_pad_ver(panel, 5, DEFAULT_SELECTOR);
        lv_obj_set_style_pad_row(panel, 3, DEFAULT_SELECTOR);
        lv_obj_set_style_pad_column(panel, 0, DEFAULT_SELECTOR);
        return panel;
    }
};

class SplashScreen : public Screen {
  public:
    void init() {
        const lock_guard<recursive_mutex> lock(lvgl_mutex);
        screen = lv_obj_create(NULL);
        lv_obj_set_style_bg_color(screen, Color::black, DEFAULT_SELECTOR);

        title = lv_label_create(screen);
        lv_obj_set_y(title, -40);
        lv_obj_set_align(title, LV_ALIGN_CENTER);
        lv_label_set_text(title, "DepMon");
        lv_obj_set_style_text_color(title, Color::white, DEFAULT_SELECTOR);
        lv_obj_set_style_text_font(title, &montserrat_regular_96, DEFAULT_SELECTOR);

        status = lv_label_create(screen);
        lv_obj_set_y(status, -30);
        lv_obj_set_align(status, LV_ALIGN_BOTTOM_MID);
        lv_label_set_text(status, "Booting up...");
        lv_obj_set_style_text_color(status, Color::white, DEFAULT_SELECTOR);
        lv_obj_set_style_text_font(status, &montserrat_regular_16, DEFAULT_SELECTOR);

        spinner = lv_spinner_create(screen, 1000, 90);
        lv_obj_set_width(spinner, 50);
        lv_obj_set_height(spinner, 51);
        lv_obj_set_y(spinner, 60);
        lv_obj_set_align(spinner, LV_ALIGN_CENTER);

        lv_obj_set_style_arc_color(spinner, Color::yellow, (uint32_t)LV_PART_INDICATOR | (uint16_t)LV_STATE_DEFAULT);
    };

  private:
    lv_obj_t *title = nullptr;
    lv_obj_t *status = nullptr;
    lv_obj_t *spinner = nullptr;
};

class LogsScreen : public Screen {
  public:
    void init() {
        const lock_guard<recursive_mutex> lock(lvgl_mutex);
        screen = lv_obj_create(NULL);
        lv_obj_set_style_bg_color(screen, Color::black, DEFAULT_SELECTOR);
        lv_obj_set_style_bg_opa(screen, 255, DEFAULT_SELECTOR);

        heading = lv_label_create(screen);
        lv_obj_set_x(heading, 20);
        lv_obj_set_y(heading, 7);
        lv_label_set_text(heading, "DepMon Logs");
        lv_obj_set_style_text_font(heading, &montserrat_regular_28, DEFAULT_SELECTOR);

        panel = createPanel(LV_SCROLL_SNAP_END);
    }
    bool addLogLine(const string &message) {
        const lock_guard<recursive_mutex> lock(lvgl_mutex);
        bool lineAdded = addNewLine(message) != nullptr;

        if (lineAdded && lv_obj_get_child_cnt(screen) > MAX_LINES) {
            lv_obj_del(lv_obj_get_child(screen, 0));
            return true;
        }

        return false;
    }
    bool addQRCode(const string &data, const int size = 120) {
        if (panel != nullptr) {
            const lock_guard<recursive_mutex> lock(lvgl_mutex);
            lv_obj_t *qrcode = lv_qrcode_create(panel, size, Color::black, Color::white);
            lv_qrcode_update(qrcode, data.c_str(), data.length());

            lv_obj_scroll_to_y(panel, LV_COORD_MAX, LV_ANIM_OFF);

            return true;
        }

        return false;
    }

  private:
    static const int MAX_LINES = 100;
    lv_obj_t *heading = nullptr;
    lv_obj_t *panel = nullptr;

    lv_obj_t *addNewLine(const string &message) {
        if (panel != nullptr) {
            const lock_guard<recursive_mutex> lock(lvgl_mutex);
            lv_obj_t *log_line = lv_label_create(panel);
            lv_obj_set_width(log_line, lv_pct(100));
            lv_obj_set_height(log_line, LV_SIZE_CONTENT);
            lv_obj_set_align(log_line, LV_ALIGN_TOP_LEFT);
            lv_label_set_text(log_line, message.c_str());
            lv_obj_set_style_text_font(log_line, &montserrat_regular_16, DEFAULT_SELECTOR);

            lv_obj_scroll_to_y(panel, LV_COORD_MAX, LV_ANIM_OFF);
            return log_line;
        }

        return nullptr;
    }
};

class DeparturesScreen : public Screen {
  public:
    void init() {
        const lock_guard<recursive_mutex> lock(lvgl_mutex);
        screen = lv_obj_create(NULL);
        lv_obj_set_style_bg_color(screen, Color::black, DEFAULT_SELECTOR);

        line = lv_label_create(screen);
        lv_obj_set_x(line, 15);
        lv_obj_set_y(line, 7);
        lv_label_set_text(line, "Linie");
        lv_obj_set_style_text_font(line, &montserrat_regular_28, DEFAULT_SELECTOR);

        direction = lv_label_create(line);
        lv_obj_set_x(direction, 88);
        lv_label_set_text(direction, "Ziel");

        departure = lv_label_create(line);
        lv_obj_set_x(departure, 342);
        lv_label_set_text(departure, "Abfahrt");

        panel = createPanel(LV_SCROLL_SNAP_START);
    }

    bool addRandomDepartureItem() {
        if (panel != nullptr) {
            DepartureItem departureItem;
            if (rand() % 2 == 0) {
                departureItem.create(panel, "M41", "Sonnenallee", "2'");
            } else {
                departureItem.create(panel, "U7", "U Rudow", "5'");
            }

            return true;
        }

        return false;
    }

  private:
    lv_obj_t *line = nullptr;
    lv_obj_t *direction = nullptr;
    lv_obj_t *departure = nullptr;
    lv_obj_t *panel = nullptr;

    class DepartureItem {
      public:
        void create(lv_obj_t *parent, const string &line_text, const string &direction_text, const string &time_text) {
            const lock_guard<recursive_mutex> lock(lvgl_mutex);
            lv_obj_t *item = lv_obj_create(parent);
            lv_obj_set_width(item, lv_pct(100));
            lv_obj_set_height(item, LV_SIZE_CONTENT);
            lv_obj_set_align(item, LV_ALIGN_TOP_LEFT);
            lv_obj_set_style_bg_color(item, Color::black, DEFAULT_SELECTOR);
            lv_obj_set_style_bg_opa(item, 0, DEFAULT_SELECTOR);
            lv_obj_set_style_border_width(item, 0, DEFAULT_SELECTOR);
            lv_obj_set_style_pad_hor(item, 0, DEFAULT_SELECTOR);
            lv_obj_set_style_pad_top(item, 0, DEFAULT_SELECTOR);
            lv_obj_set_style_pad_bottom(item, 1, DEFAULT_SELECTOR);
            lv_obj_set_style_text_color(item, Color::yellow, DEFAULT_SELECTOR);
            lv_obj_set_style_text_opa(item, 255, DEFAULT_SELECTOR);
            lv_obj_set_style_text_font(item, &montserrat_regular_28, DEFAULT_SELECTOR);

            lv_obj_t *line = lv_label_create(item);
            lv_obj_set_align(line, LV_ALIGN_LEFT_MID);
            lv_label_set_text(line, line_text.c_str());
            lv_obj_set_style_text_font(line, &montserrat_regular_28, DEFAULT_SELECTOR);

            lv_obj_t *direction = lv_label_create(item);
            lv_obj_set_width(direction, 283);
            lv_obj_set_x(direction, 80);
            lv_obj_set_align(direction, LV_ALIGN_LEFT_MID);
            lv_label_set_long_mode(direction, LV_LABEL_LONG_DOT);
            lv_label_set_text(direction, direction_text.c_str());

            lv_obj_t *time = lv_label_create(item);
            lv_obj_set_align(time, LV_ALIGN_RIGHT_MID);
            lv_obj_set_x(time, -8);
            lv_label_set_text(time, time_text.c_str());
        }
    };
};

static SplashScreen splash_screen;
static LogsScreen logs_screen;
static DeparturesScreen departures_screen;

namespace UIManager {

void init() {
    {
        const lock_guard<recursive_mutex> lock(lvgl_mutex);
        lv_disp_t *dispp = lv_disp_get_default();
        lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE),
                                                  lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
        lv_disp_set_theme(dispp, theme);
    }

    splash_screen.init();
    splash_screen.switchTo();

    logs_screen.init();
    departures_screen.init();
};

} // namespace UIManager
