# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# "Trim" the build. Include the minimal set of components, main and anything it depends on.
# This requires us to list all the components required by main in its CMakeLists.txt, via `REQUIRES`.
set(COMPONENTS main)

# For the LCD/Touch initializers in `lcd.h`
idf_build_set_property(COMPILE_OPTIONS "-Wno-missing-field-initializers" APPEND)

# Due to `ui_helpers` referencing disabled widgets
# TODO This causes warnings when compiling some C++ files because it's only valid for C/ObjC, fix it
idf_build_set_property(COMPILE_OPTIONS "-Wno-implicit-function-declaration" APPEND)

# TODO Fix deprecated enum conversion warning

project(bvg_departures)
