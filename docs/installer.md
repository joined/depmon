# Firmware Web Installer

{% if config.extra['has_firmware'] %}
Click the button below to installer the latest development firmware (commit [{{ git.short_commit }}]({{ config['repo_url'] }}/commit/{{ git.commit }})). Powered by [ESP Web Tools](https://esphome.github.io/esp-web-tools/).

<!-- TODO The dialog styling does not match the rest, fix it -->
<esp-web-install-button manifest="/installer/manifest.json"/>
{% else %}
Something went wrong when building the latest version of the firmware.
{% endif %}
