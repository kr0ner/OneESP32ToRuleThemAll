### This project aims to provide all kinds of useful features for your Home Assistant instance using a single ESP32 board.

If you like it, help me code all night 😴

<a href="https://www.buymeacoffee.com/kr0ner" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>

## Features:

* SmartMeter readings via optical sensor [https://amzn.to/4sbL7va]
* WaterMeter readings via [https://amzn.to/46uzGGs]
* Controlling a Tecalor THZ 504 (or other Tecalor | Stiebel Eltron heat pumps) via ESP32 + CAN using an MCP2515 [https://amzn.to/4rJ538H] or the Waveshare Industrial ESP32-S3 Control Board with RS485 and CAN Communication [https://amzn.to/4scTlDk]. If you want a more reliable connection, ESP32-S3 ETH development board [https://amzn.to/40qj5zU] might the right choice. There is also a PoE version of that [https://amzn.to/4sbZjnS]
* Dallas DS18B20 1-Wire temperature sensors for precise return flow measurement: [https://amzn.to/4u8ExHr]

## Community Statistics

<img width="2033" height="1044" alt="Screenshot from 2026-02-24 09-58-46" src="https://github.com/user-attachments/assets/698f1180-7b1c-4e2c-ab42-f3b1ec40256f" />

You are highly encouraged to contribute your telemetry data! By opting in and providing your district, you can compare your local heat pump metrics with the wider community. This grants you access to a global Grafana dashboard and allows you to subscribe to the community's minimum, maximum, and average values via an MQTT topic (updated every 15 minutes).

---

## Installation Guide

Choose the method that best fits your needs. The **Standard** approach is fastest for most users, while the **Advanced** approach is best if you plan to modify the code or contribute to the repository.

---

### Standard Installation (Recommended)
This is an easier, automated approach. The code is pulled directly from the repository, making it quick to set up. Note that this method is less flexible and you cannot easily contribute changes back to the source.

1.  **Open ESPHome:** Click **ESPHome** in your Home Assistant sidebar.
2.  **Create New Device:** Click **+ New Device**, give it a name, and select your board type.
3.  **Apply Configuration:** Open the editor for your new device. Copy the contents of [`esp32-standalone.yaml`](https://github.com/kr0ner/OneESP32ToRuleThemAll/blob/main/esp32-standalone.yaml) and paste them into your configuration, replacing the default text.
4.  **Install:** Click **Install** and choose your preferred flashing method (e.g., "Plug into this computer").

---

### Advanced Installation (Flexible)
Use this method if you want full control over the files, the ability to customize features locally, or if you wish to contribute to the project.

<details>
<summary><b>Step 1: Prepare Home Assistant</b></summary>

Before touching the hardware, ensure your software "brain" is ready.
1.  **Install Home Assistant:** If you haven't yet, follow the [Official Installation Guide](https://www.home-assistant.io/installation/).
2.  **Install Required Add-ons:**
    * Go to **Settings** > **Add-ons** > **Add-on Store**.
    * Install **ESPHome**.
    * Install **Studio Code Server** (VS Code).
4.  **Start & Show in Sidebar:** Make sure both are started and the "Show in sidebar" toggle is **ON**.

</details>

<details>
<summary><b>Step 2: Get the Code</b></summary>

We will use the terminal to ensure everything goes into the correct folders automatically.

1.  Open **Studio Code Server** from your Home Assistant sidebar.
2.  **Open a Terminal:** Press **`Ctrl+Shift+C`** or go to **Terminal -> New Terminal** in the top menu.
3.  **Navigate to the ESPHome folder:** Copy and paste the command below into the terminal and press Enter:
    ```bash
    cd /config/esphome
    ```
5.  **Clone the Repository:** Copy and paste the command below into the terminal and press Enter:
    ```bash
    git clone [https://github.com/kr0ner/OneESP32ToRuleThemAll.git](https://github.com/kr0ner/OneESP32ToRuleThemAll.git)
    ```
6.  **Create your local config:** Run this command to copy the "recipe" to your main ESPHome folder so the dashboard can "see" it:
    ```bash
    cp OneESP32ToRuleThemAll/esp32-poe-technik.yaml .
    ```
    *(Note: The dot `.` at the end is mandatory—it tells the computer to put the copy in your current folder.)*

</details>

<details>
<summary><b>Step 3: Configuration & Customization</b></summary>

1.  **WiFi & Secrets:** Ensure your `secrets.yaml` (located in the main `/config/esphome/` folder) contains your `wifi_ssid` and `wifi_password`.
2.  **Open your config:** In the VS Code file explorer (left side), click on the `esp32-poe-technik.yaml` file you just copied to the root folder.
3.  **Analytics Opt-in:** To opt-in to community stats, find the analytics block, uncomment it, and replace `"Landkreis"` with your actual district:
    ```yaml
    analytics: !include { file: OneESP32ToRuleThemAll/yaml/features/analytics.yaml, vars: { district: '"Landkreis"' }}
    ```
4.  **Entities:** Provide your Home Assistant entity IDs for temperature and humidity so the ESP32 can use them.
5.  **Adjust Board/Pins:** If you are using different hardware (not the default POE board), change the `board` type and SPI/CAN pins in this file now.

</details>

<details>
<summary><b>Step 4: Flashing & Pairing</b></summary>

1.  **Open ESPHome:** Click **ESPHome** in your sidebar. You should now see a card named `esp32-poe-technik`.
2.  **Install:** Click **Install** -> **Plug into this computer** (or use "Manual Download").
3.  **Pairing:** Once the flash is complete and the ESP32 reboots, Home Assistant should show a notification: **"New Device Discovered"**. Click **Configure** and follow the prompts.

</details>

---

## Maintenance & Updates

### How to Update
To get the latest features without breaking your setup, run these commands in your VS Code terminal:
```bash
cd /config/esphome/OneESP32ToRuleThemAll
git pull
```
After the pull finishes, go back to your **ESPHome Dashboard** and click **Install** on your device to push the new code.

### ⚠️ Important: Avoid Merge Conflicts
**Do not modify files inside the `OneESP32ToRuleThemAll/` folder directly.** If you change code inside that specific folder, you will run into errors (Merge Conflicts) when trying to update later. 

**Always** make your personal changes in the `esp32-poe-technik.yaml` file located in your **root** esphome folder.

---

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

1.  Navigate through the menu of the display, mounted to your heat pump.
2.  Try to match the values (e.g. 237) to the values you see in the display (23.7°).
3.  Use the existing templates to add new sensors.
4.  Add the newly added ids along with their types to `property.h`.
5.  Guard the newly added ids with e.g. `#if defined(THZ_5_5_ECO) ... #endif` to express that it was tested with that version. Once it is confirmed for the others, the guards can be removed.
6.  Add the entities to the respective yaml and use the already existing templates. For config values that can be set, use `wp_number.yaml`. For sensors that are updated automatically chose the template that fits best.
7.  Profit.

## Troubleshooting

### You are not able to control your heat pump
--> Increase the log level and try to find out which CAN identifiers to use. Also check the forum and `communication.h`.

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
