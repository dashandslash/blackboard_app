#include "app.h"

#include "gui.h"
#include "platform/imgui_impl_sdl_bgfx.h"
#include "renderer.h"
#include "resources.h"
#include "window.h"

#include <SDL/SDL.h>
#include <bgfx/bgfx.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/imgui_internal.h>
#include <imguizmo/imguizmo.h>

#include <iostream>

#ifdef __WIN32__
#include "Windows.h"
#endif

namespace blackboard::app {

App::App(const char *app_name, const renderer::Api renderer_api, const uint16_t width, const uint16_t height,
         const bool fullscreen)
    : main_window(*new Window())
{
#ifdef __WIN32__
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif
  if (renderer_api == renderer::Api::NONE)
    return;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
  {
    std::cout << "Error: " << SDL_GetError() << std::endl;
  }

  main_window.title = app_name;
  main_window.width = width;
  main_window.height = height;
  main_window.fullscreen = fullscreen;

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
  main_window.init_platform_window();

  gui::init();

  renderer::init(main_window, renderer_api, main_window.width, main_window.height);
  renderer::ImGui_Impl_sdl_bgfx_Init(main_window.imgui_view_id);

  ImGui_ImplSDL2_InitForMetal(main_window.window);
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
    const auto [drawable_width, drawable_height] = main_window.get_size_in_pixels();
    on_resize(drawable_width, drawable_height);

    SDL_Event event;
    while (running)
    {
      while (main_window.window != nullptr && SDL_PollEvent(&event))
      {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
          running = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(main_window.window))
          running = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
          const auto width = event.window.data1;
          const auto height = event.window.data2;
          main_window.width = width;
          main_window.height = height;
          renderer::ImGui_Impl_sdl_bgfx_Resize(main_window.window);
          const auto [drawable_width, drawable_height] = main_window.get_size_in_pixels();
          on_resize(drawable_width, drawable_height);
        }
      }

      renderer::ImGui_Impl_sdl_bgfx_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();
      ImGuizmo::BeginFrame();

      on_update();
      m_prev_time = std::chrono::steady_clock::now();

      ImGui::Render();
      renderer::ImGui_Impl_sdl_bgfx_Render(main_window.imgui_view_id, ImGui::GetDrawData(), 0x000000FF);

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
  if (blackboard::app::gui::isInit())
  {
    ImGui::SaveIniSettingsToDisk((resources::path() / "imgui.ini").string().c_str());

    ImGui_ImplSDL2_Shutdown();
    renderer::ImGui_Impl_sdl_bgfx_Shutdown();

    ImGui::DestroyContext();
    bgfx::shutdown();

    SDL_DestroyWindow(main_window.window);
    SDL_Quit();
  }
}

}  // namespace blackboard::app
