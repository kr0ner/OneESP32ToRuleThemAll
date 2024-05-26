#include "esphome.h"
#include "property_map.h"

class CustomClimate : public Component, public Climate {
   public:
    CustomClimate(const float min_temp, const float max_temp, const float temp_step, std::set<ClimateMode> modes,
                  std::set<ClimatePreset> presets, const std::pair<const CanMember, const std::uint16_t> temperature)
        : temperature_(temperature) {
        traits_.set_supported_modes(std::move(modes));
        traits_.set_supported_presets(std::move(presets));
        traits_.set_supports_current_temperature(true);
        traits_.set_visual_min_temperature(min_temp);
        traits_.set_visual_max_temperature(max_temp);
        traits_.set_visual_temperature_step(temp_step);
    }

    void setup() override {}

    template <typename Sensor>
    void register_current_temperature_callback(Sensor* current_temperature_sensor) {
        if (current_temperature_sensor != nullptr) {
            current_temperature_sensor->add_on_state_callback([this](float temp) {
                this->current_temperature = temp;
                this->publish_state();
            });
        }
    }

    template <typename Sensor>
    void register_target_temperature_callback(Sensor* target_temperature_sensor) {
        if (target_temperature_sensor != nullptr) {
            target_temperature_sensor->add_on_state_callback([this](float temp) {
                this->target_temperature = temp;
                this->publish_state();
            });
        }
    }

    template <typename BinarySensor>
    void register_heating_callback(BinarySensor* heating_sensor) {
        if (heating_sensor != nullptr) {
            heating_sensor->add_on_state_callback([this](const bool value) {
                this->isHeating = value;
                this->update_mode();
            });
        }
    }

    template <typename BinarySensor>
    void register_cooling_callback(BinarySensor* cooling_sensor) {
        if (cooling_sensor != nullptr) {
            cooling_sensor->add_on_state_callback([this](const bool value) {
                this->isCooling = value;
                this->update_mode();
            });
        }
    }

    template <typename BinarySensor>
    void register_fan_callback(BinarySensor* fan_sensor) {
        if (fan_sensor != nullptr) {
            fan_sensor->add_on_state_callback([this](const bool value) {
                this->fanRunning = value;
                this->update_mode();
            });
        }
    }

    void update_mode() {
        if (isHeating) {
            this->mode = climate::CLIMATE_MODE_HEAT;
        } else if (isCooling) {
            this->mode = climate::CLIMATE_MODE_COOL;
        } else if (fanRunning) {
            this->mode = climate::CLIMATE_MODE_FAN_ONLY;
        } else {
            this->mode = climate::CLIMATE_MODE_AUTO;
        }
        this->publish_state();
    }

    void control(const ClimateCall& call) override {
        // only handle the temp adjustment, since there seems to be no way to explicitly set the
        // heat pump to cooling or heating, other than setting the temperature.
        if (call.get_target_temperature().has_value()) {
            // User requested target temperature change
            float temp = *call.get_target_temperature();
            this->target_temperature = temp;

            // Send target temp to climate
            sendData(temperature_.first, temperature_.second, static_cast<std::uint16_t>(temp * 10.0f));
        }
        // Publish updated state
        this->publish_state();
    }

    ClimateTraits traits() override { return traits_; }

   private:
    climate::ClimateTraits traits_{};
    const std::pair<const CanMember, const std::uint16_t> temperature_;
    bool isHeating{false};
    bool isCooling{false};
    bool fanRunning{false};
};

class HeatingDayNight : public CustomClimate {
   public:
    HeatingDayNight(const std::uint16_t temperature)
        : CustomClimate(10.0f, 25.0f, 0.1f,
                        {climate::CLIMATE_MODE_COOL, climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_AUTO,
                         climate::CLIMATE_MODE_FAN_ONLY, climate::CLIMATE_MODE_OFF},
                        {climate::CLIMATE_PRESET_NONE, climate::CLIMATE_PRESET_HOME, climate::CLIMATE_PRESET_AWAY},
                        std::make_pair(HK1, temperature)){};
};

class HeatingDay : public HeatingDayNight {
   public:
    template <typename Sensor, typename BinarySensor>
    HeatingDay(Sensor* current_temperature_sensor, Sensor* target_temperature_sensor, BinarySensor* heating_sensor,
               BinarySensor* cooling_sensor, BinarySensor* fan_sensor)
        : HeatingDayNight(propertyManager.getPropertyAddress("kRAUMSOLLTEMP_I")) {
        register_current_temperature_callback(current_temperature_sensor);
        register_target_temperature_callback(target_temperature_sensor);
        register_heating_callback(heating_sensor);
        register_cooling_callback(cooling_sensor);
        register_fan_callback(fan_sensor);
    }
};

class HeatingNight : public HeatingDayNight {
   public:
    template <typename Sensor, typename BinarySensor>
    HeatingNight(Sensor* current_temperature_sensor, Sensor* target_temperature_sensor, BinarySensor* heating_sensor,
                 BinarySensor* cooling_sensor, BinarySensor* fan_sensor)
        : HeatingDayNight(propertyManager.getPropertyAddress("kRAUMSOLLTEMP_NACHT")) {
        register_current_temperature_callback(current_temperature_sensor);
        register_target_temperature_callback(target_temperature_sensor);
        register_heating_callback(heating_sensor);
        register_cooling_callback(cooling_sensor);
        register_fan_callback(fan_sensor);
    }
};

class HotWater : public CustomClimate {
   public:
    template <typename Sensor, typename BinarySensor>
    HotWater(Sensor* current_temperature_sensor, Sensor* target_temperature_sensor, BinarySensor* heating_sensor)
        : CustomClimate(30.0f, 70.0f, 0.5f, {climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_AUTO},
                        {climate::CLIMATE_PRESET_COMFORT, climate::CLIMATE_PRESET_ECO, climate::CLIMATE_PRESET_AWAY},
                        std::make_pair(Kessel, propertyManager.getPropertyAddress("kEINSTELL_SPEICHERSOLLTEMP"))) {
        register_current_temperature_callback(current_temperature_sensor);
        register_target_temperature_callback(target_temperature_sensor);
        register_heating_callback(heating_sensor);
    };
};
