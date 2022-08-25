cmake_minimum_required(VERSION 3.21)

include(FetchContent)
set(FETCHCONTENT_QUIET off)

set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.external)

# SDL
FetchContent_Declare(
			               sdl
                     GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
			               GIT_TAG release-2.0.20
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
FetchContent_Declare(
                     imgui
                     GIT_REPOSITORY https://github.com/ocornut/imgui.git
                     GIT_TAG docking
                     GIT_SHALLOW 1
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
                     imguizmo
                     GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo.git
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

file(GLOB imguizmo_SOURCES
     "${imguizmo_SOURCE_DIR}/*.h"
     "${imguizmo_SOURCE_DIR}/*.cpp"
)

file(GLOB imgui_SOURCES
     "${imgui_SOURCE_SYMLINK_DIR}/*.h"
     "${imgui_SOURCE_SYMLINK_DIR}/*.cpp"
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

include_directories(${FETCHCONTENT_BASE_DIR})

# glm
FetchContent_Declare(
                     glm
                     GIT_REPOSITORY https://github.com/g-truc/glm.git
                     GIT_TAG master
                     GIT_SHALLOW 1
)
FetchContent_MakeAvailable(glm)

# bgfx_cmake
FetchContent_Declare(
                     bgfx_cmake
                     GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake.git
                     GIT_TAG master
                     GIT_SHALLOW 1
)
FetchContent_GetProperties(bgfx_cmake)
if(NOT bgfx_cmake_POPULATED)
	FetchContent_Populate(bgfx_cmake)
	if(EMSCRIPTEN OR IOS)
    		set(BGFX_BUILD_TOOLS OFF CACHE INTERNAL "")
	else()
    		set(BGFX_BUILD_TOOLS ON CACHE INTERNAL "")
	endif()
	set( BGFX_BUILD_EXAMPLES  OFF CACHE INTERNAL "" )
	set( BGFX_CUSTOM_TARGETS  OFF CACHE INTERNAL "" )
	add_subdirectory(${bgfx_cmake_SOURCE_DIR} ${bgfx_cmake_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
