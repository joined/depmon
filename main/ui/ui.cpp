#include "ui.hpp"

namespace Color {
const lv_color_t black = lv_color_hex(0x000000);
const lv_color_t white = lv_color_hex(0xFFFFFF);
const lv_color_t yellow = lv_color_hex(0xFFFF00);
} // namespace Color

static lv_style_selector_t DEFAULT_SELECTOR = (uint32_t)LV_PART_MAIN | (uint16_t)LV_STATE_DEFAULT;

void Screen::switchTo(lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay) {
    if (screen == nullptr) {
        this->init();
    }

    {
        const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
        lv_scr_load_anim(screen, anim_type, time, delay, true);
    }
}

lv_obj_t *Screen::createPanel(lv_scroll_snap_t snap_type) {
    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    auto *panel = lv_obj_create(screen);
    lv_obj_set_width(panel, lv_pct(100));
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
    lv_obj_set_style_pad_hor(panel, 10, DEFAULT_SELECTOR);
    lv_obj_set_style_pad_ver(panel, 5, DEFAULT_SELECTOR);
    lv_obj_set_style_pad_row(panel, 2, DEFAULT_SELECTOR);
    lv_obj_set_style_pad_column(panel, 0, DEFAULT_SELECTOR);
    return panel;
};

void SplashScreen::init() {
    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
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

void SplashScreen::updateStatus(const std::string &message) {
    if (status == nullptr) {
        return;
    }

    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    lv_label_set_text(status, message.c_str());
};

void LogsScreen::init() {
    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, Color::black, DEFAULT_SELECTOR);
    lv_obj_set_style_bg_opa(screen, 255, DEFAULT_SELECTOR);

    heading = lv_label_create(screen);
    lv_obj_set_x(heading, 20);
    lv_obj_set_y(heading, 7);
    lv_label_set_text(heading, "DepMon Logs");
    lv_obj_set_style_text_font(heading, &roboto_condensed_light_28_4bpp, DEFAULT_SELECTOR);

    panel = createPanel(LV_SCROLL_SNAP_END);
};

void LogsScreen::addLogLine(const std::string &message) {
    if (panel == nullptr) {
        return;
    }

    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    auto *log_line = lv_label_create(panel);
    lv_obj_set_width(log_line, lv_pct(100));
    lv_obj_set_height(log_line, LV_SIZE_CONTENT);
    lv_obj_set_align(log_line, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(log_line, message.c_str());
    lv_obj_set_style_text_font(log_line, &montserrat_regular_16, DEFAULT_SELECTOR);

    lv_obj_scroll_to_y(panel, LV_COORD_MAX, LV_ANIM_OFF);

    if (lv_obj_get_child_cnt(screen) > MAX_LINES) {
        lv_obj_del(lv_obj_get_child(screen, 0));
    }
};

void LogsScreen::addQRCode(const std::string &data, const int size) {
    if (panel == nullptr) {
        return;
    }

    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    auto *qrcode = lv_qrcode_create(panel, size, Color::black, Color::white);
    lv_qrcode_update(qrcode, data.c_str(), data.length());

    lv_obj_scroll_to_y(panel, LV_COORD_MAX, LV_ANIM_OFF);
};

class DepartureItem {
  public:
    void create(lv_obj_t *parent, const std::string &line_text, const std::string &direction_text,
                const std::string &time_text) {
        const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
        item = lv_obj_create(parent);
        lv_obj_set_width(item, lv_pct(100));
        lv_obj_set_height(item, LV_SIZE_CONTENT);
        lv_obj_set_align(item, LV_ALIGN_TOP_LEFT);
        lv_obj_set_style_bg_color(item, Color::black, DEFAULT_SELECTOR);
        lv_obj_set_style_bg_opa(item, 0, DEFAULT_SELECTOR);
        lv_obj_set_style_border_width(item, 0, DEFAULT_SELECTOR);
        lv_obj_set_style_pad_hor(item, 0, DEFAULT_SELECTOR);
        lv_obj_set_style_pad_ver(item, 0, DEFAULT_SELECTOR);
        lv_obj_set_style_text_color(item, Color::yellow, DEFAULT_SELECTOR);
        lv_obj_set_style_text_opa(item, 255, DEFAULT_SELECTOR);
        lv_obj_set_style_text_font(item, &roboto_condensed_regular_28_4bpp, DEFAULT_SELECTOR);

        line = lv_label_create(item);
        lv_obj_set_align(line, LV_ALIGN_LEFT_MID);
        lv_label_set_text(line, line_text.c_str());

        direction = lv_label_create(item);
        lv_obj_set_height(direction,
                          33); // ideally we'd specify "one line" here but we can't, the value is guessed visually
        lv_obj_set_width(direction, 325);
        lv_obj_set_x(direction, 65);
        lv_obj_set_align(direction, LV_ALIGN_LEFT_MID);
        // Only use the circular mode if the label is really long, otherwise it looks weird.
        // TODO The `30` value is a guess (depends on the characters), we should do better.
        // Maybe we can use `lv_txt_get_size`.
        lv_label_set_long_mode(direction, direction_text.length() > 30 ? LV_LABEL_LONG_SCROLL : LV_LABEL_LONG_DOT);
        lv_label_set_text(direction, direction_text.c_str());
        lv_obj_set_style_text_font(direction, &roboto_condensed_light_28_4bpp, DEFAULT_SELECTOR);

        time = lv_label_create(item);
        lv_obj_set_align(time, LV_ALIGN_RIGHT_MID);
        lv_obj_set_x(time, -8);
        lv_label_set_text(time, time_text.c_str());
    }

  private:
    lv_obj_t *item;
    lv_obj_t *line;
    lv_obj_t *direction;
    lv_obj_t *time;
};

void DeparturesScreen::init() {
    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, Color::black, DEFAULT_SELECTOR);

    line = lv_label_create(screen);
    lv_obj_set_x(line, 9);
    lv_obj_set_y(line, 7);
    lv_label_set_text(line, "Line");
    lv_obj_set_style_text_font(line, &roboto_condensed_light_28_4bpp, DEFAULT_SELECTOR);

    direction = lv_label_create(line);
    lv_obj_set_x(direction, 66);
    lv_label_set_text(direction, "Direction");

    departure = lv_label_create(line);
    lv_obj_set_x(departure, 410);
    lv_label_set_text(departure, "ETD");

    panel = createPanel(LV_SCROLL_SNAP_START);
};

void DeparturesScreen::addRandomDepartureItem() {
    if (rand() % 2 == 0) {
        addItem("RE55", "Sonnenallee", std::chrono::minutes(123));
    } else {
        addItem("U7", "U Lorem ipsum sit amet consectetur", std::chrono::minutes(0));
    }
};

void DeparturesScreen::addItem(const std::string &line_text, const std::string &direction_text,
                               const std::optional<std::chrono::seconds> &time_to_departure) {
    if (panel == nullptr) {
        return;
    }

    std::string time_text;
    if (time_to_departure.has_value()) {
        const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(time_to_departure.value()).count();
        time_text = minutes <= 0 ? "Now" : std::to_string(minutes) + "'";
    } else {
        // TODO Use more appropriate symbol for cancelled trips
        time_text = "=";
    }

    DepartureItem departureItem;
    departureItem.create(panel, line_text, direction_text, time_text);
};

void DeparturesScreen::clean() {
    if (panel == nullptr) {
        return;
    }

    const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
    lv_obj_clean(panel);
};

void UIManager::init() {
    // TODO Make use of the theme, allow switching between themes
    {
        const std::lock_guard<std::recursive_mutex> lock(lvgl_mutex);
        auto *dispp = lv_disp_get_default();
        auto *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                            true, LV_FONT_DEFAULT);
        lv_disp_set_theme(dispp, theme);
    }

    splash_screen.switchTo();
};
