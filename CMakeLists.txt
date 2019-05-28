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

find_package(SOIL REQUIRED)
IF (NOT SOIL_FOUND)
    message(WARNING SOIL-NOT-FOUND : ${SOIL_INCLUDE_DIR})
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
    message(SOIL-FOUND : ${SOIL_INCLUDE_DIR})
ENDIF()

include_directories( ${OPENGL_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS} ${SOIL_INCLUDE_DIRS} OnChangeListenerTemplate /usr/local/include)

add_subdirectory(gl3w)

set(    SOURCES_AND_HEADERS
        TinyXML2/tinyxml2.h                                                             TinyXML2/tinyxml2.cpp

        Source/MyExceptions.h                                                           Source/MyExceptions.cpp


        Source/GraphicsLayer/Window/Window.h                                            Source/GraphicsLayer/Window/Window.cpp
        Source/GraphicsLayer/Window/InputSystem.h                                       Source/GraphicsLayer/Window/InputSystem.cpp

        Source/GraphicsLayer/ShaderProgram/Shader.h                                     Source/GraphicsLayer/ShaderProgram/Shader.cpp
        Source/GraphicsLayer/OpenGLinitializer.h                                        Source/GraphicsLayer/OpenGLinitializer.cpp
        Source/GraphicsLayer/OpenGLinitializer.h                                        Source/GraphicsLayer/OpenGLinitializer.cpp
        Source/GraphicsLayer/FreeCamera.h                                               Source/GraphicsLayer/FreeCamera.cpp

        Source/GraphicsLayer/RenderSystemV2/Metadata/AttributeMetadata.h                Source/GraphicsLayer/RenderSystemV2/Metadata/AttributeMetadata.cpp
        Source/GraphicsLayer/RenderSystemV2/Buffers/ElementArrayBuffer.h
        Source/GraphicsLayer/RenderSystemV2/Buffers/Buffer.h
        Source/GraphicsLayer/RenderSystemV2/Buffers/VertexArrayObject.h                 Source/GraphicsLayer/RenderSystemV2/Buffers/VertexArrayObject.cpp
        Source/GraphicsLayer/RenderSystemV2/Buffers/AttributeBuffer.h                   Source/GraphicsLayer/RenderSystemV2/Buffers/AttributeBuffer.cpp
        Source/GraphicsLayer/RenderSystemV2/Metadata/UniformMetadata.h                  Source/GraphicsLayer/RenderSystemV2/Metadata/UniformMetadata.cpp
        Source/GraphicsLayer/RenderSystemV2/Buffers/UniformBuffer.h                     Source/GraphicsLayer/RenderSystemV2/Buffers/UniformBuffer.cpp


        Source/DataLayer/StorageManagment/ModelStorageManager.h
        Source/DataLayer/StorageManagment/ModelToModelSpaceMatricesMap.h
        Source/DataLayer/StorageManagment/ModelMatricesBuffer.h
        Source/DataLayer/StorageManagment/EntitySystem.h                                Source/DataLayer/StorageManagment/EntitySystem.cpp
        Source/DataLayer/StorageManagment/EntityStorageManager.h
        Source/DataLayer/StorageManagment/StorageManagerTemplate.h

        Source/DataLayer/DataTypes/GameLogic/EntityFactory.h                            Source/DataLayer/DataTypes/GameLogic/EntityFactory.cpp
        Source/DataLayer/DataTypes/GameLogic/Entity.h                                   Source/DataLayer/DataTypes/GameLogic/Entity.cpp

        Source/DataLayer/DataTypes/Assets/ModelLoader.h                                 Source/DataLayer/DataTypes/Assets/ModelLoader.cpp
        Source/DataLayer/DataTypes/Assets/MaterialsLoader.h                             Source/DataLayer/DataTypes/Assets/MaterialsLoader.cpp
        Source/DataLayer/DataTypes/Assets/MeshLoader.h                                  Source/DataLayer/DataTypes/Assets/MeshLoader.cpp
        Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletonLoader.h               Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletonLoader.cpp
        Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletonAnimationLoader.h      Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletonAnimationLoader.cpp
        Source/DataLayer/DataTypes/Assets/Textures/Texture.h                            Source/DataLayer/DataTypes/Assets/Textures/Texture.cpp
        Source/DataLayer/DataTypes/Assets/Textures/TextureLoader.h                      Source/DataLayer/DataTypes/Assets/Textures/TextureLoader.cpp
        Source/DataLayer/DataTypes/Assets/AssimpConversion.h                            Source/DataLayer/DataTypes/Assets/AssimpConversion.cpp
        Source/DataLayer/DataTypes/Assets/SkeletalSystem/Skeleton.h                     Source/DataLayer/DataTypes/Assets/SkeletalSystem/Skeleton.cpp
        Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimation.h            Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimation.cpp
        Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletonConstants.h
        Source/DataLayer/DataTypes/Assets/Model.h                                       Source/DataLayer/DataTypes/Assets/Model.cpp
        Source/DataLayer/DataTypes/Assets/Mesh.h                                        Source/DataLayer/DataTypes/Assets/Mesh.cpp
        Source/DataLayer/DataTypes/Assets/MeshGenerator.h                               Source/DataLayer/DataTypes/Assets/MeshGenerator.cpp


        Source/SampleApps/RenderV2Sample/Application.h                                  Source/SampleApps/RenderV2Sample/Application.cpp
        Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimator.h             Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimator.cpp Source/DataLayer/DataTypes/Assets/Textures/CubicTexture.cpp Source/DataLayer/DataTypes/Assets/Textures/CubicTexture.h Source/GraphicsLayer/Window/KeyStateGetter.cpp Source/GraphicsLayer/Window/KeyStateGetter.h)

add_executable(RenderV2 Source/SampleApps/RenderV2Sample/main.cpp ${SOURCE} ${HEADERS} ${SOURCES_AND_HEADERS}    )
target_include_directories(RenderV2 PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(RenderV2 ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS} ${BOOST_LIBRARIES} ${GLM_LIBRARIES} ${ASSIMP_LIBRARIES} ${SOIL_LIBRARIES} glfw stdc++fs)