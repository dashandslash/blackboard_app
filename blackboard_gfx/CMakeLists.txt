cmake_minimum_required(VERSION 3.21)

project(blackboard_app_gfx C CXX)

include(${bgfx_cmake_SOURCE_DIR}/cmake/tools/shaderc.cmake)

file(GLOB_RECURSE SOURCES ./**.cpp ./**.c)
file(GLOB_RECURSE HEADERS ./**.hpp ./**.h)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(blackboard::gfx ALIAS ${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} shaderc)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    blackboard::app
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${blackboard_app_SOURCE_DIR}
)

if(WIN32)
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${bgfx_cmake_SOURCE_DIR}/bx/include/compat/msvc
)
endif()

function(get_shaderc_binary_path output_path)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${blackboard_app_gfx_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaderc ${output_path}
    )
endfunction()

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES})
