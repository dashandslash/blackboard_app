cmake_minimum_required(VERSION 3.21)

project(blackboard_app_lib)

option(BIG_RENDERER_GL "Use GL" OFF)
cmake_dependent_option(BIG_RENDERER_VULKAN "Use Vulkan" ON
                       "BIG_PLATFORM_LINUX; NOT BIG_RENDERER_GL" OFF)

option(BIG_WM_WAYLAND "Use Wayland" OFF)
cmake_dependent_option(BIG_WM_X11 "Use X11" ON
                        "BIG_PLATFORM_LINUX; NOT BIG_WM_WAYLAND" OFF)
                       
if(${BIG_WM_X11})
    set(BIG_COMPILE_DEFS
        SDL_VIDEO_DRIVER_X11=1
    )
elseif(${BIG_WM_WAYLAND})
    set(BIG_COMPILE_DEFS
        SDL_VIDEO_DRIVER_WAYLAND=1
    )
endif()

file(GLOB_RECURSE SOURCES ./**.cpp ./**.c)
file(GLOB_RECURSE HEADERS ./**.hpp ./**.h)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(blackboard::app ALIAS ${PROJECT_NAME})
# set(CMAKE_CXX_FLAGS " \
# --bind \
# -s TOTAL_MEMORY=67108864 \
# -s USE_SDL=3 \
# -s ASSERTIONS=1 \
# -s DISABLE_EXCEPTION_CATCHING=0 \
# -s 'EXTRA_EXPORTED_RUNTIME_METHODS=[\"writeArrayToMemory\"]' \
# ")
target_link_libraries(${PROJECT_NAME}
    SDL3::SDL3
    bgfx
    bx
    glm
    ImGui
    spdlog
)

add_compile_definitions("__EMSCRIPTEN__")
target_link_options(${PROJECT_NAME} PRIVATE
    "-sMODULARIZE=1"
    "-sEXPORT_NAME='Main'"
    "-sINVOKE_RUN=0"
    "-sDISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0"
    "-sEXPORTED_FUNCTIONS=['_main']"
    "-sEXPORTED_RUNTIME_METHODS=['callMain', 'ccall', 'cwrap']"
    "-sFORCE_FILESYSTEM=1"
    "-sUSE_SDL=3"
    "-sALLOW_MEMORY_GROWTH=1" 
    "-sASYNCIFY"
    "-O2"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${FETCHCONTENT_BASE_DIR}
    ${bgfx_cmake_SOURCE_DIR}/bx/inlcude
    ${SDL3_SOURCE_DIR}/src
)

if(WINDOWS)
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${bgfx_cmake_SOURCE_DIR}/bx/include/compat/msvc
)
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    "-D_CRT_SECURE_NO_WARNINGS"
    "-D__STDC_FORMAT_MACROS"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SDL_VIDEO_DRIVER_X11=1
    NOMINMAX
)

# target_compile_definitions(${PROJECT_NAME} PUBLIC "BX_CONFIG_DEBUG=$<CONFIG:Debug>")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES})
