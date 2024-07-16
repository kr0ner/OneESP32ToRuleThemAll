#if !defined(ENUM_H)
#define ENUM_H

#include <cstdint>
#include <ostream>
#include <string_view>
#include <unordered_map>

// clang-format off
#define DECLARE_ENUM_WITH_TYPE(E, T, ...)                                                       \
    enum E : T                                                                                  \
    {                                                                                           \
        __VA_ARGS__                                                                             \
    };                                                                                          \
    static std::unordered_map<T, std::string_view> E##MapName(generateEnumMap<T>(#__VA_ARGS__));\
    inline std::ostream &operator<<(std::ostream &os, const E enumTmp)                          \
    {                                                                                           \
        os << static_cast<T>(enumTmp);                                                          \
        return os;                                                                              \
    }                                                                                           \
    inline std::string getName(E enumTmp) {                                                     \
        const auto name{std::string(E##MapName[static_cast<T>(enumTmp)])};                      \
        return name.empty() ? std::to_string(static_cast<T>(enumTmp)) : name;                   \
    }
// clang-format on

template <typename T>
std::unordered_map<T, std::string_view> generateEnumMap(std::string_view stringView) {
    std::unordered_map<T, std::string_view> retMap;

    std::size_t begin{0U};
    std::size_t end{stringView.size()};
    constexpr auto minSize{3U};  // a=b
    while (stringView.size() >= minSize) {
        // normalize stringView
        stringView.remove_prefix(stringView.find_first_not_of(" ,"));

        // create a subview for the element Name=value
        std::string_view element = stringView;
        const auto length = element.find(',') == element.npos ? 0U : stringView.size() - element.find(',');
        element.remove_suffix(length);

        // remove the element from the stringView
        stringView.remove_prefix(element.size());

        // create a subview for the name
        std::string_view name = element;
        name.remove_suffix(element.size() - element.find('='));
        name.remove_prefix(1U);  // remove the leading 'k'

        // create a subview for the value
        std::string_view value = element;
        value.remove_prefix(1U + element.find('='));

        retMap.insert({static_cast<T>(std::strtoul(value.data(), nullptr, 0)), name});
    }
    return retMap;
}

#endif
