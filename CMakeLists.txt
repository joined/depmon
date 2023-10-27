cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# "Trim" the build. Include the minimal set of components, main and anything it depends on.
# This requires us to list all the components required by main in its CMakeLists.txt, via `REQUIRES`.
set(COMPONENTS main)

# For the LCD/Touch initializers in `lcd.h`
idf_build_set_property(COMPILE_OPTIONS "-Wno-missing-field-initializers" APPEND)

# TODO Fix deprecated enum conversion warning

# TODO The following should be handled via menuconfig, but they're missing in the KConfig
idf_build_set_property(COMPILE_OPTIONS "-DLV_ATTRIBUTE_TIMER_HANDLER=IRAM_ATTR" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-DLV_ATTRIBUTE_TICK_INC=IRAM_ATTR" APPEND)

# For the fonts
idf_build_set_property(COMPILE_OPTIONS "-DLV_LVGL_H_INCLUDE_SIMPLE" APPEND)

project(bvg_departures)
