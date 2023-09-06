#include "window.h"

#include "gui.h"
#include "logger.h"

#include <SDL3/SDL.h>

#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif  // _WIN32

namespace blackboard::app {

Window::~Window()
{
  logger::logger->info("Window {} destroyed", title);
}

void Window::init_platform_window()
{
  window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_SetWindowFullscreen(window, static_cast<SDL_bool>(fullscreen));
}

std::pair<uint16_t, uint16_t> Window::get_size_in_pixels() const
{
  int w{0u}, h{0u};
  SDL_GetWindowSizeInPixels(window, &w, &h);
  return {w, h};
}

float Window::effective_display_resolution() const
{
  const auto pixel_density = SDL_GetWindowPixelDensity(window);
  const auto display_scale = SDL_GetWindowDisplayScale(window);
  return pixel_density * display_scale * gui::STANDARD_DPI;
}

std::pair<uint16_t, uint16_t> Window::get_position() const
{
  int x{0u}, y{0u};
  SDL_GetWindowPosition(window, &x, &y);
  return {x, y};
}

}  // namespace blackboard::app
