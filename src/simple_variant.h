#if !defined(SIMPLE_VARIANT_H)
#define SIMPLE_VARIANT_H

#include <cstdint>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>

template <typename T>
struct is_string
    : public std::disjunction<std::is_same<char*, std::decay_t<T>>, std::is_same<const char*, std::decay_t<T>>,
                              std::is_same<std::string, std::decay_t<T>>> {};

template <typename T>
inline constexpr bool is_string_v = is_string<T>::value;

template <typename T>
using is_bool = std::is_same<T, bool>;

template <typename T>
inline constexpr bool is_bool_v = is_bool<T>::value;

template <typename, typename = void>
struct normalized;

template <typename T>
struct normalized<T, std::enable_if_t<std::is_integral_v<T> && !is_bool_v<T>>> {
    // this is needed, because there are no "int" sensors, only float.
    using type = float;
};

template <typename T>
struct normalized<T, std::enable_if_t<std::is_floating_point_v<T>>> {
    // for sensors
    using type = float;
};

template <typename T>
struct normalized<T, std::enable_if_t<is_bool_v<T>>> {
    // for binary sensors
    using type = bool;
};

template <typename T>
struct normalized<T, std::enable_if_t<is_string_v<T>>> {
    // for text sensors
    using type = std::string;
};

template <typename T>
using normalized_t = typename normalized<T>::type;

struct SimpleVariant {
    SimpleVariant() : type_id(typeid(void)){};

    template <typename T>
    SimpleVariant(const T& value) : type_id(typeid(T)) {
        auto& t_element = std::get<normalized_t<T>>(t);
        t_element = value;
    }

    template <typename T>
    bool holds_alternative() const {
        return (typeid(T) == type_id);
    }

    template <typename T>
    const T get() const {
        return std::get<normalized_t<T>>(t);
    }

    template <typename T>
    operator T() const {
        return get<T>();
    }

   private:
    std::tuple<bool, float, std::string> t;
    const std::type_info& type_id;
};

#endif
