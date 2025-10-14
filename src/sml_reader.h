#pragma once

#include <sml/sml_file.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "esphome.h"
#include "esphome/components/network/util.h"
#include "mqtt_client.h"

namespace {
struct ObisIdentifier {
    const std::uint8_t medium{0U};
    const std::uint8_t channel{0U};
    const std::uint8_t measured_parameter{0U};
    const std::uint8_t measurement_type{0U};
    const std::uint8_t tariff{0U};
    bool operator==(const ObisIdentifier& other) const {
        return std::tie(medium, channel, measured_parameter, measurement_type, tariff) ==
               std::tie(other.medium, other.channel, other.measured_parameter, other.measurement_type, other.tariff);
    }
};

static constexpr ObisIdentifier total_incoming{1U, 0U, 1U, 8U, 0U};
static constexpr ObisIdentifier total_outgoing{1U, 0U, 2U, 8U, 0U};
static constexpr ObisIdentifier instantaneous_power{1U, 0U, 16U, 7U, 0U};
static constexpr ObisIdentifier frequency{1U, 0U, 14U, 7U, 0U};
static constexpr ObisIdentifier instantaneous_power_l1{1U, 0U, 36U, 7U, 0U};
static constexpr ObisIdentifier instantaneous_power_l2{1U, 0U, 56U, 7U, 0U};
static constexpr ObisIdentifier instantaneous_power_l3{1U, 0U, 76U, 7U, 0U};
static constexpr ObisIdentifier instantaneous_voltage_l1{1U, 0U, 32U, 7U, 0U};
static constexpr ObisIdentifier instantaneous_voltage_l2{1U, 0U, 52U, 7U, 0U};
static constexpr ObisIdentifier instantaneous_voltage_l3{1U, 0U, 72U, 7U, 0U};
static constexpr ObisIdentifier instantaneous_current_l1{1U, 0U, 31U, 7U, 0U};
static constexpr ObisIdentifier instantaneous_current_l2{1U, 0U, 51U, 7U, 0U};
static constexpr ObisIdentifier instantaneous_current_l3{1U, 0U, 71U, 7U, 0U};

static constexpr std::array<std::uint8_t, 8U> start_sequence{0x1B, 0x1B, 0x1B, 0x1B, 0x01, 0x01, 0x01, 0x01};
static constexpr std::array<std::uint8_t, 5U> end_sequence{0x1B, 0x1B, 0x1B, 0x1B, 0x1A};
}  // namespace

enum class State : std::uint8_t {
    kWaitingForStartSequence,
    kWaitingForEndSequence,
    kParseFile,
    kProcessMessages,
    kPublishReadings
};

struct Readings {
    float power{0.0f};
    float power_l1{0.0f};
    float power_l2{0.0f};
    float power_l3{0.0f};
    float voltage_l1{0.0f};
    float voltage_l2{0.0f};
    float voltage_l3{0.0f};
    float current_l1{0.0f};
    float current_l2{0.0f};
    float current_l3{0.0f};
    float total_incoming{0.0f};
    float total_outgoing{0.0f};
    float frequency{0.0f};
};

class SMLReader : public Component, public UARTDevice {
    std::vector<std::uint8_t> _buffer;
    State _current_state{State::kWaitingForStartSequence};
    sml_file* _file;
    std::optional<Readings> _readings;

    // Native MQTT Client & State
    esp_mqtt_client_handle_t _private_client{nullptr};
    bool _mqtt_connected{false};
    bool _mqtt_initialized{false};
    std::string _mqtt_uri;
    std::string _mqtt_user;
    std::string _mqtt_pass;

    static void mqtt_event_handler(void* handler_args, esp_event_base_t, int32_t event_id, void*) {
        SMLReader* instance = (SMLReader*)handler_args;

        switch ((esp_mqtt_event_id_t)event_id) {
            case MQTT_EVENT_CONNECTED:
                ESP_LOGI("SMLReader", "Native MQTT Connected!");
                instance->_mqtt_connected = true;
                break;
            case MQTT_EVENT_DISCONNECTED:
                ESP_LOGW("SMLReader", "Native MQTT Disconnected");
                instance->_mqtt_connected = false;
                break;
            case MQTT_EVENT_ERROR:
                ESP_LOGE("SMLReader", "Native MQTT Error");
                instance->_mqtt_connected = false;
                break;
            default:
                break;
        }
    }

    template <typename T, typename F>
    void waitForSequence(const T& sequence, F func) {
        while (available()) {
            if (_buffer.size() == _buffer.capacity()) {
                // start over
                ESP_LOGV("SMLReader", "clearing buffer");
                _buffer.clear();
            }
            _buffer.push_back(read());

            // wait until there are at least as many bytes as the sequence to start comparison
            if (_buffer.size() >= sequence.size()) {
                if (std::equal(_buffer.begin() + _buffer.size() - sequence.size(), _buffer.end(), sequence.begin())) {
                    ESP_LOGV("SMLReader", "found sequence");
                    func();
                    return;
                }
            }
        }
    }

    float parseNumeric(const sml_list* const entry) const {
        if (((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_INTEGER) ||
            ((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_UNSIGNED)) {
            double value = sml_value_to_double(entry->value);
            int scaler = (entry->scaler) ? *entry->scaler : 0;
            if (scaler != 0) {
                return (value * pow(10.0f, scaler));
            }
            return value;
        }
        ESP_LOGV("SMLReader", "unknown type : %d", entry->value->type);
        return 0.0f;
    }

    void parseFile() {
        ESP_LOGV("SMLReader", "parsing file");
        if (_buffer.size() <= 24U) {
            // reset
            ESP_LOGV("SMLReader", "clearing buffer");
            _buffer.clear();
            _current_state = State::kWaitingForStartSequence;
            return;
        }
        _file = sml_file_parse(_buffer.data(), _buffer.size() - end_sequence.size() - 3U);
        if (_file == nullptr) {
            ESP_LOGV("SMLReader", "file is nullptr");
            return;
        }
    }

    void processMessages() {
        ESP_LOGV("SMLReader", "processing messages %d", _file->messages_len);
        for (std::uint8_t i{0U}; i < _file->messages_len; ++i) {
            ESP_LOGV("SMLReader", "next message");
            sml_message* message = _file->messages[i];
            if (message == nullptr) {
                ESP_LOGV("SMLReader", "message is nullptr");
                return;
            }

            if (*message->message_body->tag == SML_MESSAGE_GET_LIST_RESPONSE) {
                sml_list* entry;
                sml_get_list_response* body;
                body = (sml_get_list_response*)message->message_body->data;
                Readings readings;
                for (entry = body->val_list; entry != nullptr; entry = entry->next) {
                    if (!entry->value) {  // do not crash on null value
                        continue;
                    }

                    const ObisIdentifier obisIdentifier{entry->obj_name->str[0U], entry->obj_name->str[1U],
                                                        entry->obj_name->str[2U], entry->obj_name->str[3U],
                                                        entry->obj_name->str[4U]};

                    if (obisIdentifier == total_incoming) {
                        readings.total_incoming = parseNumeric(entry);
                    } else if (obisIdentifier == total_outgoing) {
                        readings.total_outgoing = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_power) {
                        readings.power = parseNumeric(entry);
                    } else if (obisIdentifier == frequency) {
                        readings.frequency = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_power_l1) {
                        readings.power_l1 = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_power_l2) {
                        readings.power_l2 = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_power_l3) {
                        readings.power_l3 = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_voltage_l1) {
                        readings.voltage_l1 = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_voltage_l2) {
                        readings.voltage_l2 = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_voltage_l3) {
                        readings.voltage_l3 = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_current_l1) {
                        readings.current_l1 = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_current_l2) {
                        readings.current_l2 = parseNumeric(entry);
                    } else if (obisIdentifier == instantaneous_current_l3) {
                        readings.current_l3 = parseNumeric(entry);
                    }
                }
                _readings = readings;
            }
            ESP_LOGV("SMLReader", "is not SML_MESSAGE_GET_LIST_RESPONSE %d", *message->message_body->tag);
        }
        sml_file_free(_file);
        _buffer.clear();
    }

    void publishReadings() {
        if (_readings.has_value()) {
            publishMQTT(*_readings);
            publishHA(*_readings);
            _readings.reset();
        }
        ESP_LOGV("SMLReader", "starting over");
    }

    void publishMQTT(const Readings& readings) {
        if (!_mqtt_connected || _private_client == nullptr) {
            return;
        }

        char payload[512];
        snprintf(payload, sizeof(payload),
                 "{\"grid\":{\"power\":%.1f,"
                 "\"L1\":{\"power\":%.1f,\"voltage\":%.1f,\"current\":%.1f},"
                 "\"L2\":{\"power\":%.1f,\"voltage\":%.1f,\"current\":%.1f},"
                 "\"L3\":{\"power\":%.1f,\"voltage\":%.1f,\"current\":%.1f}}}",
                 readings.power, readings.power_l1, readings.voltage_l1, readings.current_l1, readings.power_l2,
                 readings.voltage_l2, readings.current_l2, readings.power_l3, readings.voltage_l3, readings.current_l3);

        esp_mqtt_client_publish(_private_client, "homeassistant/energy/grid", payload, 0, 1, 0);
    }

    void publishHA(const Readings& readings) const {
        id(Instantaneous_Power).publish_state(readings.power);
        id(Instantaneous_Power_L1).publish_state(readings.power_l1);
        id(Instantaneous_Power_L2).publish_state(readings.power_l2);
        id(Instantaneous_Power_L3).publish_state(readings.power_l3);
        id(Instantaneous_Voltage_L1).publish_state(readings.voltage_l1);
        id(Instantaneous_Voltage_L2).publish_state(readings.voltage_l2);
        id(Instantaneous_Voltage_L3).publish_state(readings.voltage_l3);
        id(Instantaneous_Current_L1).publish_state(readings.current_l1);
        id(Instantaneous_Current_L2).publish_state(readings.current_l2);
        id(Instantaneous_Current_L3).publish_state(readings.current_l3);
        id(Frequency).publish_state(readings.frequency);
        id(Total_Incoming).publish_state(readings.total_incoming);
        id(Total_Outgoing).publish_state(readings.total_outgoing);
    }

    void initMQTT() {
        if (_mqtt_initialized) {
            return;
        }

        ESP_LOGI("SMLReader", "Initializing MQTT...");

        esp_mqtt_client_config_t mqtt_cfg = {};
        mqtt_cfg.broker.address.uri = _mqtt_uri.c_str();
        mqtt_cfg.credentials.username = _mqtt_user.c_str();
        mqtt_cfg.credentials.authentication.password = _mqtt_pass.c_str();

        _private_client = esp_mqtt_client_init(&mqtt_cfg);

        if (_private_client) {
            esp_mqtt_client_register_event(_private_client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqtt_event_handler,
                                           this);
            esp_mqtt_client_start(_private_client);
            ESP_LOGI("SMLReader", "MQTT Client Started");
            _mqtt_initialized = true;
        } else {
            ESP_LOGE("SMLReader", "Failed to init MQTT Client");
        }
    }

   public:
    SMLReader(UARTComponent* parent, std::string server, std::string user, std::string password)
        : UARTDevice(parent), _mqtt_user(std::move(user)), _mqtt_pass(std::move(password)) {
        _mqtt_uri = "mqtt://" + server + ":1883";
    }

    void setup() override { _buffer.reserve(4096U); }

    void loop() override {
        if (!_mqtt_initialized) {
            if (esphome::network::is_connected()) {
                initMQTT();
            }
        }

        switch (_current_state) {
            case (State::kWaitingForStartSequence):
                waitForSequence(start_sequence, [this]() {
                    _current_state = State::kWaitingForEndSequence;
                    _buffer.clear();
                });
                break;
            case (State::kWaitingForEndSequence):
                waitForSequence(end_sequence, [this]() { _current_state = State::kParseFile; });
                break;
            case (State::kParseFile):
                parseFile();
                _buffer.clear();
                _current_state = State::kProcessMessages;
                break;
            case (State::kProcessMessages):
                processMessages();
                _current_state = State::kPublishReadings;
                break;
            case (State::kPublishReadings):
                publishReadings();
                _current_state = State::kWaitingForStartSequence;
                break;
            default:
                break;
        }
    }
};
