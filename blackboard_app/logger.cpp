#include "logger.h"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/msvc_sink.h>

#include <filesystem>
#include <chrono>

namespace blackboard::logger {

std::filesystem::path path()
{
#ifdef __APPLE__
  if (char *base_path = SDL_GetBasePath(); base_path)
  {
    return std::filesystem::path{base_path}.parent_path().parent_path() / "log";
  }
#else
  if (char *base_path = SDL_GetBasePath(); base_path)
  {
    return std::filesystem::path{base_path}.parent_path().parent_path() / "log";
  }
#endif
  return strdup("/");
}

void init()
{
  static constexpr size_t file_size{5u * 1024u * 1024u};
  static constexpr size_t rotating_files{5u};
  static const std::filesystem::path filename{path() / "blackboard.log"};

  auto console_sink_trace = std::make_shared<spdlog::sinks::stdout_sink_mt>();
  console_sink_trace->set_pattern("[%^%l%$] %v");

  auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename.string().c_str(), file_size , rotating_files, true);
  spdlog::sinks_init_list sink_list = { file_sink, console_sink_trace };

#ifdef _WIN32
  auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#endif

  logger = std::make_shared<spdlog::logger>("blackboard_log", sink_list.begin(), sink_list.end());
  logger::logger->set_level(spdlog::level::trace);
  spdlog::set_default_logger(logger);
  using namespace std::chrono_literals;
  spdlog::flush_every(1s);
}

void shutdown()
{
  spdlog::shutdown();
}

}    // namespace blackboard::app::log
