#define SDL_MAIN_HANDLED

#include <blackboard_app/app.h>
#include <blackboard_app/gui.h>
#include <blackboard_app/resources.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imguizmo/ImGuizmo.h>

#include <filesystem>
#include <iostream>
#include <sstream>

static const std::string state_name{"default_state"};

void init()
{
  blackboard::app::gui::set_blackboard_theme();
  blackboard::app::gui::load_font(
    blackboard::app::resources::path() / "assets/fonts/Inter/Inter-Light.otf", 12.0f);
}

void app_update()
{
  blackboard::app::gui::dockspace();
  ImGui::ShowDemoWindow();
}

int main(int argc, char *argv[])
{
  static const std::string headless_arg{"headless"};
  if (argc > 1 && std::string(argv[1]) == headless_arg)
  {
    blackboard::app::App app(headless_arg.c_str(), blackboard::app::renderer::Api::NONE);
    app.run();
  }
  else
  {
    blackboard::app::App app("Example SDL",
                             blackboard::app::renderer::Api::AUTO);    // autodetect renderer api
    app.on_update = app_update;
    app.on_init = init;
    app.run();
  }

  return 0;
}
