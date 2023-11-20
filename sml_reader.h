#include "esphome.h"
#include <string>
#include <array>
#include <algorithm>
#include <vector>
#include <sml/sml_file.h>
#include <sstream>
#include <cmath>
#include <chrono>

namespace {
	struct ObisIdentifier {
		const std::uint8_t medium;
		const std::uint8_t channel;
		const std::uint8_t measured_parameter;
		const std::uint8_t measurement_type;
		const std::uint8_t tariff;
		bool operator==(const ObisIdentifier& other) const {
			return 	std::tie(medium, channel, measured_parameter, measurement_type, tariff) == 
					std::tie(other.medium, other.channel, other.measured_parameter, other.measurement_type, other.tariff);
		}
	};

	static constexpr ObisIdentifier total_incoming{1U,0U,1U,8U,0U};
	static constexpr ObisIdentifier total_outgoing{1U,0U,2U,8U,0U};
	static constexpr ObisIdentifier instantaneous_power{1U,0U,16U,7U,0U};
	static constexpr ObisIdentifier frequency{1U,0U,14U,7U,0U};
	static constexpr ObisIdentifier instantaneous_power_l1{1U,0U,36U,7U,0U};
	static constexpr ObisIdentifier instantaneous_power_l2{1U,0U,56U,7U,0U};
	static constexpr ObisIdentifier instantaneous_power_l3{1U,0U,76U,7U,0U};
	static constexpr ObisIdentifier instantaneous_voltage_l1{1U,0U,32U,7U,0U};
	static constexpr ObisIdentifier instantaneous_voltage_l2{1U,0U,52U,7U,0U};
	static constexpr ObisIdentifier instantaneous_voltage_l3{1U,0U,72U,7U,0U};
	static constexpr ObisIdentifier instantaneous_current_l1{1U,0U,31U,7U,0U};
	static constexpr ObisIdentifier instantaneous_current_l2{1U,0U,51U,7U,0U};
	static constexpr ObisIdentifier instantaneous_current_l3{1U,0U,71U,7U,0U};

	static constexpr std::array<std::uint8_t,8U> start_sequence{0x1B, 0x1B, 0x1B, 0x1B, 0x01, 0x01, 0x01, 0x01};
	static constexpr std::array<std::uint8_t,5U> end_sequence{0x1B, 0x1B, 0x1B, 0x1B, 0x1A};
}

enum class State : std::uint8_t {
	kWaitingForStartSequence,
	kWaitingForEndSequence,
	kParseMessage
};

class SMLReader : public Component, public UARTDevice {
	std::vector<std::uint8_t> buffer;
	State current_state{State::kWaitingForStartSequence};

	template<typename T, typename F>
	void waitForSequence(const T& sequence, F func) {
		while(available()) {
			if(buffer.size() == buffer.capacity()) {
				// start over
				//ESP_LOGD("SMLReader","clearing buffer");
				buffer.clear();
			}
			buffer.push_back(read());

			// wait until there are at least as many bytes as the sequence to start comparison
			if(buffer.size() >= sequence.size()) {
				if(std::equal(buffer.begin() + buffer.size() - sequence.size(), buffer.end(), sequence.begin())) {
					//ESP_LOGD("SMLReader","found sequence");
					func();
					return;
				}
			}
		}
	}

	float parseNumeric(const sml_list * const entry) const {
		if (((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_INTEGER) || ((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_UNSIGNED)) {
			double value = sml_value_to_double(entry->value);
			int scaler = (entry->scaler) ? *entry->scaler : 0;
			if(scaler != 0) {
				return (value * pow(10.0f,scaler));
			}
			return value;
		} 
		//ESP_LOGD("SMLReader","unknown type : %d", entry->value->type);
		return 0.0f;
	}

	bool parseBoolean(const sml_list * const entry) const {
		if (entry->value->type == SML_TYPE_BOOLEAN) {
			//ESP_LOGD("SMLReader","value: %s", entry->value->data.boolean ? "true" : "false");
			return entry->value->data.boolean;
		}
		//ESP_LOGD("SMLReader","unknown type : %d", entry->value->type);
		return false;
	}

	std::string parseString(const sml_list * const entry) {
		std::string str;
		if (entry->value->type == SML_TYPE_OCTET_STRING) {
			char *value;
			sml_value_to_strhex(entry->value, &value, true);
			//ESP_LOGD("SMLReader","value: %s", value);
			str = std::string(value);
			free(value);
			return str;
		}
		//ESP_LOGD("SMLReader","unknown type : %d", entry->value->type);
		return str;
	}

	void parseMessage() {
		if(buffer.size() <= 24) {
			// reset
			//ESP_LOGD("SMLReader","clearing buffer");
			buffer.clear();
			current_state = State::kWaitingForStartSequence;
			return;
		}
		sml_file *file = sml_file_parse(buffer.data(), buffer.size() - end_sequence.size() - 3U );
		if(file == nullptr) {
			//ESP_LOGD("SMLReader","file is nullptr");
			return;
		}

		//ESP_LOGD("SMLReader","parsing messages %d",file->messages_len);
		for (std::uint8_t i{0U}; i < file->messages_len; ++i) {
			//ESP_LOGD("SMLReader","next message");
			sml_message *message = file->messages[i];
			if(message == nullptr) {
				//ESP_LOGD("SMLReader","message is nullptr");
				return;
			}

			if (*message->message_body->tag == SML_MESSAGE_GET_LIST_RESPONSE)
			{
				sml_list *entry;
				sml_get_list_response *body;
				body = (sml_get_list_response *)message->message_body->data;
				for (entry = body->val_list; entry != nullptr; entry = entry->next)
				{
					if (!entry->value)
					{ // do not crash on null value
						continue;
					}

					const ObisIdentifier obisIdentifier{
						entry->obj_name->str[0U],
						entry->obj_name->str[1U],
						entry->obj_name->str[2U],
						entry->obj_name->str[3U],
						entry->obj_name->str[4U]
					};
					/*ESP_LOGD("SMLReader","Obis Identifier %d-%d:%d.%d.%d/0",
														obisIdentifier.medium, 
														obisIdentifier.channel, 
														obisIdentifier.measured_parameter, 
														obisIdentifier.measurement_type,
														obisIdentifier.tariff);*/

					const auto publish_state = [this](esphome::template_::TemplateSensor& id, const sml_list_entry* entry){ 
						const auto state = parseNumeric(entry);
						id.publish_state(state);
					};

					if(obisIdentifier == total_incoming) {
						publish_state(id(Total_incoming),entry);
					} else if(obisIdentifier == total_outgoing) {
						publish_state(id(Total_Outgoing),entry);
					} else if(obisIdentifier == instantaneous_power) {
						publish_state(id(Instantaneous_Power),entry);
					} else if(obisIdentifier == frequency) {
						publish_state(id(Frequency),entry);
					} else if(obisIdentifier == instantaneous_power_l1) {
						publish_state(id(Instantaneous_Power_L1),entry);
					} else if(obisIdentifier == instantaneous_power_l2) {
						publish_state(id(Instantaneous_Power_L2),entry);
					} else if(obisIdentifier == instantaneous_power_l3) {
						publish_state(id(Instantaneous_Power_L3),entry);
					} else if(obisIdentifier == instantaneous_voltage_l1) {
						publish_state(id(Instantaneous_Voltage_L1),entry);
					} else if(obisIdentifier == instantaneous_voltage_l2) {
						publish_state(id(Instantaneous_Voltage_L2),entry);
					} else if(obisIdentifier == instantaneous_voltage_l3) {
						publish_state(id(Instantaneous_Voltage_L3),entry);
					} else if(obisIdentifier == instantaneous_current_l1) {
						publish_state(id(Instantaneous_Current_L1),entry);
					} else if(obisIdentifier == instantaneous_current_l2) {
						publish_state(id(Instantaneous_Current_L2),entry);
					} else if(obisIdentifier == instantaneous_current_l3) {
						publish_state(id(Instantaneous_Current_L3),entry);
					}
				}
			}
			//ESP_LOGD("SMLReader","is not SML_MESSAGE_GET_LIST_RESPONSE %d", *message->message_body->tag);
		}

		//ESP_LOGD("SMLReader","starting over");
		sml_file_free(file);
		buffer.clear();
	}

  public:
	SMLReader(UARTComponent *parent) : UARTDevice(parent) {}

	void setup() override {
		buffer.reserve(4096);
	}

	void loop() override {
		static auto last_run = std::chrono::steady_clock::now();
		switch(current_state) {
			case(State::kWaitingForStartSequence):
				waitForSequence(start_sequence,[this](){ current_state = State::kWaitingForEndSequence; buffer.clear(); });
				break;
			case(State::kWaitingForEndSequence):
				waitForSequence(end_sequence,[this](){ current_state = State::kParseMessage; });
				break;
			case(State::kParseMessage):
				if(std::chrono::steady_clock::now() - last_run > std::chrono::seconds(14)) {
					parseMessage();
					last_run = std::chrono::steady_clock::now();
				}
				buffer.clear();
				current_state = State::kWaitingForStartSequence;
				break;
			default:
				break;
		}
	}
};