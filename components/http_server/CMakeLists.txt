set(SOURCE_EMBEDDED_FILES_CMAKE_PATH ${COMPONENT_DIR}/data/source_embedded_files.cmake)
set(STATIC_ROUTES_HPP_PATH ${COMPONENT_DIR}/static_routes.hpp)

if(NOT EXISTS ${SOURCE_EMBEDDED_FILES_CMAKE_PATH})
    message(FATAL_ERROR "Missing `source_embedded_files.cmake`. Please generate it via `generate_server_embeds.py`.")
endif()

if(NOT EXISTS ${STATIC_ROUTES_HPP_PATH})
    message(FATAL_ERROR "Missing `static_routes.hpp`. Please generate it via `generate_server_embeds.py`.")
endif()

include(${SOURCE_EMBEDDED_FILES_CMAKE_PATH})

idf_component_register(SRCS http_server.cpp
                       INCLUDE_DIRS include
                       REQUIRES esp_http_server
                       EMBED_FILES ${FRONTEND_FILES_TO_EMBED})
