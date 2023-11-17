file(GLOB_RECURSE FONT_SRCS ui/fonts/*.c)

idf_component_register(
    SRCS "nvs_engine.cpp" "utils.cpp" "bvg_api_client.cpp" "lcd.cpp" "main.cpp" "http_server.cpp" "ui/ui.cpp" "time.cpp" ${FONT_SRCS}
    INCLUDE_DIRS "." "ui"
    PRIV_REQUIRES esp_app_format esp_http_client esp_http_server esp_timer esp_wifi json nvs_flash spiffs vfs wifi_provisioning lwip
)

set(FRONTEND_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../frontend")

if(DEFINED ENV{GITHUB_ACTIONS})
    message(STATUS "Running on GitHub Actions. Assuming that web UI is already built.")

    spiffs_create_partition_image(www ${FRONTEND_DIR}/dist FLASH_IN_PROJECT)
else()
    find_program(YARN_EXECUTABLE yarn)
    if(NOT YARN_EXECUTABLE)
        message(FATAL_ERROR "`yarn` not found, please install it.")
    endif()

    # TODO This breaks if the dev server is running, fix it
    ExternalProject_Add(
        build_frontend
        PREFIX ${CMAKE_BINARY_DIR}/build_frontend
        SOURCE_DIR ${FRONTEND_DIR}
        CONFIGURE_COMMAND ${YARN_EXECUTABLE}
        USES_TERMINAL_BUILD true
        BUILD_COMMAND ${YARN_EXECUTABLE} build
        INSTALL_COMMAND ""
        # TODO Ideally we would be building the frontend only if any files changed since the last build,
        # but that seems to be impossible with ExternalProject_Add.
        BUILD_ALWAYS ON
        BUILD_IN_SOURCE TRUE
    )

    spiffs_create_partition_image(www ${FRONTEND_DIR}/dist FLASH_IN_PROJECT DEPENDS build_frontend)
endif()