cmake_minimum_required(VERSION 3.13)
project(GameEngine)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)


find_package(glfw3 REQUIRED)
IF (NOT glfw3_FOUND)
    message(WARNING glfw3-NOT-FOUND : ${glfw3_INCLUDE_DIR})
ENDIF()

find_package(ASSIMP REQUIRED)
IF(NOT ASSIMP_FOUND)
    message(WARNING ASSIMP-NOT-FOUND : ${ASSIMP_INCLUDE_DIRS})
ENDIF()

find_package(Boost REQUIRED)
IF (NOT Boost_FOUND)
    message(WARNING Boost-NOT-FOUND : ${Boost_INCLUDE_DIR})
ENDIF()

find_package(GLM REQUIRED)
IF (NOT GLM_FOUND)
    message(WARNING GLM-NOT-FOUND : ${GLM_INCLUDE_DIR})
ENDIF()

find_package(OpenGL REQUIRED)
IF (NOT OPENGL_FOUND)
    message(WARNING OPENGL-NOT-FOUND: ${OPENGL_INCLUDE_DIR})
else()
    message(OPENGL-FOUND: ${OPENGL_INCLUDE_DIR})
    message(GLM-FOUND : ${GLM_INCLUDE_DIRS})
    message(glfw3-FOUND : ${glfw3_DIR})
    message(Boost-FOUND : ${Boost_INCLUDE_DIR})
    message(ASSIMP-FOUND : ${ASSIMP_INCLUDE_DIRS})
ENDIF()

include_directories( ${OPENGL_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS} ${SOIL_INCLUDE_DIRS} /usr/local/include Source/ OnChangeListenerTemplate/)

add_subdirectory(Dependencies)

set(    SOURCES_AND_HEADERS
        Dependencies/include/TinyXML2/tinyxml2.h Dependencies/include/TinyXML2/tinyxml2.cpp

        Source/MyExceptions.h                                                           Source/MyExceptions.cpp


        Source/Platform/OpenGL/Window/Window.h Source/Platform/OpenGL/Window/Window.cpp
        Source/Platform/OpenGL/Window/InputSystem.h Source/Platform/OpenGL/Window/InputSystem.cpp

        Source/Platform/OpenGL/ShaderProgram/Shader.h Source/Platform/OpenGL/ShaderProgram/Shader.cpp
        Source/Platform/OpenGL/OpenGLinitializer.h Source/Platform/OpenGL/OpenGLinitializer.cpp
        Source/Platform/OpenGL/OpenGLinitializer.h Source/Platform/OpenGL/OpenGLinitializer.cpp
        Source/GraphicsLayer/FreeCamera.h                                               Source/GraphicsLayer/FreeCamera.cpp

        Source/Platform/OpenGL/Buffers/Metadata/AttributeMetadata.h Source/Platform/OpenGL/Buffers/Metadata/AttributeMetadata.cpp
        Source/Platform/OpenGL/Buffers/ElementArrayBuffer.h
        Source/Platform/OpenGL/Buffers/Buffer.h
        Source/Platform/OpenGL/Buffers/VertexArrayObject.h
        Source/Platform/OpenGL/Buffers/AttributeBuffer.h Source/Platform/OpenGL/Buffers/AttributeBuffer.cpp
        Source/Platform/OpenGL/Buffers/Metadata/UniformMetadata.h Source/Platform/OpenGL/Buffers/Metadata/UniformMetadata.cpp
        Source/Platform/OpenGL/Buffers/UniformBuffer.h Source/Platform/OpenGL/Buffers/UniformBuffer.cpp


        Source/EngineLogic/ModelStorageManager.h
        Source/EngineLogic/ModelMatricesBuffer.h
        Source/EngineLogic/EntitySystem.h Source/EngineLogic/EntitySystem.cpp
        Source/EngineLogic/EntityStorageManager.h
        Source/EngineLogic/StorageManagerTemplate.h

        Source/EngineLogic/EntityFactory.h Source/EngineLogic/EntityFactory.cpp
        Source/EngineLogic/Entity.h Source/EngineLogic/Entity.cpp

        Source/Assets/myXML.h Source/Assets/myXML.cpp
        Source/Assets/ModelLoader.h Source/Assets/ModelLoader.cpp
        Source/Assets/Textures/MaterialsLoader.h Source/Assets/Textures/MaterialsLoader.cpp
        Source/Assets/Mesh/MeshLoader.h Source/Assets/Mesh/MeshLoader.cpp
        Source/Assets/SkeletalSystem/SkeletonLoader.h Source/Assets/SkeletalSystem/SkeletonLoader.cpp
        Source/Assets/SkeletalSystem/SkeletonAnimationLoader.h Source/Assets/SkeletalSystem/SkeletonAnimationLoader.cpp
        Source/Assets/Textures/Texture.h Source/Assets/Textures/Texture.cpp
        Source/Assets/Textures/TextureLoader.h Source/Assets/Textures/TextureLoader.cpp
        Source/Assets/AssimpConversion.h Source/Assets/AssimpConversion.cpp
        Source/Assets/SkeletalSystem/Skeleton.h Source/Assets/SkeletalSystem/Skeleton.cpp
        Source/Assets/SkeletalSystem/SkeletalAnimation.h Source/Assets/SkeletalSystem/SkeletalAnimation.cpp
        Source/Assets/SkeletalSystem/SkeletonConstants.h
        Source/Assets/Model.h Source/Assets/Model.cpp
        Source/Assets/Mesh/Mesh.h Source/Assets/Mesh/Mesh.cpp
        Source/Assets/Mesh/MeshGenerator.h Source/Assets/Mesh/MeshGenerator.cpp


        Source/Application.h Source/Application.cpp
        Source/Assets/SkeletalSystem/SkeletalAnimator.h Source/Assets/SkeletalSystem/SkeletalAnimator.cpp Source/Platform/OpenGL/Window/KeyStateGetter.cpp Source/Platform/OpenGL/Window/KeyStateGetter.h
        Source/Assets/Mesh/MeshFactory.cpp Source/Assets/Mesh/MeshFactory.h Source/Platform/OpenGL/Buffers/SingleAtributeBuffer.h OnChangeListenerTemplate/Utility.cpp

        Source/Platform/OpenGL/Texture/Texture.h Source/Platform/OpenGL/Texture/Texture.cpp
        Source/Assets/Textures/stb_image.h
        Source/Platform/OpenGL/Texture/sb7ktx.h Source/Platform/OpenGL/Texture/sb7ktx.cpp Source/Assets/Textures/Material.cpp Source/Assets/Textures/Material.h)

message(Headers: " ${INCLUDE_DIRS}")
add_executable(RenderV2 Source/main.cpp ${SOURCE} ${HEADERS} ${SOURCES_AND_HEADERS}    )
target_include_directories(RenderV2 PUBLIC ${INCLUDE_DIRS} )
target_link_libraries(RenderV2 ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${BOOST_LIBRARIES} ${GLM_LIBRARIES} ${ASSIMP_LIBRARIES} glfw stdc++fs)