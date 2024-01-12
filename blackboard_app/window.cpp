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
  SDL_PropertiesID props = SDL_CreateProperties();
  SDL_SetStringProperty(props, "title", title.c_str());
  SDL_SetNumberProperty(props, "x", SDL_WINDOWPOS_CENTERED);
  SDL_SetNumberProperty(props, "y", SDL_WINDOWPOS_CENTERED);
  SDL_SetNumberProperty(props, "width", width);
  SDL_SetNumberProperty(props, "height", height);
  SDL_SetNumberProperty(props, SDL_PROPERTY_WINDOW_CREATE_VULKAN_BOOLEAN, SDL_TRUE);
  SDL_SetNumberProperty(props, "flags", SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
  window = SDL_CreateWindowWithProperties(props);
  SDL_DestroyProperties(props);
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
