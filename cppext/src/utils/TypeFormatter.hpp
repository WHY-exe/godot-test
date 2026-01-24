#pragma once
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <spdlog/fmt/bundled/format.h>

template <>
class fmt::formatter<godot::String> {
public:
    constexpr auto parse(fmt::format_parse_context& context) {
        auto	   iter{ context.begin() };
        const auto end{ context.end() };
        if (iter != end && *iter != '}') {
            throw fmt::format_error{ "Invalid godot format specifier." };
        }

        return iter;
    }

    auto format(const godot::String& str, fmt::context& context) const {
        return fmt::format_to(context.out(), "{}", str.utf8().ptr());
    }
};

template <>
class fmt::formatter<godot::StringName> {
public:
    constexpr auto parse(fmt::format_parse_context& context) {
        auto	   iter{ context.begin() };
        const auto end{ context.end() };
        if (iter != end && *iter != '}') {
            throw fmt::format_error{ "Invalid godot format specifier." };
        }

        return iter;
    }

    auto format(const godot::StringName& str, fmt::context& context) const {
        return fmt::format_to(context.out(), "{}", godot::String(str));
    }
};