// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: bvg_departures

#ifndef _BVG_DEPARTURES_UI_H
#define _BVG_DEPARTURES_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
  #if __has_include("lvgl.h")
    #include "lvgl.h"
  #elif __has_include("lvgl/lvgl.h")
    #include "lvgl/lvgl.h"
  #else
    #include "lvgl.h"
  #endif
#else
  #include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "components/ui_comp.h"
#include "components/ui_comp_hook.h"
#include "ui_events.h"
// SCREEN: ui_splash
void ui_splash_screen_init(void);
extern lv_obj_t *ui_splash;
extern lv_obj_t *ui_splashtitle;
extern lv_obj_t *ui_splashspinner;
// SCREEN: ui_logs
void ui_logs_screen_init(void);
extern lv_obj_t *ui_logs;
extern lv_obj_t *ui_ubahn;
extern lv_obj_t *ui_heading;
extern lv_obj_t *ui_logspanel;
// SCREEN: ui_mainscreen
void ui_mainscreen_screen_init(void);
extern lv_obj_t *ui_mainscreen;
extern lv_obj_t *ui_line;
extern lv_obj_t *ui_direction;
extern lv_obj_t *ui_departure;
extern lv_obj_t *ui_departurespanel;
extern lv_obj_t *ui____initial_actions0;

LV_IMG_DECLARE( ui_img_ubahn_png);   // assets/ubahn.png

LV_FONT_DECLARE( ui_font_terminus16regular);
LV_FONT_DECLARE( ui_font_terminus28bold);
LV_FONT_DECLARE( ui_font_terminus36bold);
LV_FONT_DECLARE( ui_font_robotocondensed96regular);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
