# ESP32 BVG Timetable

Using ESP32_3248S035C board.

## How to compile
- Git clone repo
- Open root in VSCode
- [Install](https://platformio.org/install/ide?install=vscode) the "PlatformIO IDE" extension if you haven't already
- The extension should initialize everything on its own
- (Optional) Copy `platformio-local.sample.ini` to `platformio-local.ini` and adapt as needed
- Use the "Build" command of the environment you want to target

## Environments
There are two environments: `esp32dev` and `simulator`.
The `esp32dev` target is designed to be uploaded to a [Sunton ESP32-3248S035C board](https://www.aliexpress.com/item/1005004632953455.html).
The `simulator` environment uses LVGL's support for SDL to run a native simulation.

## Common problems
### IntelliSense is broken, red/yellow squiggles everywhere, includes cannot be followed
Use the "Pick Project Environment" command in VSCode to select either `env:esp32dev` or `env:simulator`
based on which one you're working with.
