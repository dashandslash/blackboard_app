#pragma once
#include <stdint.h>

struct SDL_Window;

namespace blackboard::app {
  struct Window;
}

namespace blackboard::renderer {

enum class Api : uint8_t
{
  NONE = 0,
  METAL,
  D3D11,
  VULKAN,
  OPENGL,
  WEBGL,
  AUTO
};

bool init(app::Window &window, Api &, const uint16_t width, const uint16_t height);

}    // namespace blackboard::renderer
