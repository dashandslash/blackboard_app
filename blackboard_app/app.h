#pragma once
#include "renderer.h"

#include <chrono>
#include <functional>
#include <iostream>

struct Window;

namespace blackboard::app {

class App
{
public:
  App() = delete;
  App(const char *app_name, const renderer::Api renderer_api, const uint16_t width = 1280u,
      const uint16_t height = 720u, const bool fullscreen = false);
  ~App();
  void run();
  std::function<void()> on_init{[]() { std::cout << "init function not defined" << std::endl; }};
  std::function<void()> on_update{[]() { std::cout << "update function not defined" << std::endl; }};
  std::function<void(const uint16_t, const uint16_t)> on_resize{
    [](const uint16_t width, const uint16_t height) {
      std::cout << "window resize function not defined" << std::endl;
    }};

  static float delta_time()
  {
    using namespace std::chrono;
    return duration_cast<duration<float, std::milli>>(steady_clock::now() - m_prev_time).count();
  }

  static float elapsed_time()
  {
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_start_time).count();
  }

  bool running{true};
  Window &main_window;

protected:
  uint32_t m_update_rate{16};
  renderer::Api m_renderer_api{renderer::Api::AUTO};
  inline static std::chrono::time_point<std::chrono::steady_clock> m_start_time =
    std::chrono::steady_clock::now();
  inline static std::chrono::time_point<std::chrono::steady_clock> m_prev_time =
    std::chrono::steady_clock::now();
};

}    // namespace blackboard::app
