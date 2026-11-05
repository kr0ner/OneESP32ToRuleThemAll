#pragma once

#include "esphome.h"
#include "esphome/components/web_server_base/web_server_base.h"

namespace esphome {
namespace wp_web_exporter {

class WpWebExporter : public Component, public AsyncWebHandler {
   public:
    WpWebExporter(web_server_base::WebServerBase* base) : base_(base) {}

    void setup() override {
        if (this->base_ != nullptr) {
            this->base_->add_handler(this);
            ESP_LOGI("wp_web_exporter", "Registered export handler at /export/csv");
        } else {
            ESP_LOGE("wp_web_exporter", "No web_server_base provided!");
        }
    }

    bool canHandle(AsyncWebServerRequest* request) const override {
        std::array<char, AsyncWebServerRequest::URL_BUF_SIZE> url_buf{};
        const auto url = request->url_to(url_buf);
        return url == "/export/csv";
    }

    void handleRequest(AsyncWebServerRequest* request) override {
        std::string csv = buildCsvSnapshot();
        const std::string timestamp = filenameTimestamp();
        const std::string filename = timestamp.empty() ? "thz_export.csv" : "thz_export_" + timestamp + ".csv";
        std::string disposition = "attachment; filename=\"" + filename + "\"";

        AsyncWebServerResponse* response = request->beginResponse(200, "text/csv; charset=utf-8", csv);
        response->addHeader("Content-Disposition", disposition.c_str());
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    }

    bool isRequestHandlerTrivial() const override { return false; }

    float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

   private:
    web_server_base::WebServerBase* base_{nullptr};

    static std::string csvEscape(const std::string& input) {
        bool needs_escape = false;
        for (const char c : input) {
            if (c == '"' || c == ';' || c == '\n' || c == '\r') {
                needs_escape = true;
                break;
            }
        }
        if (!needs_escape) {
            return input;
        }

        std::string output;
        output.reserve(input.size() + 2);
        output += '"';
        for (const char c : input) {
            if (c == '"') {
                output += "\"\"";
            } else {
                output += c;
            }
        }
        output += '"';
        return output;
    }

    static std::string filenameTimestamp() {
        const time_t epoch = ::time(nullptr);
        auto now = ESPTime::from_epoch_local(epoch);
        if (!now.is_valid()) {
            return "";
        }

        // Colons are not allowed in many filesystem file names, use dashes.
        return now.strftime("%Y-%m-%dT%H-%M-%S");
    }

    static std::string stateToString(sensor::Sensor* sensor) {
        if (sensor->has_state()) {
            char buf[32];
            snprintf(buf, sizeof(buf), "%g", sensor->get_state());
            return std::string(buf);
        }
        return "";
    }

    static std::string stateToString(binary_sensor::BinarySensor* sensor) {
        if (sensor->has_state()) {
            return sensor->get_state() ? "true" : "false";
        }
        return "";
    }

    static std::string stateToString(text_sensor::TextSensor* sensor) {
        if (sensor->has_state()) {
            return sensor->get_state();
        }
        return "";
    }

    static std::string stateToString(number::Number* number) {
        if (number->has_state()) {
            char buf[32];
            snprintf(buf, sizeof(buf), "%g", number->state);
            return std::string(buf);
        }
        return "";
    }

    static std::string stateToString(switch_::Switch* sw) {
        if (sw->has_state()) {
            return sw->state ? "true" : "false";
        }
        return "";
    }

    static std::string stateToString(select::Select* select) {
        if (select->has_state()) {
            return std::string(select->current_option().str());
        }
        return "";
    }

    static std::string stateToString(climate::Climate* climate) {
        if (!climate->has_state()) {
            return "";
        }
        std::string result;
        result += "mode=" + std::to_string(static_cast<int>(climate->mode)) + ",";
        result += "action=" + std::to_string(static_cast<int>(climate->action)) + ",";
        result += "current=" + std::to_string(climate->current_temperature) + ",";
        result += "target=" + std::to_string(climate->target_temperature);
        return result;
    }

    static std::string unitOfMeasurement(EntityBase* entity) {
        const auto unit = entity->get_unit_of_measurement_ref();
        if (!unit.empty()) {
            return unit.str();
        }
        return "";
    }

    template <typename T>
    void appendRows(std::string& csv, const T& entities, const char* type_name) {
        for (auto* entity : entities) {
            if (entity == nullptr || entity->is_internal()) {
                continue;
            }
            const std::string name = entity->get_name();
            if (name.empty()) {
                continue;
            }

            csv += csvEscape(type_name);
            csv += ";";
            csv += csvEscape(name);
            csv += ";";
            csv += csvEscape(stateToString(entity));
            csv += ";";
            csv += csvEscape(unitOfMeasurement(entity));
            csv += ";";
            csv += entity->has_state() ? "true" : "false";
            csv += "\n";
        }
    }

    std::string buildCsvSnapshot() {
        std::string csv;
        csv.reserve(4096U);
        csv += "type;name;value;unit_of_measurement;has_state\n";

        appendRows(csv, App.get_sensors(), "sensor");
        appendRows(csv, App.get_binary_sensors(), "binary_sensor");
        appendRows(csv, App.get_text_sensors(), "text_sensor");
        appendRows(csv, App.get_numbers(), "number");
        appendRows(csv, App.get_switches(), "switch");
        appendRows(csv, App.get_selects(), "select");
        appendRows(csv, App.get_climates(), "climate");

        return csv;
    }
};

}  // namespace wp_web_exporter
}  // namespace esphome
