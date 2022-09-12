#pragma once
#include <spdlog/spdlog.h>

#include <filesystem>

namespace blackboard::app::logger {

inline std::shared_ptr<spdlog::logger> logger{nullptr};

std::filesystem::path path();

void init();

void shutdown();

}    // namespace blackboard::app::log
