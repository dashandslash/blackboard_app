#pragma once
#include <stdint.h>

struct SDL_Window;

namespace blackboard::app {

struct Window;

namespace renderer {

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

bool init(Window &window, Api &, const uint16_t width, const uint16_t height);

}    // namespace renderer
}    // namespace blackboard::app
