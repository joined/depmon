## Step 1: Get the board

You'll need a Sunton [3248S035C](https://www.openhasp.com/0.7.0/hardware/sunton/esp32-3248s035/) board.
You can get one [here](https://de.aliexpress.com/item/1005004632953455.html).
You might want to enclose the board in a case. 3D models to print are available online ([example](https://cults3d.com/en/3d-model/gadget/sunon-esp32-3248s035-matsekberg)).

## Step 2: Flash the firmware

### Option A: Web installer

See [web installer](installer.md).

### Option B: Compile it yourself

Clone the [repository](https://github.com/joined/SunTransit).

Follow the instructions in [development](development.md) to install ESP-IDF and pnpm.

Build the firmware, connect the board via USB and flash it either via the ESP-IDF VSCode extension or `idf.py`.

## Step 3: Configure WiFi

The board needs to be connected to the Internet to retrieve the realtime departures information.
After flash and reboot, a wizard should appear on the board with instructions on how to connect the board to a WiFi access point.
This can be done via the "ESP SoftAP Provisioning" app ([Play Store](https://play.google.com/store/apps/details?id=com.espressif.provsoftap), [Apple Store](https://apps.apple.com/us/app/esp-softap-provisioning/id1474040630)).

## Step 4: Configure station

After the network connection is estabilished, the screen should show a message asking to configure the station to show departures from,
along with the URL to connect to to do so ([http://suntransit.local](http://suntransit.local)).

## Step 5: Profit!
