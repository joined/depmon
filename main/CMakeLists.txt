# TODO Find a way to make use of the CMakeLists.txt in the ui folder instead
set(UI_DIR ./ui)
file(GLOB_RECURSE UI_SRCS ${UI_DIR}/*.c)

idf_component_register(
    SRCS "main.cpp" ${UI_SRCS}
    INCLUDE_DIRS "." ${UI_DIR}
    REQUIRES esp_wifi esp_http_client
)
