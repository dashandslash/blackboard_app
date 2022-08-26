cmake_minimum_required(VERSION 3.21)

project(blackboard_app_template)

file(GLOB_RECURSE SOURCES ./**.cpp ./**.c)
file(GLOB_RECURSE HEADERS ./**.hpp ./**.h)

if(WIN32)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${HEADERS})
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          BUNDLE True
                          RESOURCE True
                          MACOSX_BUNDLE_GUI_IDENTIFIER ll.dashandslash.${PROJECT_NAME}
                          MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
                          MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
                          MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
)
endif()

target_link_libraries(${PROJECT_NAME}
                      PRIVATE
                      blackboard::app
)

if(WIN32)
target_include_directories(${PROJECT_NAME}
                           PRIVATE
                           ${bgfx_cmake_SOURCE_DIR}/bx/include/compat/msvc
)
endif()

target_compile_definitions(${PROJECT_NAME}
                           PRIVATE
                           "-D_CRT_SECURE_NO_WARNINGS"
                           "-D__STDC_FORMAT_MACROS"
)
target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_20)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES})
