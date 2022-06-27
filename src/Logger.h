/**
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2022 R4nd0m6uy <r4nd0m6uy@r4nd0m6uy.ch>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#pragma once

#include <string_view>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace DearQOI::Logger {

static constexpr std::string_view LOGGER_NAME_MAIN = "main";

inline std::shared_ptr<spdlog::logger> GetLogger(const std::string& name) {
    auto logger = spdlog::get(name);

    if (!logger) {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        logger = std::make_shared<spdlog::logger>(name, console_sink);
        logger->set_level(spdlog::level::info);
        spdlog::register_logger(logger);
    }

    return logger;
}

inline std::shared_ptr<spdlog::logger> Main() {
    return GetLogger(LOGGER_NAME_MAIN.data());
}

inline void Init(spdlog::level::level_enum logLevel) {
    GetLogger(LOGGER_NAME_MAIN.data());
    spdlog::set_level(logLevel);
}

}