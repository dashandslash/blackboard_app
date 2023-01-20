cmake_minimum_required(VERSION 3.21)

include(FetchContent)
set(FETCHCONTENT_QUIET off)

set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/_external)

# SDL
if(EXISTS ${FETCHCONTENT_BASE_DIR}/sdl-src)
    set(repo_sdl "file://${FETCHCONTENT_BASE_DIR}/sdl-src")
    set(FETCHCONTENT_SOURCE_DIR_SDL ${FETCHCONTENT_BASE_DIR}/sdl-src)
else()
    set(repo_sdl "git@github.com:libsdl-org/SDL.git")
endif()

FetchContent_Declare(sdl
    GIT_REPOSITORY ${repo_sdl}
    GIT_TAG release-2.26.1
    GIT_SHALLOW 1
)
FetchContent_GetProperties(sdl)

if(NOT sdl_POPULATED)
    FetchContent_Populate(sdl)

    if(APPLE)
        set(SDL_VIDEO_METAL 1)
    endif()

	  add_subdirectory(${sdl_SOURCE_DIR} ${sdl_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# imgui
if(EXISTS ${FETCHCONTENT_BASE_DIR}/imgui-src)
    set(repo_imgui "file://${FETCHCONTENT_BASE_DIR}/imgui-src")
    set(FETCHCONTENT_SOURCE_DIR_IMGUI ${FETCHCONTENT_BASE_DIR}/imgui-src)
else()
    set(repo_imgui "git@github.com:ocornut/imgui.git")
endif()
FetchContent_Declare(imgui
    GIT_REPOSITORY ${repo_imgui}
    GIT_TAG docking
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(imgui)


if(EXISTS ${FETCHCONTENT_BASE_DIR}/imguizmo-src)
    set(repo_imguizmo "file://${FETCHCONTENT_BASE_DIR}/imguizmo-src")
    set(FETCHCONTENT_SOURCE_DIR_IMGUIZMO ${FETCHCONTENT_BASE_DIR}/imguizmo-src)
else()
    set(repo_imguizmo "git@github.com:CedricGuillemet/ImGuizmo.git")
endif()
FetchContent_Declare(imguizmo
    GIT_REPOSITORY ${repo_imguizmo}
    GIT_TAG master
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(imguizmo)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${SDL2_SOURCE_DIR}/include ${FETCHCONTENT_BASE_DIR}/SDL 
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${imgui_SOURCE_DIR} ${FETCHCONTENT_BASE_DIR}/imgui
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${imguizmo_SOURCE_DIR} ${FETCHCONTENT_BASE_DIR}/imguizmo
)

set(imgui_SOURCE_SYMLINK_DIR ${FETCHCONTENT_BASE_DIR}/imgui)
set(SDL_SOURCE_SYMLINK_DIR ${FETCHCONTENT_BASE_DIR}/SDL)
set(SDL_visualtest_INCLUDE ${SDL2_SOURCE_DIR}/visualtest/include)

file(GLOB imguizmo_SOURCES
    "${imguizmo_SOURCE_DIR}/*.h"
    "${imguizmo_SOURCE_DIR}/*.cpp"
)

set(imgui_SOURCES
    ${imgui_SOURCE_SYMLINK_DIR}/imgui.cpp
    ${imgui_SOURCE_SYMLINK_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_SYMLINK_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_SYMLINK_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_SYMLINK_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_SYMLINK_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_SYMLINK_DIR}/backends/imgui_impl_sdl.cpp
    # ${imgui_color_text_edit_SOURCE_DIR}/TextEditor.cpp
)

set(imgui_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/blackboard_core/gui/imconfig.h
    ${imgui_SOURCE_SYMLINK_DIR}/imgui.h
    ${imgui_SOURCE_SYMLINK_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_SYMLINK_DIR}/imstb_truetype.h
    ${imgui_SOURCE_SYMLINK_DIR}/imgui_internal.h
    ${imgui_SOURCE_SYMLINK_DIR}/imstb_textedit.h
    ${imgui_SOURCE_SYMLINK_DIR}/backends/imgui_impl_sdl.h
    ${imgui_SOURCE_SYMLINK_DIR}/misc/cpp/imgui_stdlib.h
    # ${imgui_color_text_edit_SOURCE_DIR}/TextEditor.h
)

add_library(ImGui STATIC ${imgui_SOURCES} ${imguizmo_SOURCES})

target_link_libraries(ImGui
    PUBLIC
    SDL2-static
)

target_include_directories(ImGui
    PUBLIC
    $<BUILD_INTERFACE:${imgui_SOURCE_SYMLINK_DIR}>
)

# spdlog
if(EXISTS ${FETCHCONTENT_BASE_DIR}/spdlog-src)
    set(repo_spdlog "file://${FETCHCONTENT_BASE_DIR}/spdlog-src")
    set(FETCHCONTENT_SOURCE_DIR_SPDLOG ${FETCHCONTENT_BASE_DIR}/spdlog-src)
else()
    set(repo_spdlog "git@github.com:gabime/spdlog.git")
endif()

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY ${repo_spdlog}
    GIT_TAG v1.10.0
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(spdlog)

# glm
if(EXISTS ${FETCHCONTENT_BASE_DIR}/glm-src)
    set(repo_glm "file://${FETCHCONTENT_BASE_DIR}/glm-src")
    set(FETCHCONTENT_SOURCE_DIR_GLM ${FETCHCONTENT_BASE_DIR}/glm-src)
else()
    set(repo_glm "git@github.com:g-truc/glm.git")
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY ${repo_glm}
    GIT_TAG master
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(glm)

# bgfx_cmake
if(EXISTS ${FETCHCONTENT_BASE_DIR}/bgfx_cmake-src)
    set(repo_bgfx_cmake "file://${FETCHCONTENT_BASE_DIR}/bgfx_cmake-src")
    set(FETCHCONTENT_SOURCE_DIR_BGFX_CMAKE ${FETCHCONTENT_BASE_DIR}/bgfx_cmake-src)
else()
    set(repo_bgfx_cmake "git@github.com:bkaradzic/bgfx.cmake.git")
endif()
FetchContent_Declare(
    bgfx_cmake
    GIT_REPOSITORY ${repo_bgfx_cmake}
    GIT_TAG v1.118.8398-377
    GIT_SHALLOW 1
)
FetchContent_GetProperties(bgfx_cmake)
if(NOT bgfx_cmake_POPULATED)
    FetchContent_Populate(bgfx_cmake)
    set( BGFX_BUILD_TOOLS OFF CACHE INTERNAL "")
    set( BGFX_BUILD_EXAMPLES  OFF CACHE INTERNAL "" )
    set( BGFX_CUSTOM_TARGETS  OFF CACHE INTERNAL "" )
    add_subdirectory(${bgfx_cmake_SOURCE_DIR} ${bgfx_cmake_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
