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

namespace blackboard::app {

App::App(const char *app_name, const renderer::Api renderer_api, const uint16_t width,
         const uint16_t height, const bool fullscreen)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::cout << "Error: " << SDL_GetError() << std::endl;
    }
    
    auto &window = m_windows.emplace_back();

    window.title = app_name;
    window.width = width;
    window.height = height;
    window.fullscreen = fullscreen;

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
    window.init_platform_window();

    gui::init();

    renderer::init(window, renderer_api, window.width, window.height);
    renderer::ImGui_Impl_sdl_bgfx_Init(window.imgui_view_id);

    ImGui_ImplSDL2_InitForMetal(window.window);
}

void App::run()
{
    auto &window = main_window();
    on_init();
    auto layout_ui = resources::path() / "imgui.ini";
    if (!std::filesystem::exists(layout_ui))
    {
        layout_ui = resources::path() / "assets/layouts/default_imgui.ini";
    }
    ImGui::LoadIniSettingsFromDisk(layout_ui.string().c_str());
    const auto [drawable_width, drawable_height] = window.get_size_in_pixels();
    on_resize(drawable_width, drawable_height);

    SDL_Event event;
    while (m_running)
    {
        while (window.window != nullptr && SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                m_running = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window.window))
                m_running = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                const auto width = event.window.data1;
                const auto height = event.window.data2;
                window.width = width;
                window.height = height;
                renderer::ImGui_Impl_sdl_bgfx_Resize(window.window);
                const auto [drawable_width, drawable_height] = window.get_size_in_pixels();
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
        renderer::ImGui_Impl_sdl_bgfx_Render(window.imgui_view_id, ImGui::GetDrawData(), 0x000000FF);

        if (const auto io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        bgfx::frame();
    }
}

App::~App()
{
    ImGui::SaveIniSettingsToDisk((resources::path() / "imgui.ini").string().c_str());

//    renderer::material_manager().shutdown();

    ImGui_ImplSDL2_Shutdown();
    renderer::ImGui_Impl_sdl_bgfx_Shutdown();

    ImGui::DestroyContext();
    bgfx::shutdown();

    SDL_DestroyWindow(main_window().window);
    SDL_Quit();
}

Window& App::main_window()
{
    assert(m_windows.size());
    return m_windows[0];
}
}    // namespace blackboard::app
