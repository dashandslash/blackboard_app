#include "window.h"

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
  window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_SetWindowFullscreen(window, static_cast<SDL_bool>(fullscreen));

//#ifdef _WIN32
//  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
//#endif  // _WIN32
}

std::pair<uint16_t, uint16_t> Window::get_size_in_pixels() const
{
  int w{0u}, h{0u};
  SDL_GetWindowSizeInPixels(window, &w, &h);
  return {w, h};
}

float Window::get_ddpi() const
{
  // https://github.com/libsdl-org/SDL/blob/813c586edb9c3e83446f4cf6e801c8a62a3f9d17/docs/README-migration.md?plain=1#LL1092C122-L1092C122
  return SDL_GetDesktopDisplayMode(SDL_GetDisplayForWindow(window))->display_scale * 96.0f;
}

std::pair<uint16_t, uint16_t> Window::get_position() const
{
  int x{0u}, y{0u};
  SDL_GetWindowPosition(window, &x, &y);
  return {x, y};
}

}  // namespace blackboard::app
