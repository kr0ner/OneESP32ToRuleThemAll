This project aims to provide all kinds of useful features for your Home Assistant instance using a single ESP32 board.

## Features:

  - SmartMeter readings via optical sensor [https://amzn.eu/d/1a28LBy]
  - WaterMeter readings via [https://amzn.eu/d/8Rx0UVJ]
  - Controlling a Tecalor THZ 504 (or other Tecalor | Stiebel Eltron heat pumps) via CAN  [https://amzn.eu/d/5nJJK3r]

## Prerequisites:

  - A working Home Assistant installation [https://www.home-assistant.io/]
  - ESPHome AddOn installed [https://www.home-assistant.io/integrations/esphome/]

## Add new readings from heat pump / adopt to your device

1. Change the log level in esp32-poe-technik.yaml to DEBUG or VERBOSE to see what exactly is sent on the CAN bus
2. Navigate through the menu of the display, mounted to your heat pump
4. Try to match the values (e.g. 237) to the values you see in the display (23.7°)
5. Use the existing templates to add new sensors
6. profit

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
