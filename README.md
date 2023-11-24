# DepMon

An ESP32 project to monitor departures of the Berliner Verkehrsbetriebe (BVG) using Sunton development boards.

## Development

### ESP32

The ESP32 code is based on the ESP-IDF framework.
To work with it it is recommended to install the "ESP-IDF" VSCode extension.

### Simulator

The simulator code lives in `simulator` and is used to develop the UI. The UI library is symlinked from the ESP folder.
To work with it is recommended to install the "PlatformIO" VSCode extension.

### Frontend

The `frontend` folder contains the source of the frontend for the configuration Webapp powered by the ESP32 board.

Useful commands:

- `yarn`: installs the dependencies
- `yarn start`: starts a dev server with hot reloading and the mock backend API server
- `yarn build`: builds the gzipped production version of the app, to be stored in the data partition of the ESP

### VSCode C++ Extension

The `simulator` folder should be handled as separate VSCode workspace.
This is due to PlatformIO insisting on owning and overwriting `.vscode/c_cpp_properties.json` which makes it impossible to have a functioning IDE
if you want to use the "ESP-IDF" extension rather than PlatformIO's own build system.
I don't want to use PlatformIO's build system due to the poor integration with `idf.py`.

Please copy `.vscode/c_cpp_properties.sample.json` to `.vscode/c_cpp_properties.json` and read the instructions for ESP-IDF development.
PlatformIO will generate `c_cpp_properties.json` automatically based on `platformio.io` for the simulator.
