
cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
project(testas)
add_executable(testas main.cpp glad.c)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
# find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

target_link_libraries(testas ${OPENGL_LIBRARIES} glfw)
