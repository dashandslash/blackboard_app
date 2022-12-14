#include "window.h"

#include "logger.h"

#include <SDL/SDL.h>

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
  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                            (fullscreen ? (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS) : SDL_WINDOW_SHOWN) |
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

#ifdef _WIN32
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif  // _WIN32
}

std::pair<uint16_t, uint16_t> Window::get_size_in_pixels() const
{
  int w{0u}, h{0u};
  SDL_GL_GetDrawableSize(window, &w, &h);
  return {w, h};
}

float Window::get_ddpi() const
{
  float ddpi, hdpi, vdpi;
  SDL_GetDisplayDPI(SDL_GetWindowDisplayIndex(window), &ddpi, &hdpi, &vdpi);
  return ddpi;
}

std::pair<uint16_t, uint16_t> Window::get_position() const
{
  int x{0u}, y{0u};
  SDL_GetWindowPosition(window, &x, &y);
  return {x, y};
}

}  // namespace blackboard::app
