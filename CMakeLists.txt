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
        Source/EntitySystem/Vertex.h Source/EntitySystem/Vertex.cpp
        Source/EntitySystem/MeshGenerator.h Source/EntitySystem/MeshGenerator.cpp
        Source/OpenGL/Window/Window.cpp Source/OpenGL/Window/Window.h
        Source/OpenGL/ShaderProgram/Shader.cpp Source/OpenGL/ShaderProgram/Shader.h
        Source/MyExceptions.cpp Source/MyExceptions.h
        Source/OpenGL/OpenGLinitializer.cpp Source/OpenGL/OpenGLinitializer.h
        Source/EntitySystem/Mesh.h Source/EntitySystem/Mesh.cpp
        Source/OpenGL/SendingDataToGPU/ToGPUattribueSender.cpp Source/OpenGL/SendingDataToGPU/ToGPUattribueSender.h
        Source/OpenGL/RenderSystem/AbstractRenderer.cpp Source/OpenGL/RenderSystem/AbstractRenderer.h
        Source/OpenGL/SendingDataToGPU/ToGPUniformSender.cpp Source/OpenGL/SendingDataToGPU/ToGPUniformSender.h
        Source/OpenGL/SendingDataToGPU/UniformSendingInfo.cpp Source/OpenGL/SendingDataToGPU/UniformSendingInfo.h
        Source/OpenGL/RenderSystem/Forward/ForwardRenderer.cpp Source/OpenGL/RenderSystem/Forward/ForwardRenderer.h
        Source/SampleApps/SinWaveApp/SinWaveApp.cpp Source/SampleApps/SinWaveApp/SinWaveApp.h
        Source/OpenGL/OpenGLinitializer.cpp Source/OpenGL/OpenGLinitializer.h
        Source/OpenGL/RenderSystem/Forward/ForwardSinWaveRenderer.cpp Source/OpenGL/RenderSystem/Forward/ForwardSinWaveRenderer.h)

target_include_directories(GameEngine PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(GameEngine ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${BOOST_LIBRARIES} ${GLM_LIBRARIES} glfw)