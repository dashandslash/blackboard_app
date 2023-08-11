#pragma once
#include <SDL3/SDL.h>

#include <filesystem>
#include <string.h>

namespace blackboard::app::resources {

inline std::filesystem::path path()
{
#ifdef __APPLE__
  if (char *base_path = SDL_GetBasePath(); base_path)
  {
    return {base_path};
  }
#else
  if (char *base_path = SDL_GetBasePath(); base_path)
  {
    return std::filesystem::path(base_path) / "Resources";
  }
#endif
  return strdup("/");
}
}    // namespace blackboard::app::resources
