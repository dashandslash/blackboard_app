#pragma once
#include <stdint.h>

struct SDL_Window;

namespace blackboard::app {

struct Window;

namespace renderer {

enum class Api : uint8_t
{
    none = 0,
    metal,
    d3d11,
    webgpu,
    count
};

bool init(Window &window, const Api, const uint16_t width, const uint16_t height);

}    // namespace renderer
}    // namespace blackboard::app
