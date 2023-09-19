SET(SOURCES components/ui_comp_departureitem.c
    components/ui_comp.c
    screens/ui_mainscreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_terminus28bold.c
    fonts/ui_font_terminus36bold.c)

add_library(ui ${SOURCES})
