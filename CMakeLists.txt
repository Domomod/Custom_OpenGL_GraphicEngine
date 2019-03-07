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

set(    SOURCES_AND_HEADERS
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
        Source/OpenGL/RenderSystem/Forward/ForwardAbstractRenderer.cpp Source/OpenGL/RenderSystem/Forward/ForwardAbstractRenderer.h
        Source/SampleApps/SinWaveApp/SinWaveApp.cpp Source/SampleApps/SinWaveApp/SinWaveApp.h
        Source/OpenGL/OpenGLinitializer.cpp Source/OpenGL/OpenGLinitializer.h
        Source/OpenGL/RenderSystem/Forward/ForwardSinWaveRenderer.cpp Source/OpenGL/RenderSystem/Forward/ForwardSinWaveRenderer.h
        Source/SampleApps/TesselationApp/TesselationApp.cpp Source/SampleApps/TesselationApp/TesselationApp.h
        Source/OpenGL/RenderSystem/Forward/ForwardTesselation.cpp Source/OpenGL/RenderSystem/Forward/ForwardTesselation.h
        Source/OpenGL/RenderSystemV2/Buffers/UniformBuffer.cpp Source/OpenGL/RenderSystemV2/Buffers/UniformBuffer.h
        Source/OpenGL/RenderSystemV2/ContentProvider/RenderContentProvider.h
        Source/OpenGL/RenderSystemV2/Renderer/Renderer.cpp Source/OpenGL/RenderSystemV2/Renderer/Renderer.h
        Source/OpenGL/RenderSystemV2/Buffers/AttributeBuffer.h
        Source/OpenGL/RenderSystemV2/Metadata/AttributeMetadata.cpp Source/OpenGL/RenderSystemV2/Metadata/AttributeMetadata.h
        Source/OpenGL/RenderSystemV2/Buffers/ElementArrayBuffer.h
        Source/OpenGL/RenderSystemV2/Buffers/Buffer.h
        Source/SampleApps/RenderV2Sample/RendererSystemV2Sample.cpp Source/SampleApps/RenderV2Sample/RendererSystemV2Sample.h
        Source/OpenGL/RenderSystemV2/ContentProvider/SingleTriangleProvider.h
        Source/OpenGL/RenderSystemV2/Buffers/VertexArrayObject.cpp Source/OpenGL/RenderSystemV2/Buffers/VertexArrayObject.h)

add_executable(SinWave Source/SampleApps/SinWaveApp/main.cpp ${SOURCE} ${HEADERS} ${SOURCES_AND_HEADERS})
target_include_directories(SinWave PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(SinWave ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${BOOST_LIBRARIES} ${GLM_LIBRARIES} glfw)

add_executable(Tesselation Source/SampleApps/TesselationApp/main.cpp ${SOURCE} ${HEADERS} ${SOURCES_AND_HEADERS})
target_include_directories(Tesselation PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(Tesselation ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${BOOST_LIBRARIES} ${GLM_LIBRARIES} glfw)

add_executable(RenderV2 Source/SampleApps/RenderV2Sample/main.cpp ${SOURCE} ${HEADERS} ${SOURCES_AND_HEADERS})
target_include_directories(RenderV2 PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(RenderV2 ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${BOOST_LIBRARIES} ${GLM_LIBRARIES} glfw)

