cmake_minimum_required(VERSION 3.12)
project(opengl)

set(CMAKE_CXX_STANDARD 14)

add_executable(opengl lab11.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_library(SOIL SOIL SOIL/SOIL.h)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(opengl ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SOIL} )