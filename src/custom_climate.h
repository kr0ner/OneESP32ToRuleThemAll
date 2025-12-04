#include "esphome.h"
#include "property.h"

class CustomClimate : public Component, public Climate {
   public:
    CustomClimate(esphome::climate::ClimateModeMask modes, esphome::climate::ClimatePresetMask presets,
                  const std::vector<std::pair<const CanMember, const Property>> targetTemperatureProperties)
        : targetTemperatureProperties_(targetTemperatureProperties) {
        traits_.set_supported_modes(modes);
        traits_.set_supported_presets(presets);
        traits_.add_feature_flags(climate::CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);
        update_mode();
    }

    void setup() override {}

    float get_setup_priority() const override { return esphome::setup_priority::LATE; }

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
            this->mode = climate::CLIMATE_MODE_OFF;
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
            for (const auto& targetTemperatureProperty : targetTemperatureProperties_) {
                sendData(targetTemperatureProperty.first, targetTemperatureProperty.second,
                         static_cast<std::uint16_t>(temp * 10.0f));
            }
        }
        // Publish updated state
        this->publish_state();
    }

    ClimateTraits traits() override { return traits_; }

   protected:
    climate::ClimateTraits traits_{};

   private:
    const std::vector<std::pair<const CanMember, const Property>> targetTemperatureProperties_;
    bool isHeating{false};
    bool isCooling{false};
    bool fanRunning{false};
};

class Heating : public CustomClimate {
   public:
    template <typename Sensor, typename BinarySensor>
    Heating(Sensor* current_temperature_sensor, Sensor* target_temperature_sensor, BinarySensor* heating_sensor,
            BinarySensor* cooling_sensor, BinarySensor* fan_sensor, const Property targetHeatingTemperature,
            const Property targetCoolingTemperature)
        : CustomClimate({climate::CLIMATE_MODE_COOL, climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_FAN_ONLY,
                         climate::CLIMATE_MODE_OFF},
                        {climate::CLIMATE_PRESET_NONE, climate::CLIMATE_PRESET_HOME, climate::CLIMATE_PRESET_AWAY},
                        {std::make_pair(HK1, targetHeatingTemperature), std::make_pair(HK1, targetCoolingTemperature),
                         std::make_pair(HK2, targetCoolingTemperature)}) {
        register_current_temperature_callback(current_temperature_sensor);
        register_target_temperature_callback(target_temperature_sensor);
        register_heating_callback(heating_sensor);
        register_cooling_callback(cooling_sensor);
        register_fan_callback(fan_sensor);
    };
};

class HotWater : public CustomClimate {
   public:
    template <typename Sensor, typename BinarySensor>
    HotWater(Sensor* current_temperature_sensor, Sensor* target_temperature_sensor, BinarySensor* heating_sensor,
             const Property targetTemperature)
        : CustomClimate({climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_OFF},
                        {climate::CLIMATE_PRESET_COMFORT, climate::CLIMATE_PRESET_ECO, climate::CLIMATE_PRESET_AWAY},
                        {std::make_pair(Kessel, targetTemperature)}) {
        register_current_temperature_callback(current_temperature_sensor);
        register_target_temperature_callback(target_temperature_sensor);
        register_heating_callback(heating_sensor);
    };
};
