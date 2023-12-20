import json
from pathlib import Path
import shutil

IN_FIRMWARE_PATH = Path(__file__).parent.parent / "build" / "merged-firmware.bin"


def get_firmware_content():
    if not IN_FIRMWARE_PATH.exists():
        print(
            f"Could not find firmware at {IN_FIRMWARE_PATH}, using dummy firmware"
        )

        return "{}"

    manifest = {
        "name": "SunTransit",
        # TODO Get version from somewhere, like Git (hash / hash-dirty)
        "version": "0.1",
        "new_install_prompt_erase": False,
        "builds": [
            {
                "chipFamily": "ESP32",
                "parts": [{"path": IN_FIRMWARE_PATH.name, "offset": 0}],
            }
        ],
    }

    return json.dumps(manifest, indent=4)


def generate_espwebtools_manifest(site_dir: str):
    MANIFEST_PATH = Path(site_dir) / "installer" / "manifest.json"

    with open(MANIFEST_PATH, "w") as out_file:
        out_file.write(get_firmware_content())

def copy_firmware(site_dir: str):
    if not IN_FIRMWARE_PATH.exists():
        return

    OUT_FIRMWARE_PATH = Path(site_dir) / "installer" / IN_FIRMWARE_PATH.name

    shutil.copyfile(IN_FIRMWARE_PATH, OUT_FIRMWARE_PATH)


def on_post_build(config, **kwargs):
    generate_espwebtools_manifest(config["site_dir"])
    copy_firmware(config["site_dir"])
