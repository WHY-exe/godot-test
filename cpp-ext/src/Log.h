#pragma once
#include <filesystem>
#include "TypeFormatter.hpp"
#include <spdlog/spdlog.h>

namespace godot {
namespace log {
    using level = spdlog::level::level_enum;
    using path  = std::filesystem::path;

    struct Config {
        level       logLevel      = level::info;
        bool        enableConsole = true;
        bool        enableFile    = true;
        bool        enableRotate  = false;
        std::size_t maxSize       = 10 * 1024;
        std::size_t maxFiles      = 3;
        path        logPath       = "logs/default.log";
        std::string pattern       = "[%Y-%m-%d %H:%M:%S.%e][%L][%s:%#] %v";
    };
    // void Log(level logLevl, const char* szFunc, int nLine, std::string formatted);
    void Init(const Config& logConfig);
}  // namespace log
}  // namespace godot
