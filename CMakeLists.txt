cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

include(cmake/fetch_external_dependencies.cmake)

set(PROJECT_NAME blackboard_app C CXX OBJC)
project (${PROJECT_NAME})

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

add_subdirectory(blackboard_app)
