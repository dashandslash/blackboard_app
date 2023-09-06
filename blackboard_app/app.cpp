#include "app.h"

#include "gui.h"
#include "logger.h"
#include "platform/imgui_impl_sdl_bgfx.h"
#include "renderer.h"
#include "resources.h"
#include "window.h"

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/imgui_internal.h>
#include <imguizmo/ImGuizmo.h>

#include <iostream>

namespace blackboard::app {

App::App(const char *app_name, const renderer::Api renderer_api, const uint16_t width, const uint16_t height,
         const bool fullscreen)
: main_window{std::make_unique<Window>()}, m_renderer_api{renderer_api}
, on_init{[]() { logger::logger->info("init function not defined"); }}
, on_update{[]() { logger::logger->info("update function not defined"); }}
, on_resize{[](const uint16_t width, const uint16_t height) {
  logger::logger->info("window resize function not defined");}}
{
  if (renderer_api == renderer::Api::NONE)
    return;

  logger::init();
  logger::logger->info("App constructor");

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) != 0)
  {
    logger::logger->error(SDL_GetError());
  }

  main_window->title = app_name;
  main_window->width = width;
  main_window->height = height;
  main_window->fullscreen = fullscreen;

  main_window->init_platform_window();

  gui::init();

  renderer::init(*main_window, m_renderer_api, main_window->width, main_window->height);
  renderer::ImGui_Impl_sdl_bgfx_Init(main_window->imgui_view_id);

  switch (m_renderer_api)
  {
    case renderer::Api::METAL:
    {
      ImGui_ImplSDL3_InitForMetal(main_window->window);
      SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
    }
    break;
    case renderer::Api::D3D11:
    {
      ImGui_ImplSDL3_InitForD3D(main_window->window);
      SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");
    }
    break;
    case renderer::Api::VULKAN:
    {
      ImGui_ImplSDL3_InitForVulkan(main_window->window);
    }
    break;
    case renderer::Api::OPENGL:
    {
      ImGui_ImplSDL3_InitForVulkan(main_window->window);
      SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    }
    break;
    case renderer::Api::NONE:
      logger::logger->info("Render context not initialized");
      break;
  }

  logger::logger->info("Ending App constructor");
}

void App::run()
{
  on_init();

  if (ImGui::GetCurrentContext())
  {
    auto layout_ui = resources::path() / "imgui.ini";
    if (!std::filesystem::exists(layout_ui))
    {
      layout_ui = resources::path() / "assets/layouts/default_imgui.ini";
    }
    ImGui::LoadIniSettingsFromDisk(layout_ui.string().c_str());
    const auto [drawable_width, drawable_height] = main_window->get_size_in_pixels();
    on_resize(drawable_width, drawable_height);

    SDL_Event event;
    while (running)
    {
      while (main_window->window != nullptr && SDL_PollEvent(&event))
      {
        ImGui_ImplSDL3_ProcessEvent(&event);

        if (event.type == SDL_EVENT_QUIT)
          running = false;
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
            event.window.windowID == SDL_GetWindowID(main_window->window))
          running = false;
        if (event.type == SDL_EVENT_WINDOW_RESIZED)
        {
          const auto width = event.window.data1;
          const auto height = event.window.data2;
          main_window->width = width;
          main_window->height = height;
          renderer::ImGui_Impl_sdl_bgfx_Resize(main_window->window);
          const auto [drawable_width, drawable_height] = main_window->get_size_in_pixels();
          on_resize(drawable_width, drawable_height);
        }
      }

      renderer::ImGui_Impl_sdl_bgfx_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGui::NewFrame();
      ImGuizmo::BeginFrame();

      on_update();
      m_prev_time = std::chrono::steady_clock::now();

      ImGui::Render();
      renderer::ImGui_Impl_sdl_bgfx_Render(main_window->imgui_view_id, ImGui::GetDrawData(), 0x000000FF);

      if (const auto io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
      {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
      }

      bgfx::frame();
    }
  }
  else
  {
    while (running)
    {
      on_update();
      m_prev_time = std::chrono::steady_clock::now();
    }
  }
}

App::~App()
{
  if (blackboard::gui::isInit())
  {
    ImGui::SaveIniSettingsToDisk((resources::path() / "imgui.ini").string().c_str());

    ImGui_ImplSDL3_Shutdown();
    renderer::ImGui_Impl_sdl_bgfx_Shutdown();

    ImGui::DestroyContext();
    bgfx::shutdown();

    SDL_DestroyWindow(main_window->window);
    SDL_Quit();
  }
  main_window.reset();
  logger::shutdown();
}

}  // namespace blackboard::app
