#include "esphome.h"
//#include "communication.h"
#include "property.h"

class GenericCustomClimate : public Component, public Climate {
  public:
    GenericCustomClimate(const float min_temp, const float max_temp, const float temp_step, std::set<ClimateMode> modes, std::set<ClimatePreset> presets, const Property mode, const std::pair<const CanMember, const Property> temperature) : mode_(mode), temperature_(temperature) {
        traits_.set_supported_modes(std::move(modes));
        traits_.set_supported_presets(std::move(presets));
        traits_.set_supports_current_temperature(true);
        traits_.set_visual_min_temperature(min_temp);
        traits_.set_visual_max_temperature(max_temp);
        traits_.set_visual_temperature_step(temp_step);
    }

    void setup() override {

    }

    void register_current_temperature_callback(sensor::Sensor *current_temperature_sensor) {
        if(current_temperature_sensor != nullptr){
          current_temperature_sensor->add_on_state_callback([this](float temp) { this->update_current_temperature(temp); });
        }
    }

    void register_target_temperature_callback(sensor::Sensor *target_temperature_sensor) {
        if(target_temperature_sensor != nullptr){
          target_temperature_sensor->add_on_state_callback([this](float temp) { this->update_target_temperature(temp); });
        }
    }

    void register_mode_callback(sensor::Sensor *mode_sensor) {
        if(mode_sensor != nullptr){
          mode_sensor->add_on_state_callback([this](float mode) { this->update_mode(mode); });
        }
    }

    void update_current_temperature(const float current_temperature) {
        this->current_temperature = current_temperature;
        this->publish_state();
    }

    void update_target_temperature(const float target_temperature) {
        this->target_temperature = target_temperature;
        this->publish_state();
    }


    void update_mode(const float /*mode*/) {
        this->publish_state();
    }

    void control(const ClimateCall &call) override {
        if (call.get_mode().has_value()) {
            // User requested mode change
            ClimateMode mode = *call.get_mode();

            if(mode_ != Property::kINDEX_NOT_FOUND) {
                this->mode = mode;
                //sendData(Kessel, mode_, mode);
            }
        }
        if (call.get_target_temperature().has_value()) {
            // User requested target temperature change
            float temp = *call.get_target_temperature();
            this->target_temperature = temp;
            
            // Send target temp to climate
            sendData(temperature_.first, temperature_.second, static_cast<std::uint16_t>(temp * 10.0f)) ;
        }
        // Publish updated state
        this->publish_state();
    }

    ClimateTraits traits() override {
        return traits_;
    }
  private:
    climate::ClimateTraits traits_{};
    const Property mode_;
    const std::pair<const CanMember, const Property> temperature_;
};

class HeatingDayNight : public GenericCustomClimate {
  public:
    HeatingDayNight(const Property temperature) : GenericCustomClimate(
        10.0f,
        25.0f,
        0.1f,
        {climate::CLIMATE_MODE_COOL, climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_AUTO},
        {climate::CLIMATE_PRESET_NONE, climate::CLIMATE_PRESET_HOME, climate::CLIMATE_PRESET_AWAY},
        Property::kINDEX_NOT_FOUND,
        std::make_pair(Heizmodul,temperature)
    ){};
};

class HeatingDay : public HeatingDayNight {
  public:
    HeatingDay(sensor::Sensor *current_temperature_sensor, sensor::Sensor *target_temperature_sensor, sensor::Sensor *current_mode_sensor = nullptr) : HeatingDayNight(Property::kRAUMSOLLTEMP_I) {
      register_current_temperature_callback(current_temperature_sensor);
      register_target_temperature_callback(target_temperature_sensor);
      register_mode_callback(current_mode_sensor);
    }
};

class HeatingNight : public HeatingDayNight {
  public:
    HeatingNight(sensor::Sensor *current_temperature_sensor, sensor::Sensor *target_temperature_sensor, sensor::Sensor *current_mode_sensor = nullptr) : HeatingDayNight(Property::kRAUMSOLLTEMP_NACHT) {
      register_current_temperature_callback(current_temperature_sensor);
      register_target_temperature_callback(target_temperature_sensor);
      register_mode_callback(current_mode_sensor);
    }
};

class HotWater : public GenericCustomClimate {
  public:
    HotWater(sensor::Sensor *current_temperature_sensor, sensor::Sensor *target_temperature_sensor, sensor::Sensor *current_mode_sensor = nullptr) : GenericCustomClimate(
        35.0f,
        70.0f,
        0.5f,
        {climate::CLIMATE_MODE_HEAT},
        {climate::CLIMATE_PRESET_COMFORT, climate::CLIMATE_PRESET_ECO, climate::CLIMATE_PRESET_AWAY},
        Property::kINDEX_NOT_FOUND,
        std::make_pair(Kessel,Property::kEINSTELL_SPEICHERSOLLTEMP)
    ){
      register_current_temperature_callback(current_temperature_sensor);
      register_target_temperature_callback(target_temperature_sensor);
      register_mode_callback(current_mode_sensor);
    };
};