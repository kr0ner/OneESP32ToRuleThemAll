This project aims to provide all kinds of useful features for your Home Assistant instance using a single ESP32 board.

If you like the project, help me code all night 😴

<a href="https://www.buymeacoffee.com/kr0ner" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>

## Features:

  - SmartMeter readings via optical sensor [https://amzn.eu/d/1a28LBy]
  - WaterMeter readings via [https://amzn.eu/d/8Rx0UVJ]
  - Controlling a Tecalor THZ 504 (or other Tecalor | Stiebel Eltron heat pumps) via CAN  [https://amzn.eu/d/5nJJK3r]

## Prerequisites:

  - A working Home Assistant installation [https://www.home-assistant.io/]
  - ESPHome AddOn installed [https://www.home-assistant.io/integrations/esphome/]

## HowTo:

1. navigate to your esphome folder
2. clone the repo (git clone https://github.com/kr0ner/OneESP32ToRuleThemAll.git)
3. copy or symlink OneESP32ToRuleThemAll/esp32-poe-technik.yaml to your esphome folder
4. change board type if needed
5. comment out packages you don't want/need
6. add Wifi if necessary
7. change pins for SPI/CAN if needed
8. provide HA entities for temperature and humidity

## Add new readings from heat pump / adopt to your device

1. Navigate through the menu of the display, mounted to your heat pump
2. Try to match the values (e.g. 237) to the values you see in the display (23.7°)
3. Use the existing templates to add new sensors
4. Add the newly added ids along with their types to property.h
5. Guard the newly added ids with e.g. #if defined(THZ_5_5_ECO) ... #endif to express that it was tested with that version once it is confirmed for the others, the guards can be removed
6. Add the entities to the respective yaml and use the already existing templates. For config values that can be set, use the wp_number.yaml. For sensors that are updated automatically chose the template that fits best.
7. profit

## Troubleshooting
### You are not able to control your heat pump
 --> Increase the log level and try to find out which CAN identifiers to use. Also check the forum and communciation.h

### You get garbage values for certain sensors
 --> There is no offical description of the IDs available. Everything is somewhat trial-and-error. 
     Also there are many differences between the different heat pumps. Play around with the display and try to find out
     the correct IDs.



Heavily inspired by this post in HA community forum:

https://community.home-assistant.io/t/configured-my-esphome-with-mcp2515-can-bus-for-stiebel-eltron-heating-pump/366053

and

https://github.com/bullitt186/ha-stiebel-control

special thanks to [@hovhannes85](https://github.com/hovhannes85)

### Useful links
https://www.stiebel-eltron.de/content/dam/ste/cdbassets/historic/bedienungs-_u_installationsanleitungen/ISG_Modbus__b89c1c53-6d34-4243-a630-b42cf0633361.pdf
