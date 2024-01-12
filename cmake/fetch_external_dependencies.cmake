cmake_minimum_required(VERSION 3.21)

include(FetchContent)
set(FETCHCONTENT_QUIET off)

set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/_external)

include(cmake/fetch_external/SDL.cmake)
include(cmake/fetch_external/ImGui.cmake)
include(cmake/fetch_external/bgfx.cmake)
include(cmake/fetch_external/glm.cmake)
include(cmake/fetch_external/spdlog.cmake)