#include "gui.h"

#include <bgfx/bgfx.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace blackboard::app::gui {

void init()
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // Set up input output configs
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
  io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

  ImGui::StyleColorsDark();

  // Setup friendly style for multiviewport
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    style.FrameBorderSize = 0.f;
    style.FramePadding = ImVec2(0.f, 0.f);
  }
}

bool isInit()
{
  return ImGui::GetCurrentContext();
}

void set_blackboard_theme()
{
  if (!isInit())
    return;

  ImGui::StyleColorsDark();

  static ImVec4 background{string_hex_to_rgba_float("#282a36ff")};
  static auto selection{string_hex_to_rgba_float("#44475aff")};
  static auto foreground{string_hex_to_rgba_float("#f8f8f2ff")};
  static auto comment{string_hex_to_rgba_float("#6272a4ff")};
  static auto cyan{string_hex_to_rgba_float("#8be9fdff")};
  static auto green{string_hex_to_rgba_float("#50fa7bff")};
  static auto orange{string_hex_to_rgba_float("#ffb86cff")};
  static auto pink{string_hex_to_rgba_float("#ff79c6ff")};
  static auto purple{string_hex_to_rgba_float("#bd93f9ff")};
  static auto red{string_hex_to_rgba_float("#ff5555ff")};
  static auto yellow{string_hex_to_rgba_float("#f1fa8cff")};

  const auto dark_alpha_selection{selection * ImVec4{1.0f, 1.0f, 1.0f, 0.5f}};
  const auto dark_alpha_purple{purple * ImVec4{1.0f, 1.0f, 1.0f, 0.3f}};
  const auto dark_background{background * ImVec4{0.35f, 0.35f, 0.35f, 1.0f}};
  const auto darker_background{dark_background * ImVec4{0.15f, 0.15f, 0.15f, 1.0f}};
  const auto dark_alpha_red{red * ImVec4{1.0f, 1.0f, 1.0f, 0.10f}};

  auto &colors{ImGui::GetStyle().Colors};

  const auto IconColour{ImVec4{0.718, 0.62f, 0.86f, 1.00f}};
  colors[ImGuiCol_Text] = foreground;
  colors[ImGuiCol_TextSelectedBg] = comment;
  colors[ImGuiCol_TextDisabled] = string_hex_to_rgba_float("#666666ff");

  colors[ImGuiCol_WindowBg] = dark_background;
  colors[ImGuiCol_ChildBg] = dark_background;

  colors[ImGuiCol_PopupBg] = dark_background;
  colors[ImGuiCol_Border] = dark_alpha_purple;
  colors[ImGuiCol_BorderShadow] = ImVec4{0.00f, 0.00f, 0.00f, 0.00f};
  colors[ImGuiCol_FrameBg] = selection;
  colors[ImGuiCol_FrameBgHovered] = selection * ImVec4{1.1f, 1.1f, 1.1f, 1.0f};
  colors[ImGuiCol_FrameBgActive] = selection * ImVec4{1.2f, 1.2f, 1.2f, 1.0f};

  colors[ImGuiCol_TitleBg] = (selection + dark_background) * ImVec4{0.5f, 0.5f, 0.5f, 1.0f};
  colors[ImGuiCol_TitleBgActive] = (selection + dark_background) * ImVec4{0.5f, 0.5f, 0.5f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed] = (selection + dark_background) * ImVec4{0.5f, 0.5f, 0.5f, 1.0f};
  colors[ImGuiCol_MenuBarBg] = selection;

  colors[ImGuiCol_ScrollbarBg] = ImVec4{0.02f, 0.02f, 0.02f, 0.39f};
  colors[ImGuiCol_ScrollbarGrab] = dark_alpha_selection;
  colors[ImGuiCol_ScrollbarGrabActive] = dark_alpha_selection * ImVec4{1.3f, 1.3f, 1.3f, 1.3f};
  colors[ImGuiCol_ScrollbarGrabHovered] = dark_alpha_selection * ImVec4{1.2f, 1.2f, 1.2f, 1.2f};

  colors[ImGuiCol_CheckMark] = comment;
  colors[ImGuiCol_SliderGrab] = comment;
  colors[ImGuiCol_SliderGrabActive] = comment * ImVec4{1.3f, 1.3f, 1.3f, 1.3f};
  colors[ImGuiCol_Button] = comment;
  colors[ImGuiCol_ButtonHovered] = comment * ImVec4{1.2f, 1.2f, 1.2f, 1.2f};
  colors[ImGuiCol_ButtonActive] = comment * ImVec4{1.3f, 1.3f, 1.3f, 1.3f};

  colors[ImGuiCol_Separator] = selection;
  colors[ImGuiCol_SeparatorHovered] = selection;
  colors[ImGuiCol_SeparatorActive] = selection;

  colors[ImGuiCol_ResizeGrip] = dark_alpha_purple;
  colors[ImGuiCol_ResizeGripHovered] = dark_alpha_purple * ImVec4{1.2f, 1.2f, 1.2f, 1.2f};
  colors[ImGuiCol_ResizeGripActive] = dark_alpha_purple * ImVec4{1.3f, 1.3f, 1.3f, 1.3f};

  colors[ImGuiCol_PlotLines] = yellow;
  colors[ImGuiCol_PlotLinesHovered] = yellow * ImVec4{1.2f, 1.2f, 1.2f, 1.2f};
  colors[ImGuiCol_PlotHistogram] = yellow;
  colors[ImGuiCol_PlotHistogramHovered] = yellow * ImVec4{1.2f, 1.2f, 1.2f, 1.2f};

  colors[ImGuiCol_DragDropTarget] = red;

  colors[ImGuiCol_NavHighlight] = red;
  colors[ImGuiCol_NavWindowingHighlight] = comment;
  colors[ImGuiCol_NavWindowingDimBg] = red;
  colors[ImGuiCol_ModalWindowDimBg] = dark_alpha_red;

  colors[ImGuiCol_Header] = dark_alpha_selection;
  colors[ImGuiCol_HeaderHovered] = dark_alpha_selection * ImVec4{1.2f, 1.2f, 1.2f, 1.2f};
  colors[ImGuiCol_HeaderActive] = dark_alpha_selection * ImVec4{1.3f, 1.3f, 1.3f, 1.3f};

  colors[ImGuiCol_Tab] = comment;
  colors[ImGuiCol_TabHovered] = comment * ImVec4{1.2f, 1.2f, 1.2f, 1.2f};
  colors[ImGuiCol_TabActive] = comment * ImVec4{1.3f, 1.3f, 1.3f, 1.3f};
  colors[ImGuiCol_TabUnfocused] = comment * ImVec4{0.5f, 0.5f, 0.5f, 0.5f};
  colors[ImGuiCol_TabUnfocusedActive] = comment * ImVec4{0.5f, 0.5f, 0.5f, 0.5f};

  colors[ImGuiCol_DockingEmptyBg] = darker_background;
  colors[ImGuiCol_DockingPreview] = dark_alpha_purple;

  colors[ImGuiCol_TableHeaderBg] = comment;
  colors[ImGuiCol_TableBorderLight] = dark_alpha_purple;
  colors[ImGuiCol_TableBorderStrong] = dark_alpha_purple;

  auto &style{ImGui::GetStyle()};
  style.FramePadding = {2.0f, 2.0f};
  style.CellPadding = {2.0f, 2.0f};
  style.TabBorderSize = 1.0f;
  style.TabRounding = 1.0f;
  style.ScrollbarRounding = 2.0f;
  style.GrabRounding = 2.0f;
  style.WindowRounding = 2.0f;
  style.ChildRounding = 2.0f;
  style.FrameRounding = 2.0f;
}

void load_font(const std::filesystem::path &path, const float size, const float ddpi, const bool set_as_default,
               const int oversample_h, const int oversample_v, const float rasterizer_multiply)
{
  if (!isInit())
    return;

  ImFontConfig font_config;
  font_config.RasterizerMultiply = rasterizer_multiply;
  font_config.OversampleH = oversample_h;
  font_config.OversampleV = oversample_v;
  auto &io{ImGui::GetIO()};
  if (!std::filesystem::exists(path))
  {
    return;
  }
  if (path.extension() != ".ttf" && path.extension() != ".otf")
  {
    return;
  }
  io.Fonts->AddFontFromFileTTF(path.string().c_str(), size * (ddpi / 96.f), &font_config);
  // setup default font
  if (set_as_default)
  {
    io.FontDefault = io.Fonts->Fonts.back();
  }
#ifdef __APPLE__
  io.FontGlobalScale = 1.0f / (ddpi / 96.f);
#endif
}

void dockspace()
{
  if (!isInit())
    return;

  const static ImGuiDockNodeFlags dockspace_flags{ImGuiDockNodeFlags_None};

  const static ImGuiWindowFlags window_flags{
    ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus};

  const ImGuiViewport *viewport{ImGui::GetMainViewport()};
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

  ImGui::Begin("DockSpace Demo", nullptr, window_flags);

  ImGui::PopStyleVar(3);

  // Submit the DockSpace
  ImGuiIO &io{ImGui::GetIO()};
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {
    ImGuiID dockspace_id{ImGui::GetID("main_dockspace")};
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }
  ImGui::End();
}

ImVec4 string_hex_to_rgba_float(const std::string &color)
{
  assert(color.size() == 9);
  return {std::stoul(color.substr(1, 2), nullptr, 16) / 255.0f, std::stoul(color.substr(3, 2), nullptr, 16) / 255.0f,
          std::stoul(color.substr(5, 2), nullptr, 16) / 255.0f, std::stoul(color.substr(7, 2), nullptr, 16) / 255.0f};
}

void *toId(bgfx::TextureHandle _handle, uint8_t _flags, uint8_t _mip)
{
  union
  {
    struct
    {
      bgfx::TextureHandle handle;
      uint8_t flags;
      uint8_t mip;
    } s;
    ImTextureID id;
  } tex;
  tex.s.handle = _handle;
  tex.s.flags = _flags;
  tex.s.mip = _mip;
  return tex.id;
}

}  // namespace blackboard::app::gui
