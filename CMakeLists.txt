cmake_minimum_required(VERSION 3.2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra")
project(opengl_assignment)
add_executable(testas main.cpp)
add_executable(lsystem lsystem_basic.cpp)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

target_link_libraries(testas ${OPENGL_LIBRARIES} glfw)
target_link_libraries(lsystem ${OPENGL_LIBRARIES} glfw)
