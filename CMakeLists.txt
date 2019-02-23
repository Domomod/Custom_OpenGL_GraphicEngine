cmake_minimum_required(VERSION 3.13)
project(GameEngine)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Boost REQUIRED)
find_package(GLM REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS}s OnChangeListenerTemplate /usr/local/include)

add_subdirectory(gl3w)

add_executable(GameEngine Source/main.cpp ${SOURCE} ${HEADERS}
        Source/SampleApps/Vertex.h Source/SampleApps/Vertex.cpp
        Source/SampleApps/MeshGenerator.h Source/SampleApps/MeshGenerator.cpp
        Source/View/Window.cpp Source/View/Window.h
        Source/View/Shader.cpp Source/View/Shader.h
        Source/MyExceptions.cpp Source/MyExceptions.h
        Source/SampleApps/Application.cpp Source/SampleApps/Application.h
        Source/SampleApps/Mesh.h Source/SampleApps/Mesh.cpp)

target_link_libraries(GameEngine ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${BOOST_LIBRARIES} ${GLM_LIBRARIES} glfw)