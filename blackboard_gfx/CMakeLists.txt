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
    Boost::process
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

function(copy_shaderc_binary output_path)
    if(APPLE)
        set(shaderc_output_path ${output_path}"/../Resources/tools/shaderc/shaderc")
        set(shaders_output_path ${output_path}"/../Resources/assets/shaders")
    else()
        set(shaderc_output_path ${output_path}"/Resources/tools/shaderc/shaderc")
        set(shaders_output_path ${output_path}"/Resources/assets/shaders")
    endif()

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${bgfx_cmake_SOURCE_DIR}/bgfx/src/bgfx_compute.sh ${shaders_output_path}/bgfx_compute.sh
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${bgfx_cmake_SOURCE_DIR}/bgfx/src/bgfx_shader.sh ${shaders_output_path}/bgfx_shader.sh
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${bgfx_cmake_SOURCE_DIR}/bgfx/examples/common/shaderlib.sh ${shaders_output_path}/common/shaderlib.sh
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${bgfx_cmake_SOURCE_DIR}/bgfx/examples/common/common.sh ${shaders_output_path}/common/common.sh
        
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${blackboard_app_gfx_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaderc ${shaderc_output_path}
    )
endfunction()

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES})
