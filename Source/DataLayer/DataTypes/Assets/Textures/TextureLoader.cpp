//
// Created by dominik on 06.04.19.
//

#include "TextureLoader.h"

#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Source/MyExceptions.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/VertexArrayObject.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/AttributeBuffer.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/UniformBuffer.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/ElementArrayBuffer.h"
#include "Source/DataLayer/DataTypes/Assets/Mesh/MeshGenerator.h"
#include "Source/DataLayer/DataTypes/Assets/Mesh/Mesh.h"

#include "Texture.h"
#include "CubicTexture.h"

std::shared_ptr<Texture> TextureLoader::loadTexture(const std::string &filePath) {
    int width, height;
    unsigned char* data= SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    if(data == nullptr){
        throw FileNotFound(filePath + "not found");
    }

    std::shared_ptr<Texture> returnTexture = std::make_shared<Texture>(width, height, data);
    SOIL_free_image_data(data);
    return returnTexture;
}


std::shared_ptr<Texture> TextureLoader::getDefaultTexture() {
    if(!isDefaultTextureLoaded){
        defaultTexture = loadTexture(defaultTexturePath);
    }
    return defaultTexture;
}


std::shared_ptr<CubicTexture> TextureLoader::loadCubicTexture(const std::vector<std::string> &filePaths) {
    if(filePaths.size()<6){
        throw InvalidData("To create a cubic texture you need to pass at least 6 paths texture\n.");
    }

    int width[6], height[6];
    unsigned char* data[6];

    for(int i = 0; i < 6; i++){
        data[i] = SOIL_load_image(filePaths[i].c_str(), &width[i], &height[i], 0, SOIL_LOAD_RGBA);

        if(width[0] != width[i] || height[0] != height[i]){
            throw InvalidData("Not all cubic faces textures are of the same size.");
        }
    }

    std::shared_ptr<CubicTexture> returnTexture = std::make_shared<CubicTexture>(width[0], height[0], data);

    for (auto &i : data) {
        SOIL_free_image_data(i);
    }

    return returnTexture;
}

std::shared_ptr<CubicTexture> TextureLoader::loadCubicTextureFromEquirectangluar(
        const std::shared_ptr<Texture> &texture) {
    if(!isEquirToCubemapShaderSet){
        throw TextureConverterShaderNotSet("Tried to create a cubic texture from a "
                                           "equirectangular image, without providing a converter shader.\n");
    }

    VertexArrayObject vao;
    vao.bind();

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
            {
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
            };

    auto skyboxMesh = MeshGenerator::generateSkyBox();

    GLint previousViewport[4];
    glGetIntegerv( GL_VIEWPORT, previousViewport );

    skyboxMesh->bindVao();

    glm::mat4 Projection;
    glm::mat4 View;
    UniformBuffer basicShaderBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert( UniformMetadata( &Projection, GL_FLOAT_MAT4 ) )
            .insert( UniformMetadata( &View, GL_FLOAT_MAT4 ) )
            .make();


    /*Create framebuffers*/
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    unsigned int cubeMapSize = 512;

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubeMapSize, cubeMapSize);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    /*Create cubemap*/
    unsigned int cubeMap;
    glGenTextures(1, &cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    /*TODO: add mipmaps*/
//    glTexStorage2D( GL_TEXTURE_CUBE_MAP,
//                    1,
//                    GL_RGBA32F,
//                    cubeMapSize,
//                    cubeMapSize
//                    );
    for (unsigned int i = 0; i < 6; i++)
    {
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                     cubeMapSize, cubeMapSize, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*Render to cube map*/
    texture->bind(0);
    glDepthFunc(GL_LEQUAL);
    glViewport(0,0,cubeMapSize, cubeMapSize);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    equirToCubemapShader->use();
    for(unsigned int face = 0; face < 6; face++){
        Projection = captureProjection;
        View = captureViews[face];

        basicShaderBuffer.bind();
        basicShaderBuffer.bakeData();
        basicShaderBuffer.sendBufferToGPU();


        glFramebufferTexture2D( GL_FRAMEBUFFER,
                                GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                                cubeMap,
                                0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(skyboxMesh->getIndiciesCount()), GL_UNSIGNED_SHORT, nullptr);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    equirToCubemapShader->unuse();

    glViewport(previousViewport[0],
               previousViewport[1],
               previousViewport[2],
               previousViewport[3]);       

    return std::make_shared<CubicTexture>(cubeMap);
}

void TextureLoader::setEquirToCubemapShaderSet(const std::shared_ptr<Shader> &equirToCubemapShaderSet) {
    isEquirToCubemapShaderSet = true;
    TextureLoader::equirToCubemapShader = equirToCubemapShaderSet;
}
