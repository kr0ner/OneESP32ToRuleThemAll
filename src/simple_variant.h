#if !defined(SIMPLE_VARIANT_H)
#define SIMPLE_VARIANT_H

#include <cstdint>
#include <tuple>
#include <variant>
#include <typeinfo>
#include <esphome.h>

#include "type.h"

using esphome::esp_log_printf_;

struct SimpleVariant {
    SimpleVariant() : type_id(typeid(void)){};

    template <typename T>
    SimpleVariant(const T& value) : type_id(typeid(T)) {
        if constexpr (std::is_same_v<T, bool> || std::is_same_v<T, std::uint8_t> || std::is_same_v<T, std::int16_t> || std::is_same_v<T, std::uint16_t> || std::is_same_v<T, float> || std::is_same_v<T, std::string>) {
            auto& t_element = std::get<T>(t);
            t_element = value;
        } else {
          ESP_LOGE("ERROR", "Type not supported by Simple Variant");
    }
}


    template <typename T>
    bool holds_alternative() const {
        return (typeid(T) == type_id);
    }

    template <typename T>
    T get() const {
        return std::get<T>(t);
    }

    // Method to get the type of the stored value
    const std::type_info& type() const {
        return type_id;
    }

    // Method to get the type of the stored value (alternative name)
    const std::type_info& getType() const {
        return type();
    }

   private:
    std::tuple<bool, std::uint8_t, std::int16_t, std::uint16_t, float, std::string> t;
    const std::type_info& type_id;
};


#endif
