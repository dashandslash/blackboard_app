cmake_minimum_required(VERSION 3.21)

project(APP_TEMPLATE C CXX)

file(GLOB_RECURSE SOURCES ./**.cpp ./**.c)
file(GLOB_RECURSE HEADERS ./**.hpp ./**.h)
file(GLOB_RECURSE ASSETS ./assets/*)

if(WINDOWS)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
elseif(MACOS)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${HEADERS})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE True
        RESOURCE True
        MACOSX_BUNDLE_GUI_IDENTIFIER ll.dashandslash.${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
)
elseif(LINUX)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    blackboard::app
    blackboard::gfx
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${blackboard_app_SOURCE_DIR}
)

if(WINDOWS)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${bgfx_SOURCE_DIR}/bx/include/compat/msvc
)
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    "-D_CRT_SECURE_NO_WARNINGS"
    "-D__STDC_FORMAT_MACROS"
    "-DSDL_MAIN_HANDLED"
)

# Copy assets folder in the Resources folder of binaray 

add_dependencies(${PROJECT_NAME} "${PROJECT_NAME}_copy_assets")

if(MACOS)
    add_custom_target("${PROJECT_NAME}_copy_assets" ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources/assets"
)
else()
    add_custom_target("${PROJECT_NAME}_copy_assets" ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/assets
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources/assets"
)
endif()

copy_shaderc_binary("$<TARGET_FILE_DIR:${PROJECT_NAME}>")

# Create tree group to tidy the files inside IDEs

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES} ${ASSETS})
