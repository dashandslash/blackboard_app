cmake_minimum_required(VERSION 3.21)

include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY "git@github.com:ocornut/imgui.git"
    GIT_TAG docking
    GIT_SHALLOW 1
    SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/imgui
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(imguizmo
    GIT_REPOSITORY "git@github.com:CedricGuillemet/ImGuizmo.git"
    GIT_TAG master
    GIT_SHALLOW 1
    SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/imguizmo
)
FetchContent_MakeAvailable(imguizmo)

file(GLOB imguizmo_SOURCES
    ${imguizmo_SOURCE_DIR}/*.h
    ${imguizmo_SOURCE_DIR}/*.cpp
)

set(imgui_SOURCES
   ${imgui_SOURCE_DIR}/imgui.cpp
   ${imgui_SOURCE_DIR}/imgui_demo.cpp
   ${imgui_SOURCE_DIR}/imgui_draw.cpp
   ${imgui_SOURCE_DIR}/imgui_tables.cpp
   ${imgui_SOURCE_DIR}/imgui_widgets.cpp
   ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
   ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    # ${imgui_color_text_edit_SOURCE_DIR}/TextEditor.cpp
)

set(imgui_HEADERS
    # ${CMAKE_CURRENT_SOURCE_DIR}/blackboard_core/gui/imconfig.h
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
    # ${imgui_color_text_edit_SOURCE_DIR}/TextEditor.h
)

add_library(ImGui ${imgui_HEADERS} ${imgui_SOURCES} ${imguizmo_SOURCES})

target_link_libraries(ImGui
    PUBLIC
    SDL3::SDL3
)

target_include_directories(ImGui
    PUBLIC
    $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
    # $<BUILD_INTERFACE:${FETCHCONTENT_BASE_DIR}/SDL/include>
)
