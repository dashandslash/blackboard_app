#include "renderer.h"

#include "platform/imgui_impl_sdl_bgfx.h"
#include "window.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <blackboard_app/logger.h>
#include <SDL3/SDL.h>

#include <iostream>
#include <utility>

namespace blackboard::renderer {

bool init(app::Window &window, Api &renderer_api, const uint16_t width, const uint16_t height)
{
  ImGui_Impl_sdl_bgfx_Init(window.imgui_view_id);

  auto window_handle{native_window_handle(ImGui::GetMainViewport(), window.window)};
  if(!window_handle)
  {
    logger::logger->error(SDL_GetError());
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
    case Api::VULKAN:
      bgfx_init.type = bgfx::RendererType::Vulkan;    // auto choose renderer
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
  #ifdef SDL_VIDEO_DRIVER_X11
    bgfx_init.platformData.ndt = SDL_GetProperty(SDL_GetWindowProperties(window.window), "SDL.window.x11.display", NULL);
    bgfx_init.platformData.nwh = (void *)window_handle;
  #endif
  bgfx::init(bgfx_init);

//  bgfx::setDebug(BGFX_DEBUG_TEXT);
//  bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);
//  bgfx::setViewRect(0, 0, 0, drawable_width, drawable_height);

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
