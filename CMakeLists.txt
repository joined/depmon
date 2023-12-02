cmake_minimum_required(VERSION 3.16)
# TODO Set `PROJECT_VER`
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# "Trim" the build. Include the minimal set of components, main and anything it depends on.
# This requires us to list all the components required by main in its CMakeLists.txt, via `REQUIRES`.
set(COMPONENTS esp)
set(EXTRA_COMPONENT_DIRS esp)

# For the LCD/Touch initializers in `lcd.h`
idf_build_set_property(COMPILE_OPTIONS "-Wno-missing-field-initializers" APPEND)

# TODO Fix deprecated enum conversion warning
# Seems like it's fixed on `master`, but not in the latest release (v8.3)

# TODO The following should be handled via menuconfig, but they're missing in the KConfig
idf_build_set_property(COMPILE_OPTIONS "-DLV_ATTRIBUTE_TIMER_HANDLER=IRAM_ATTR" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-DLV_ATTRIBUTE_TICK_INC=IRAM_ATTR" APPEND)

# For the fonts
idf_build_set_property(COMPILE_OPTIONS "-DLV_LVGL_H_INCLUDE_SIMPLE" APPEND)

project(depmon)
