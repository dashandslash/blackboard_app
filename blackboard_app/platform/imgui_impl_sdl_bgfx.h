#pragma once
#include <bgfx/bgfx.h>
#include <imgui/imgui.h>

struct SDL_Window;

namespace blackboard::renderer {
void ImGui_Impl_sdl_bgfx_Init(int view);
void ImGui_Impl_sdl_bgfx_Shutdown();
void ImGui_Impl_sdl_bgfx_NewFrame();
void ImGui_Impl_sdl_bgfx_Resize(SDL_Window *);
void ImGui_Impl_sdl_bgfx_Render(const bgfx::ViewId viewId, ImDrawData *draw_data, uint32_t clearColor);

// Use if you want to reset your rendering device without losing ImGui state.
void ImGui_Impl_sdl_bgfx_InvalidateDeviceObjects();
bool ImGui_Impl_sdl_bgfx_CreateDeviceObjects();

unsigned long native_window_handle(ImGuiViewport* viewport, SDL_Window* window);
}    // namespace blackboard::renderer
