#pragma once
#include <godot_cpp/variant/utility_functions.hpp>
#include <mutex>
#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>

namespace godot {
template <typename Mutex>
class LogSink : public spdlog::sinks::base_sink<Mutex> {
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {

        // log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
        // msg.payload (before v1.3.0: msg.raw) contains pre formatted log

        // If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

        auto gdStr = String::utf8(formatted.data(), formatted.size() - 2);
        switch (msg.level)
        {
        case spdlog::level::trace:
            UtilityFunctions::print_rich(gdStr);
            break;
        case spdlog::level::debug:
            UtilityFunctions::print_rich("[color=cyan]", gdStr, "[/color]");
            break;
        case spdlog::level::info:
            UtilityFunctions::print_rich("[color=green]", gdStr, "[/color]");
            break;
        case spdlog::level::warn:
            UtilityFunctions::push_warning(gdStr);
            break;
        case spdlog::level::err:
            UtilityFunctions::push_error(gdStr);
            break;
        default:
            UtilityFunctions::print(gdStr);
            break;
        }
    }

    void flush_() override {
    }
};

using LogSink_mt = LogSink<std::mutex>;
using LogSink_st = LogSink<spdlog::details::null_mutex>;

}  // namespace godot