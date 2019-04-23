cmake_minimum_required(VERSION 3.13)
project(GameEngine)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Boost REQUIRED)
find_package(GLM REQUIRED)
find_package(assimp REQUIRED)
find_package(SOIL REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS} ${SOIL_INCLUDE_DIRS} OnChangeListenerTemplate /usr/local/include)

add_subdirectory(gl3w)

set(    SOURCES_AND_HEADERS
        Source/DataLayer/DataTypes/Assets/MeshGenerator.h Source/DataLayer/DataTypes/Assets/MeshGenerator.cpp
        Source/GraphicsLayer/Window/Window.cpp Source/GraphicsLayer/Window/Window.h
        Source/GraphicsLayer/ShaderProgram/Shader.cpp Source/GraphicsLayer/ShaderProgram/Shader.h
        Source/MyExceptions.cpp Source/MyExceptions.h
        Source/GraphicsLayer/OpenGLinitializer.cpp Source/GraphicsLayer/OpenGLinitializer.h
        Source/DataLayer/DataTypes/Assets/Mesh.h Source/DataLayer/DataTypes/Assets/Mesh.cpp
        Source/GraphicsLayer/OpenGLinitializer.cpp Source/GraphicsLayer/OpenGLinitializer.h
        Source/GraphicsLayer/RenderSystemV2/Buffers/AttributeBuffer.h
        Source/GraphicsLayer/RenderSystemV2/Metadata/AttributeMetadata.cpp Source/GraphicsLayer/RenderSystemV2/Metadata/AttributeMetadata.h
        Source/GraphicsLayer/RenderSystemV2/Buffers/ElementArrayBuffer.h
        Source/GraphicsLayer/RenderSystemV2/Buffers/Buffer.h
        Source/GraphicsLayer/RenderSystemV2/Buffers/VertexArrayObject.cpp
        Source/GraphicsLayer/RenderSystemV2/Buffers/VertexArrayObject.h
        Source/GraphicsLayer/RenderSystemV2/Buffers/AttributeBuffer.cpp
        Source/SampleApps/RenderV2Sample/Application.cpp
        Source/SampleApps/RenderV2Sample/Application.h
        Source/DataLayer/StorageManagment/ModelStorageManager.h
        Source/DataLayer/StorageManagment/ModelToModelSpaceMatricesMap.h
        Source/DataLayer/StorageManagment/ModelMatricesBuffer.h
        Source/DataLayer/StorageManagment/EntitySystem.h
        Source/DataLayer/DataTypes/GameLogic/Entity.cpp
        Source/DataLayer/DataTypes/GameLogic/Entity.h
        Source/DataLayer/StorageManagment/EntityStorageManager.h
        Source/DataLayer/StorageManagment/StorageManagerTemplate.h
        Source/DataLayer/StorageManagment/EntitySystem.cpp
        Source/DataLayer/DataTypes/GameLogic/EntityFactory.cpp
        Source/DataLayer/DataTypes/GameLogic/EntityFactory.h
        Source/DataLayer/DataTypes/Assets/ModelLoader.cpp Source/DataLayer/DataTypes/Assets/ModelLoader.h
        Source/DataLayer/DataTypes/Assets/MeshLoader.cpp Source/DataLayer/DataTypes/Assets/MeshLoader.h
        Source/DataLayer/DataTypes/Assets/SkeletonLoader.cpp Source/DataLayer/DataTypes/Assets/SkeletonLoader.h
        Source/DataLayer/DataTypes/Assets/Textures/Texture.cpp Source/DataLayer/DataTypes/Assets/Textures/Texture.h
        Source/DataLayer/DataTypes/Assets/Textures/TextureLoader.cpp Source/DataLayer/DataTypes/Assets/Textures/TextureLoader.h
        Source/DataLayer/DataTypes/Assets/AssimpConversion.cpp Source/DataLayer/DataTypes/Assets/AssimpConversion.h
        Source/GraphicsLayer/Window/InputSystem.cpp Source/GraphicsLayer/Window/InputSystem.h
        Source/GraphicsLayer/FreeCamera.cpp Source/GraphicsLayer/FreeCamera.h
        Source/GraphicsLayer/Window/InputMetadata.h Source/DataLayer/DataTypes/Assets/Skeleton.cpp
        Source/DataLayer/DataTypes/Assets/Skeleton.h Source/DataLayer/DataTypes/Assets/SkeletalAnimation.cpp
        Source/DataLayer/DataTypes/Assets/SkeletalAnimation.h Source/DataLayer/DataTypes/Assets/SkeletonConstants.h
        Source/DataLayer/DataTypes/Assets/Model.cpp Source/DataLayer/DataTypes/Assets/Model.h
        )

add_executable(RenderV2 Source/SampleApps/RenderV2Sample/main.cpp ${SOURCE} ${HEADERS} ${SOURCES_AND_HEADERS} Source/GraphicsLayer/RenderSystemV2/Metadata/UniformMetadata.cpp Source/GraphicsLayer/RenderSystemV2/Metadata/UniformMetadata.h Source/GraphicsLayer/RenderSystemV2/Buffers/UniformBuffer.cpp Source/GraphicsLayer/RenderSystemV2/Buffers/UniformBuffer.h)
target_include_directories(RenderV2 PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(RenderV2 ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${BOOST_LIBRARIES} ${GLM_LIBRARIES} ${ASSIMP_LIBRARIES} ${SOIL_LIBRARIES} glfw)

