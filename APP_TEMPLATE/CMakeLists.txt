cmake_minimum_required(VERSION 3.21)

project(blackboard_app_template)

file(GLOB_RECURSE SOURCES ./**.cpp ./**.c)
file(GLOB_RECURSE HEADERS ./**.hpp ./**.h)

if(WIN32)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
#    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:\"mainCRTStartup\"" )
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${HEADERS})
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          BUNDLE True
                          RESOURCE true
                          MACOSX_BUNDLE_GUI_IDENTIFIER ll.dashandslash.${PROJECT_NAME}
                          MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
                          MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
                          MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
)
endif()

target_link_libraries(${PROJECT_NAME}
                      PRIVATE
                      blackboard::app
                      SDL2-static
                      bgfx
                      bx
                      glm
                      ImGui
)

target_include_directories(${PROJECT_NAME}
                           PUBLIC
                           ${CMAKE_SOURCE_DIR}
                           ${FETCHCONTENT_BASE_DIR}
                        #    ${imgui_SOURCE_SYMLINK_DIR}
                           ${imgui_cmake_SOURCE_DIR}
                           ${SDL_cmake_SOURCE_DIR}
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
# set_property(${PROJECT_NAME} TARGET CMAKE_CXX_STANDARD 20)
# set_property(${PROJECT_NAME} TARGET CMAKE_CXX_STANDARD_REQUIRED TRUE)
# target_compile_definitions(${PROJECT_NAME} PUBLIC "BX_CONFIG_DEBUG=$<CONFIG:Debug>")
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_20)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADERS} ${SOURCES})
