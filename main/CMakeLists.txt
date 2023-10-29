file(GLOB_RECURSE FONT_SRCS ui/fonts/*.c)

idf_component_register(
    SRCS "main.cpp" "http_server.cpp" ${FONT_SRCS}
    INCLUDE_DIRS "." "ui"
    PRIV_REQUIRES esp_wifi esp_http_client esp_http_server nvs_flash wifi_provisioning spiffs vfs
)

set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../frontend")
if(EXISTS ${WEB_SRC_DIR}/dist)
    spiffs_create_partition_image(www ${WEB_SRC_DIR}/dist FLASH_IN_PROJECT)
else()
    message(FATAL_ERROR "${WEB_SRC_DIR}/dist doesn't exit. Please run 'yarn build' in ${WEB_SRC_DIR}")
endif()
