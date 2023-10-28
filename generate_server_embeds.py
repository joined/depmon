# Assumption: yarn build has been run in the frontend directory

# TODO Clean up this mess

from pathlib import Path
import shutil

frontend_build_folder = Path("frontend/dist")
component_folder = Path("components/http_server")
static_routes_file = component_folder / "static_routes.hpp"
target_data_folder = component_folder / "data"
cmake_file = target_data_folder / "source_embedded_files.cmake"


def create_target_folder_and_clean_it_up():
    if not target_data_folder.exists():
        print(f"Creating {target_data_folder}")
        target_data_folder.mkdir(parents=True)

    print(f"Cleaning up {target_data_folder}")
    for file in target_data_folder.glob("*.*"):
        if file.is_file():
            file.unlink()


def get_target_location(source_file: Path):
    return target_data_folder / source_file.name


def get_embed_files_cmake_content(files_to_embed: list[Path]):
    files_to_embed_relative = [
        file.relative_to(component_folder) for file in files_to_embed
    ]
    return (
        "# AUTOMATICALLY GENERATED FILE\n"
        "# DO NOT EDIT\n"
        "set(FRONTEND_FILES_TO_EMBED\n"
        + "\n".join([f"    {filename}" for filename in files_to_embed_relative])
        + "\n)"
        "\nset(FRONTEND_FILES_TO_EMBED ${FRONTEND_FILES_TO_EMBED} PARENT_SCOPE)"
    )


def get_blob_variable_name(file: Path):
    return f"{file.stem.replace('.', '')}Data"


def get_handler_name_for_file(file: Path):
    return f"{file.stem.replace('.', '')}Handler"


def get_blob_declaration(file: Path):
    asm_name = f"_binary_{file.name.replace('.', '_')}_start"
    return f'extern const char {get_blob_variable_name(file)}[] asm("{asm_name}");'


def get_content_type_for_file(file: Path):
    if file.suffix == ".js":
        return "text/javascript"
    elif file.suffix == ".css":
        return "text/css"
    elif file.suffix == ".html":
        return "text/html"
    else:
        raise Exception(f"Unknown file type {file}")


def get_handler_declaration(file: Path):
    uri = f"/{file.name}" if file.name != "index.html" else "/"
    blob_name = get_blob_variable_name(file)
    return "\n".join(
        [
            f'httpd_uri_t {get_handler_name_for_file(file)} = '
            f'{{.uri = "{uri}", .method = HTTP_GET, .handler = [](httpd_req_t *req) {{',
            f'                                       ESP_LOGI("HTTPD", "GET request received for {uri}");',
            f'                                       httpd_resp_set_type(req, "{get_content_type_for_file(file)}");',
            f"                                      return httpd_resp_send(req, {blob_name}, HTTPD_RESP_USE_STRLEN);",
            "                                 }};",
        ]
    )


def get_static_routes_header_content(files_to_embed: list[Path]):
    # TODO assert no filename collisions
    return (
        "// AUTOMATICALLY GENERATED FILE\n"
        "// DO NOT EDIT\n"
        "#pragma once\n\n"
        "#include <esp_http_server.h>\n"
        "#include <esp_log.h>\n\n"
        + "\n".join([get_blob_declaration(file) for file in files_to_embed])
        + "\n\n"
        + "\n".join([get_handler_declaration(file) for file in files_to_embed])
        + "\n\n"
        "void register_static_routes(httpd_handle_t server_handle) {\n"
        + "\n".join(
            [
                f"    httpd_register_uri_handler(server_handle, &{get_handler_name_for_file(file)});"
                for file in files_to_embed
            ]
        )
        + "\n}"
    )


if __name__ == "__main__":
    relevant_files = [
        file
        for file in frontend_build_folder.glob("*.*")
        if file.is_file() and file.suffix in [".js", ".css", ".html"]
    ]

    create_target_folder_and_clean_it_up()

    new_locations = [get_target_location(file) for file in relevant_files]

    for source_file in relevant_files:
        target_file = get_target_location(source_file)
        print(f"Copying {source_file} to {target_file}")
        shutil.copy(source_file, target_file)

    print(f"Writing cmake file {cmake_file}")
    with open(cmake_file, "w") as cmake_file_handle:
        cmake_file_handle.write(get_embed_files_cmake_content(new_locations))

    print(f"Writing static routes header file {component_folder / 'static_routes.h'}")
    with open(static_routes_file, "w") as static_routes_file_handle:
        static_routes_file_handle.write(get_static_routes_header_content(new_locations))
