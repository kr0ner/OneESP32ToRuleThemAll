#include <sml/sml_file.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include "esphome.h"

namespace {
struct ObisIdentifier {
    const std::uint8_t medium;
    const std::uint8_t channel;
    const std::uint8_t measured_parameter;
    const std::uint8_t measurement_type;
    const std::uint8_t tariff;
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

enum class State : std::uint8_t { kWaitingForStartSequence, kWaitingForEndSequence, kParseMessage };

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
    
    void publishMQTT() const {
        std::ostringstream payload;
        payload.precision(1);
        payload << std::fixed;
        payload << "{\"grid\":{\"power\":" << power << ",";
        payload << "\"L1\":{\"power\":" << power_l1 <<",\"voltage\":" << voltage_l1 << ",\"current\":" << current_l1 << "},";
        payload << "\"L2\":{\"power\":" << power_l2 <<",\"voltage\":" << voltage_l2 << ",\"current\":" << current_l2 << "},";
        payload << "\"L3\":{\"power\":" << power_l3 <<",\"voltage\":" << voltage_l3 << ",\"current\":" << current_l3 << "}}}";
        id(mqtt_client).publish("homeassistant/energy/grid", payload.str());
    }

    void publishHA() const {
        id(Instantaneous_Power).publish_state(power);
        id(Instantaneous_Power_L1).publish_state(power_l1);
        id(Instantaneous_Power_L2).publish_state(power_l2);
        id(Instantaneous_Power_L3).publish_state(power_l3);
        id(Instantaneous_Voltage_L1).publish_state(voltage_l1);
        id(Instantaneous_Voltage_L2).publish_state(voltage_l2);
        id(Instantaneous_Voltage_L3).publish_state(voltage_l3);
        id(Instantaneous_Current_L1).publish_state(current_l1);
        id(Instantaneous_Current_L2).publish_state(current_l2);
        id(Instantaneous_Current_L3).publish_state(current_l3);
        id(Frequency).publish_state(frequency);
        id(Total_incoming).publish_state(total_incoming);
        id(Total_Outgoing).publish_state(total_outgoing);
    }
};

class SMLReader : public Component, public UARTDevice {
    std::vector<std::uint8_t> buffer;
    State current_state{State::kWaitingForStartSequence};

    template <typename T, typename F>
    void waitForSequence(const T& sequence, F func) {
        while (available()) {
            if (buffer.size() == buffer.capacity()) {
                // start over
                ESP_LOGV("SMLReader", "clearing buffer");
                buffer.clear();
            }
            buffer.push_back(read());

            // wait until there are at least as many bytes as the sequence to start comparison
            if (buffer.size() >= sequence.size()) {
                if (std::equal(buffer.begin() + buffer.size() - sequence.size(), buffer.end(), sequence.begin())) {
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

    bool parseBoolean(const sml_list* const entry) const {
        if (entry->value->type == SML_TYPE_BOOLEAN) {
            ESP_LOGV("SMLReader", "value: %s", entry->value->data.boolean ? "true" : "false");
            return entry->value->data.boolean;
        }
        ESP_LOGV("SMLReader", "unknown type : %d", entry->value->type);
        return false;
    }

    std::string parseString(const sml_list* const entry) {
        std::string str;
        if (entry->value->type == SML_TYPE_OCTET_STRING) {
            char* value;
            sml_value_to_strhex(entry->value, &value, true);
            ESP_LOGV("SMLReader", "value: %s", value);
            str = std::string(value);
            free(value);
            return str;
        }
        ESP_LOGV("SMLReader", "unknown type : %d", entry->value->type);
        return str;
    }

    void parseMessage() {
        if (buffer.size() <= 24) {
            // reset
            ESP_LOGV("SMLReader", "clearing buffer");
            buffer.clear();
            current_state = State::kWaitingForStartSequence;
            return;
        }
        sml_file* file = sml_file_parse(buffer.data(), buffer.size() - end_sequence.size() - 3U);
        if (file == nullptr) {
            ESP_LOGV("SMLReader", "file is nullptr");
            return;
        }

        ESP_LOGV("SMLReader", "parsing messages %d", file->messages_len);
        Readings readings;
        for (std::uint8_t i{0U}; i < file->messages_len; ++i) {
            ESP_LOGV("SMLReader", "next message");
            sml_message* message = file->messages[i];
            if (message == nullptr) {
                ESP_LOGV("SMLReader", "message is nullptr");
                return;
            }

            if (*message->message_body->tag == SML_MESSAGE_GET_LIST_RESPONSE) {
                sml_list* entry;
                sml_get_list_response* body;
                body = (sml_get_list_response*)message->message_body->data;
                for (entry = body->val_list; entry != nullptr; entry = entry->next) {
                    if (!entry->value) {  // do not crash on null value
                        continue;
                    }

                    const ObisIdentifier obisIdentifier{entry->obj_name->str[0U], entry->obj_name->str[1U],
                                                        entry->obj_name->str[2U], entry->obj_name->str[3U],
                                                        entry->obj_name->str[4U]};
                    /*ESP_LOGD("SMLReader","Obis Identifier %d-%d:%d.%d.%d/0",
														obisIdentifier.medium, 
														obisIdentifier.channel, 
														obisIdentifier.measured_parameter, 
														obisIdentifier.measurement_type,
														obisIdentifier.tariff);*/

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
                readings.publishMQTT();
                readings.publishHA();
            }
            ESP_LOGV("SMLReader", "is not SML_MESSAGE_GET_LIST_RESPONSE %d", *message->message_body->tag);
        }

        ESP_LOGV("SMLReader", "starting over");
        sml_file_free(file);
        buffer.clear();
    }

   public:
    SMLReader(UARTComponent* parent) : UARTDevice(parent) {}

    void setup() override { buffer.reserve(4096); }

    void loop() override {
        switch (current_state) {
            case (State::kWaitingForStartSequence):
                waitForSequence(start_sequence, [this]() {
                    current_state = State::kWaitingForEndSequence;
                    buffer.clear();
                });
                break;
            case (State::kWaitingForEndSequence):
                waitForSequence(end_sequence, [this]() { current_state = State::kParseMessage; });
                break;
            case (State::kParseMessage):
                parseMessage();
                buffer.clear();
                current_state = State::kWaitingForStartSequence;
                break;
            default:
                break;
        }
    }
};