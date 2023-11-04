#pragma once

#include <mutex>
#include <string>
#include "lvgl.h"

inline std::recursive_mutex lvgl_mutex;

LV_FONT_DECLARE(montserrat_regular_16);
LV_FONT_DECLARE(montserrat_regular_28);
LV_FONT_DECLARE(montserrat_regular_96);


class Screen {
  public:
    bool switchTo(lv_scr_load_anim_t anim_type = LV_SCR_LOAD_ANIM_NONE, uint32_t time = 0, uint32_t delay = 0);

  protected:
    lv_obj_t *createPanel(lv_scroll_snap_t snap_type);
    lv_obj_t *screen = nullptr;
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
    bool addLogLine(const std::string &message);
    bool addQRCode(const std::string &data, const int size = 120);

  private:
    static const int MAX_LINES = 100;
    lv_obj_t *heading = nullptr;
    lv_obj_t *panel = nullptr;
    lv_obj_t *addNewLine(const std::string &message);
};

class DeparturesScreen : public Screen {
  public:
    void init();
    bool addRandomDepartureItem();
    bool addItem(const std::string &line_text, const std::string &direction_text, const std::string &time_text);
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