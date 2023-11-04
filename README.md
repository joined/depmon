# DepMon

An ESP32 project to monitor departures of the Berliner Verkehrsbetriebe (BVG) using Sunton development boards.

## Development

### ESP32

All the ESP32 code lives in the `esp` folder. It uses the ESP-IDF framework.
To work with it it is recommended to install the "ESP-IDF" VSCode extension.

### Simulator

The simulator code lives in `simulator` and is used to develop the UI. The UI library is symlinked from the ESP folder.
To work with it is recommended to install the "PlatformIO" VSCode extension.

### Frontend

The `frontend` folder contains the frontend code for the configuration Web application powered by the ESP32 board.

Useful commands:

- `yarn`: installs the dependencies
- `yarn serve`: starts a dev server with hot reloading
- `yarn build`: builds the gzipped production version of the app, to be stored in the data partition of the ESP
