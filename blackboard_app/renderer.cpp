#include "renderer.h"

#include "platform/imgui_impl_sdl_bgfx.h"
#include "window.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <blackboard_app/logger.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <SDL3/SDL.h>
#include <build_config/SDL_build_config.h>

#include <iostream>
#include <utility>

namespace blackboard::renderer {

bool init(app::Window &window, Api &renderer_api, const uint16_t width, const uint16_t height)
{
  void* window_handle{nullptr};
#if defined(__WIN32__) && !defined(__WINRT__)
  window_handle = SDL_GetProperty(SDL_GetWindowProperties(window.window), "SDL.window.win32.hwnd");
#elif defined(__APPLE__) && defined(SDL_VIDEO_DRIVER_COCOA)
  window_handle = SDL_GetProperty(SDL_GetWindowProperties(window.window), "SDL.window.cocoa.window", NULL);
#endif
  if(!window_handle)
  {
    blackboard::app::logger::logger->error(SDL_GetError());
    return false;
  }

  bgfx::Init bgfx_init;
  bgfx::renderFrame();    // single threaded mode
  switch (renderer_api)
  {
    case Api::METAL:
      bgfx_init.type = bgfx::RendererType::Metal;    // auto choose renderer
      break;
    case Api::D3D11:
      bgfx_init.type = bgfx::RendererType::Direct3D11;    // auto choose renderer
      break;
    case Api::WEBGL:
      bgfx_init.type = bgfx::RendererType::OpenGL;    // auto choose renderer
      break;
    default:
      bgfx_init.type = bgfx::RendererType::Count;    // auto choose renderer
      break;
  }
  const auto [drawable_width, drawable_height] = window.get_size_in_pixels();
  bgfx_init.resolution.width = drawable_width;
  bgfx_init.resolution.height = drawable_height;
  bgfx_init.resolution.numBackBuffers = 1;
  bgfx_init.resolution.reset = BGFX_RESET_VSYNC | BGFX_RESET_HIDPI | BGFX_RESET_MSAA_X4;
  bgfx_init.platformData.nwh = renderer::native_window_handle(ImGui::GetMainViewport(), window.window);
  auto videodriver = std::string(SDL_GetCurrentVideoDriver());
  #ifdef SDL_ENABLE_SYSWM_X11
    bgfx_init.platformData.ndt = wmi.info.x11.display;
    bgfx_init.platformData.nwh = (void *)(uintptr_t)wmi.info.x11.window;
  #endif
  bgfx::init(bgfx_init);

  bgfx::setDebug(BGFX_DEBUG_TEXT);
  bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
  bgfx::setViewRect(0, 0, 0, drawable_width, drawable_height);

  switch (bgfx::getRendererType())
  {
    case bgfx::RendererType::Direct3D11:
      renderer_api = Api::D3D11;
      break;
    case bgfx::RendererType::Metal:
      renderer_api = Api::METAL;
      break;
    case bgfx::RendererType::Vulkan:
      renderer_api = Api::VULKAN;
      break;
    case bgfx::RendererType::OpenGL:
      renderer_api = Api::OPENGL;
      break;
    default:
      renderer_api = Api::NONE;
      break;
  }

  return true;
}
}    // namespace blackboard::renderer
