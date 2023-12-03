# DepMon

An ESP32 project to monitor departures of the Berliner Verkehrsbetriebe (BVG) using Sunton development boards.

## Development

The project is composed of three main parts:

-   The ESP32 project, developed with the [ESP-IDF framework](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
-   The native simulator for the GUI (based on [LVGL](https://lvgl.io/))
-   The frontend React SPA served by the ESP32 which allows to configure it and get system information

### ESP32

The ESP32 code is based on the [ESP-IDF framework](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

It is designed to run on a Sunton [3248S035C](https://www.openhasp.com/0.7.0/hardware/sunton/esp32-3248s035/) board (3.5", 480x320px).
You can get one [here](https://de.aliexpress.com/item/1005004632953455.html).
In the future, support for other boards is planned.

### Simulator

The simulator code lives in `simulator` and is used to develop the UI. The UI library is symlinked from the ESP folder.
It is developed using the [PlatformIO](https://platformio.org/) framework.
Follow [these instructions](https://platformio.org/install/ide?install=vscode) to get started using it within VSCode.

### Frontend

The frontend is developed using React.
You'll need to use to use [pnpm](https://pnpm.io/) to build it.

Useful commands:

-   `pnpm i`: installs the dependencies
-   `pnpm start`: starts a dev server with hot reloading and the mock backend API server
-   `pnpm build`: builds the gzipped production version of the app, to be stored in the data partition of the ESP

### VSCode C++ Extension

The `simulator` folder should be handled as separate VSCode workspace.
This is due to PlatformIO insisting on owning and overwriting `.vscode/c_cpp_properties.json` which makes it impossible to have a functioning IDE
if you want to use the "ESP-IDF" extension rather than PlatformIO's own build system.
I don't want to use PlatformIO's build system due to the poor integration with `idf.py`.

Please copy `.vscode/c_cpp_properties.sample.json` to `.vscode/c_cpp_properties.json` and read the instructions for ESP-IDF development.
PlatformIO will generate `c_cpp_properties.json` automatically based on `platformio.io` for the simulator.
