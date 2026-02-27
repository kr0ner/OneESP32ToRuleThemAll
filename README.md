### This project aims to provide all kinds of useful features for your Home Assistant instance using a single ESP32 board.

If you like it, help me code all night 😴

<a href="https://www.buymeacoffee.com/kr0ner" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>

## Features:

* SmartMeter readings via optical sensor [https://amzn.to/4sbL7va]
* WaterMeter readings via [https://amzn.to/4sbL7va]
* Controlling a Tecalor THZ 504 (or other Tecalor | Stiebel Eltron heat pumps) via ESP32 + CAN using an MCP2515 [https://amzn.to/4rJ538H] or the Waveshare Industrial ESP32-S3 Control Board with RS485 and CAN Communication [https://amzn.to/4scTlDk]. If you want a more reliable connection, ESP32-S3 ETH development board [https://amzn.to/40qj5zU] might the right choice. There is also a PoE version of that [https://amzn.to/4sbZjnS]
* Dallas DS18B20 1-Wire temperature sensors for precise return flow measurement: [https://amzn.to/4u8ExHr]

## Community Statistics

<img width="2033" height="1044" alt="Screenshot from 2026-02-24 09-58-46" src="https://github.com/user-attachments/assets/698f1180-7b1c-4e2c-ab42-f3b1ec40256f" />

You are highly encouraged to contribute your telemetry data! By opting in and providing your district, you can compare your local heat pump metrics with the wider community. This grants you access to a global Grafana dashboard and allows you to subscribe to the community's minimum, maximum, and average values via an MQTT topic (updated every 15 minutes).

To opt-in, uncomment the analytics block in your configuration file and replace `"Landkreis"` with your actual district:

~~~yaml
# To opt-in to community stats, uncomment the line below.
# This sends anonymous data to an MQTT server:
# - Events: Triggered by switch/setting changes.
# - Telemetry: Every 15 minutes for standard sensors (custom sensors are NEVER exposed!).
# A unique Device ID will be visible as a sensor. This can be used to access the Grafana dashboard and compare your data with the community.
analytics: !include { file: OneESP32ToRuleThemAll/yaml/features/analytics.yaml, vars: { district: '"Landkreis"' }} # <-- remember to change the district
~~~

## Prerequisites:

* A working Home Assistant installation [https://www.home-assistant.io/]
* ESPHome AddOn installed [https://www.home-assistant.io/integrations/esphome/]

## HowTo:

1.  Navigate to your esphome folder
2.  Clone the repo:
    ~~~bash
    git clone https://github.com/kr0ner/OneESP32ToRuleThemAll.git
    ~~~
3.  Copy or symlink `OneESP32ToRuleThemAll/esp32-poe-technik.yaml` to your esphome folder
4.  Change board type if needed
5.  Comment out packages you don't want/need
6.  Add Wifi if necessary
7.  Change pins for SPI/CAN if needed
8.  Provide HA entities for temperature and humidity

## Hierarchy

~~~mermaid
flowchart LR
    core[core.yaml<br>System Core] --> wpl13([wpl13.yaml<br>Manifest])
    core --> txx[txx_base.yaml<br>Common TXX Base]

    txx --> ttf07([ttf07.yaml<br>Manifest])
    txx --> thz[thz_base.yaml<br>Common THZ Base]
    txx --> thz304([thz304.yaml<br>Manifest])

    thz --> thz404([thz404.yaml<br>Manifest])
    thz --> thz504([thz504.yaml<br>Manifest])
    thz --> thz55([thz5_5_eco.yaml<br>Manifest])

    style core stroke-width:2px,stroke-dasharray: 5 5
    style wpl13 stroke-width:2px
    style ttf07 stroke-width:2px
    style thz304 stroke-width:2px
    style thz404 stroke-width:2px
    style thz504 stroke-width:2px
    style thz55 stroke-width:2px
~~~

* **System Core (`core.yaml`)**: Fundamental hardware and network operations.
* **Series Common Layer (`txx_base.yaml`)**: Shared logic specifically for the TXX (THZ/TTF) family.
* **Model Base Layer (`thz_base.yaml`)**: Heavy lifting and C++ logic for specific sub-families.
* **Device Manifests (e.g. `thz304.yaml`, `thz504.yaml`, `wpl13.yaml`)**: The clean, top-level "recipes" used to compile the firmware.

## Add new readings from heat pump / adopt to your device

1.  Navigate through the menu of the display, mounted to your heat pump
2.  Try to match the values (e.g. 237) to the values you see in the display (23.7°)
3.  Use the existing templates to add new sensors
4.  Add the newly added ids along with their types to `property.h`
5.  Guard the newly added ids with e.g. `#if defined(THZ_5_5_ECO) ... #endif` to express that it was tested with that version. Once it is confirmed for the others, the guards can be removed.
6.  Add the entities to the respective yaml and use the already existing templates. For config values that can be set, use `wp_number.yaml`. For sensors that are updated automatically chose the template that fits best.
7.  Profit

## Troubleshooting

### You are not able to control your heat pump
--> Increase the log level and try to find out which CAN identifiers to use. Also check the forum and `communciation.h`.

### You get garbage values for certain sensors
--> There is no official description of the IDs available. Everything is somewhat trial-and-error. Also, there are many differences between the different heat pumps. Play around with the display and try to find out the correct IDs.

***

Heavily inspired by this post in the HA community forum:
https://community.home-assistant.io/t/configured-my-esphome-with-mcp2515-can-bus-for-stiebel-eltron-heating-pump/366053

and

https://github.com/bullitt186/ha-stiebel-control

Special thanks to [@hovhannes85](https://github.com/hovhannes85)

### Useful links
https://www.stiebel-eltron.de/content/dam/ste/cdbassets/historic/bedienungs-_u_installationsanleitungen/ISG_Modbus__b89c1c53-6d34-4243-a630-b42cf0633361.pdf
