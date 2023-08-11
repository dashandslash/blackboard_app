#include <blackboard_app/app.h>
#include <blackboard_app/gui.h>
#include <blackboard_app/resources.h>
#include <blackboard_app/window.h>

#include <imgui/imgui.h>

#include <filesystem>

blackboard::app::App *app_ptr;

void init()
{
  blackboard::app::gui::set_blackboard_theme();
  const auto dpi{app_ptr->main_window->effective_display_resolution()};
  blackboard::app::gui::load_font(blackboard::app::resources::path() / "assets/fonts/Inter/Inter-Light.otf", 12.0f,
                                  dpi);
}

void app_update()
{
  blackboard::app::gui::dockspace();
  ImGui::ShowDemoWindow();
}

int main(int argc, char *argv[])
{
  blackboard::app::App app("APP_TEMPLATE",
                           blackboard::app::renderer::Api::AUTO);  // autodetect renderer api
  app_ptr = &app;
  app.on_update = app_update;
  app.on_init = init;
  app.run();

  return 0;
}