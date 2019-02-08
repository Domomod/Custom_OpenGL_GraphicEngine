cmake_minimum_required(VERSION 3.13)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  /usr/local/include)

add_subdirectory(gl3w)

add_executable(GameEngine main.cpp ${SOURCE} ${HEADERS} Window.cpp Window.h)

target_link_libraries(GameEngine ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} glfw)