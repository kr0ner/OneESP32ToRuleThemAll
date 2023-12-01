#if !defined(SIMPLE_VARIANT_H)
#define SIMPLE_VARIANT_H

#include <tuple>
#include <cstdint>

struct SimpleVariant {
    SimpleVariant() : type_id(typeid(void)) {};

    template<typename T>
    SimpleVariant(const T& value) : type_id(typeid(T)) {
        auto& t_element = std::get<T>(t);
        t_element = value;
    }

    template<typename T>
    bool holds_alternative() const { return (typeid(T) == type_id); }

    template<typename T>
    T get() const { return std::get<T>(t); }

    
  private:
    std::tuple<bool,std::uint8_t,std::int16_t,std::uint16_t,float,double,std::string> t;
    const std::type_info& type_id;
};

#endif