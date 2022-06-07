project(SnakeGame)
cmake_minimum_required(VERSION 3.18.0)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(SnakeGame main.cpp)
add_definitions(-DUNICODE -D_UNICODE)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SnakeGame)