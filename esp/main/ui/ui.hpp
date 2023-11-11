#pragma once

#include <chrono>
#include <mutex>
#include <string>
#include "lvgl.h"

inline std::recursive_mutex lvgl_mutex;

LV_FONT_DECLARE(montserrat_regular_16);
LV_FONT_DECLARE(montserrat_regular_28);
LV_FONT_DECLARE(montserrat_regular_96);

class Screen {
  public:
    void switchTo(lv_scr_load_anim_t anim_type = LV_SCR_LOAD_ANIM_NONE, uint32_t time = 0, uint32_t delay = 0);

  protected:
    lv_obj_t *createPanel(lv_scroll_snap_t snap_type);
    lv_obj_t *screen = nullptr;

    virtual void init() = 0;
};

class SplashScreen : public Screen {
  public:
    void init();
    void updateStatus(const std::string &message);

  private:
    lv_obj_t *title = nullptr;
    lv_obj_t *status = nullptr;
    lv_obj_t *spinner = nullptr;
};

class LogsScreen : public Screen {
  public:
    void init();
    void addLogLine(const std::string &message);
    void addQRCode(const std::string &data, const int size = 120);

  private:
    static const int MAX_LINES = 100;
    lv_obj_t *heading = nullptr;
    lv_obj_t *panel = nullptr;
};

class DeparturesScreen : public Screen {
  public:
    void init();
    void addRandomDepartureItem();
    void addItem(const std::string &line_text, const std::string &direction_text,
                 const std::chrono::seconds &time_to_departure);
    void clean();

  private:
    lv_obj_t *line = nullptr;
    lv_obj_t *direction = nullptr;
    lv_obj_t *departure = nullptr;
    lv_obj_t *panel = nullptr;
};

inline SplashScreen splash_screen;
inline LogsScreen logs_screen;
inline DeparturesScreen departures_screen;

namespace UIManager {
void init();
}