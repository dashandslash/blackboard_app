cmake_minimum_required(VERSION 3.21)

project(blackboard_app_lib)

file(GLOB_RECURSE SOURCES ./**.cpp ./**.c)
file(GLOB_RECURSE HEADERS ./**.hpp ./**.h)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(blackboard::app ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    SDL2-static
    bgfx
    bx
    glm
    ImGui
    spdlog
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${FETCHCONTENT_BASE_DIR}
    ${bgfx_cmake_SOURCE_DIR}/bx/inlcude
)

if(WIN32)
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

# target_compile_definitions(${PROJECT_NAME} PUBLIC "BX_CONFIG_DEBUG=$<CONFIG:Debug>")
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES})
