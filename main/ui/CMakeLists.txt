SET(SOURCES components/ui_comp_departureitem.c
    components/ui_comp.c
    screens/ui_splash.c
    screens/ui_logs.c
    screens/ui_departures.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_ubahn_png.c
    fonts/ui_font_terminus16regular.c
    fonts/ui_font_terminus28bold.c
    fonts/ui_font_terminus36bold.c
    fonts/ui_font_robotocondensed96regular.c)

add_library(ui ${SOURCES})
