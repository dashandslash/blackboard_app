#pragma once

#include <filesystem>
#include <string>

namespace bgfx {
struct TextureHandle;
}
struct ImVec4;

namespace blackboard::app {

struct State;

namespace gui {

constexpr float STANDARD_DPI = 96.0f;

void init();

bool isInit();

void set_blackboard_theme();

void dockspace();

/// @brief Load a font inside the collection of gui fonts
void load_font(const std::filesystem::path &path, const float size, const float ddpi, const bool set_as_default = false,
               const int oversample_h = 4, const int oversample_v = 4, const float rasterizer_multiply = 1.25f);

// input format #aa1199ff
ImVec4 string_hex_to_rgba_float(const std::string &color);

// bgfx texture handle to imgui texture id
void *toId(bgfx::TextureHandle _handle, uint8_t _flags, uint8_t _mip);
}  // namespace gui
}  // namespace blackboard::app
