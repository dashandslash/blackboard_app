#include "program.h"

#include <blackboard_app/resources.h>
#include <blackboard_app/logger.h>

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace internal {

constexpr auto shader_bin_extension = ".bin";

#ifdef __APPLE__
constexpr auto shader_platform_flags = " --platform osx";
constexpr auto shader_fragment_program_flags = " -p metal";
constexpr auto shader_vertex_program_flags = " -p metal";
constexpr auto shaderc_binary = "tools/shaderc/shaderc";
#elif _WIN32
constexpr auto shader_platform_flags = " --platform windows";
constexpr auto shader_fragment_program_flags = " -p ps_5_0";
constexpr auto shader_vertex_program_flags = " -p vs_5_0";
constexpr auto shaderc_binary = "tools/shaderc/shaderc.exe";
#elif __linux__
constexpr auto shader_platform_flags = " --platform linux";
constexpr auto shader_fragment_program_flags = " -p spirv";
constexpr auto shader_vertex_program_flags = " -p spirv";
constexpr auto shaderc_binary = "tools/shaderc/shaderc";
#endif

#ifdef __APPLE__
constexpr auto ShaderPlatformFlags = " --platform osx";
#elif _WIN32
constexpr auto ShaderPlatformFlags = " --platform windows -p s_5_0";
#endif

bgfx::ShaderHandle load_program(const std::filesystem::path &file_path)
{
    if (std::filesystem::exists(file_path))
    {
        std::ifstream file(file_path, std::ios::ate | std::ios::binary);
        std::streampos file_size = file.tellg();
        const bgfx::Memory *mem = bgfx::alloc(static_cast<uint32_t>(file_size) + 1);
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char *>(mem->data), file_size);
        file.close();
        auto handle = bgfx::createShader(mem);
        if (isValid(handle))
        {
            bgfx::setName(handle, file_path.string().c_str());
        }
        return handle;
    }
    return {bgfx::kInvalidHandle};
}

int compile_program(const std::filesystem::path &file_path,
                  blackboard::gfx::Program::Type type)
{
    std::string cmd =
      blackboard::app::resources::path().append(shaderc_binary).string();    // shaderc binary
    cmd.append(" -f " + file_path.string());    // input file
    cmd.append(" -o " + file_path.string() + shader_bin_extension);    // output file
    cmd.append(
      " -i " +
      blackboard::app::resources::path().append("assets/shaders/common").string());    // include path
    cmd.append(blackboard::gfx::Program::TypeFlag[type]);    // shader type
    cmd.append(shader_platform_flags);    // platform flags
    switch (type)
    {
      case blackboard::gfx::Program::VERTEX:
            cmd.append(shader_vertex_program_flags);    // platform flags
            break;
        case blackboard::gfx::Program::FRAGMENT:
            cmd.append(shader_fragment_program_flags);    // platform flags
            break;
        default:
            assert("Program type not implemented");
            break;
    }
  cmd.append(" > " + blackboard::app::resources::path().append(shaderc_binary).parent_path().string() + "/temp_output.txt");
    return system(cmd.c_str());
}

void log_compilation_error(const std::filesystem::path &shader_file_path)
{
    std::ifstream temp_file(blackboard::app::resources::path().append(shaderc_binary).parent_path().string() + "/temp_output.txt", std::ios::binary | std::ios::in);
    if(temp_file.is_open())
    {
        std::ostringstream sstr;
        sstr << temp_file.rdbuf();
        blackboard::app::logger::logger->error("Error compiling shader:\n{} \n{}", shader_file_path.string(), sstr.str());
        temp_file.close();
    }
}

}    // namespace

namespace blackboard::gfx {

Program::~Program()
{
  if (bgfx::isValid(program_handle))
    bgfx::destroy(program_handle);
  if (bgfx::isValid(vertex_shader_handel))
    bgfx::destroy(vertex_shader_handel);
  if (bgfx::isValid(fragment_shader_handel))
    bgfx::destroy(fragment_shader_handel);
}

bool init(Program& prog, const std::filesystem::path &vsh_path, const std::filesystem::path &fsh_path)
{
    using namespace internal;
    const auto vert_error_code = compile_program(vsh_path, Program::Type::VERTEX);
    if (vert_error_code != 0)
    {
        log_compilation_error(vsh_path);
        return false;
    }
    const auto frag_error_code = compile_program(fsh_path, Program::Type::FRAGMENT);
    if (frag_error_code != 0)
    {
        log_compilation_error(fsh_path);
        return false;
    }

    const auto vsh = load_program(vsh_path.string() + shader_bin_extension);
    if (!bgfx::isValid(vsh))
    {
        app::logger::logger->error("Error loading program: {}", fsh_path.string() + shader_bin_extension);
        return false;
    }
    const auto fsh = load_program(fsh_path.string() + shader_bin_extension);
    if (!bgfx::isValid(fsh))
    {
        app::logger::logger->error("Error loading program: {}", fsh_path.string() + shader_bin_extension);
        return false;
    }

    const auto prog_handle = bgfx::createProgram(vsh, fsh, false);
    if(bgfx::isValid(prog_handle))
    {
      prog = {};
      prog.program_handle = prog_handle;
      prog.vertex_shader_handel = vsh;
      prog.fragment_shader_handel = fsh;
      
      return true;
    }

    return false;
}

}    // namespace blackboard::gfx
