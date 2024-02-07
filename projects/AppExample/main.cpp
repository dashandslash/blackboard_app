#include <blackboard_app/app.h>
#include <blackboard_app/gui.h>
#include <blackboard_app/resources.h>
#include <blackboard_app/logger.h>
#include <blackboard_app/window.h>

#include <bgfx/bgfx/include/bgfx/bgfx.h>
#include <imgui/imgui.h>

#include <filesystem>
#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

blackboard::app::App *app_ptr;

void init()
{
  blackboard::gui::set_blackboard_theme();
  // const auto dpi{app_ptr->main_window->effective_display_resolution()};
  // blackboard::gui::load_font(blackboard::app::resources::path() / "assets/fonts/Inter/Inter-Light.otf", 12.0f,
  //                                 dpi);
}

void app_update()
{
  blackboard::gui::dockspace();
  ImGui::ShowDemoWindow();
}

int main(int argc, char *argv[])
{
  std::cout << "cout from main" << std::endl; 

  blackboard::app::App app("AppExample",
                           blackboard::renderer::Api::WEBGL);  // autodetect renderer api
  // app_ptr = &app;
  app.on_update = app_update;
  app.on_init = init;
  app.run();

  return 0;
}
