#pragma once
#include <bgfx/bgfx.h>

#include <filesystem>
#include <array>
#include <string>

namespace blackboard::gfx {

struct Program
{
  enum Type : uint8_t
  {
      VERTEX = 0,
      FRAGMENT,
      COMPUTE,
      Count
  };
  inline static const std::array<std::string, Program::Type::Count> TypeFlag{" --type v ", " --type f ", " --type c "};
  
  ~Program();

  bgfx::ShaderHandle vertex_shader_handel{bgfx::kInvalidHandle};
  bgfx::ShaderHandle fragment_shader_handel{bgfx::kInvalidHandle};
  bgfx::ShaderHandle compute_shader_handel{bgfx::kInvalidHandle};
  bgfx::ProgramHandle program_handle{bgfx::kInvalidHandle};
};

bool init(Program& program, const std::filesystem::path &vshPath, const std::filesystem::path &fshPath);

}    // namespace blackboard::core::renderer
