//
// Created by dominik on 06.04.19.
//

#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Source/MyExceptions.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/VertexArrayObject.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/AttributeBuffer.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/UniformBuffer.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/ElementArrayBuffer.h"
#include "Source/DataLayer/DataTypes/Assets/Mesh/Mesh.h"

#include "Texture.h"

#define RUM_COMPRESS true

std::shared_ptr<Texture2D> TextureLoader::loadTexture2D(const std::string &filePath) {
    int width, height, valuesPerColor;
    unsigned char* data= stbi_load(filePath.c_str(), &width, &height, &valuesPerColor, 0);

    if(data == nullptr){
        throw FileNotFound(filePath + " error while loading\n" + stbi__g_failure_reason);
    }

    Texture2D* texture = Texture2D::Create(width, height, valuesPerColor, data, RUM_COMPRESS);

    stbi_image_free(data);
    return std::shared_ptr<Texture2D>( texture );
}


std::shared_ptr<Texture2D> TextureLoader::getDefaultTexture() {
    if(!isDefaultTextureLoaded){
        defaultTexture = std::shared_ptr<Texture2D>(loadTexture2D(defaultTexturePath));
    }
    return defaultTexture;
}


std::shared_ptr<TextureCube> TextureLoader::loadCubicTexture(const std::vector<std::string> &filePaths) {
    if(filePaths.size()<6){
        throw InvalidData("To create a cubic texture you need to pass at least 6 paths texture\n.");
    }

    int width[6], height[6], valuesPerColor[6];
    unsigned char* data[6];

    for(int i = 0; i < 6; i++){
        data[i]= stbi_load(filePaths[i].c_str(), &width[i], &height[i], &valuesPerColor[i], 0);


        if(width[0] != width[i] || height[0] != height[i] || valuesPerColor[0] != valuesPerColor[i]){
            throw InvalidData("Provided images do not have the same size or same number of channels.");
        }
    }

    TextureCube* texture = TextureCube::Create(width[0], height[0], valuesPerColor[0], data, RUM_COMPRESS);

    stbi_image_free(data[0]);
    stbi_image_free(data[1]);
    stbi_image_free(data[2]);
    stbi_image_free(data[3]);
    stbi_image_free(data[4]);
    stbi_image_free(data[5]);

    return std::shared_ptr<TextureCube>( texture );
}

std::shared_ptr<TextureCube> TextureLoader::calculateCubeMapFromEquirectangularTexture(
        const std::shared_ptr<Texture2D> &texture) {

    if(!isEquirToCubemapShaderSet){
        throw TextureConverterShaderNotSet("Tried to create a cubic texture from a "
                                           "equirectangular image, without providing a converter shader.\n");
    }

    texture->bind(0);
    equirToCubemapShader->use();
    unsigned int cubeMap = createCubeMap(512);
    texture->bind(0);
    cubeMap = renderToCubeMap(cubeMap, 512);

    equirToCubemapShader->unuse();
    return std::shared_ptr<TextureCube>( TextureCube::Create(cubeMap) );
}

std::shared_ptr<TextureCube>TextureLoader::calculateDiffuseIrradianceMapFromEnviromentMap(
        const std::shared_ptr<TextureCube> &enviromentMap) {

    if(!isEnviromentToDiffuseShaderSet){
        throw TextureConverterShaderNotSet("Tried to create a diffuse map from a "
                                           "enviroment map, without providing a converter shader.\n");
    }

    enviromentToDiffuseShader->use();
    unsigned int cubeMap = createCubeMap(512);
    enviromentMap->bind(0);
    cubeMap = renderToCubeMap(cubeMap, 512);

    enviromentToDiffuseShader->unuse();
    return std::shared_ptr<TextureCube>(  TextureCube::Create(cubeMap) );
}

unsigned int TextureLoader::createCubeMap(unsigned int cubeMapSize) {
    unsigned int cubeMap;

    /*Create cubemap*/
    glGenTextures(1, &cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    /*TODO: add mipmaps*/
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

    return cubeMap;
}


unsigned int TextureLoader::renderToCubeMap(unsigned int cubeMap, unsigned int cubeMapSize) {
    GLint previousViewport[4];
    glGetIntegerv(GL_VIEWPORT, previousViewport );

    if(!isSkyboxGenerated) {
        skyboxMesh = MeshGenerator::generateSkyBox();
        isSkyboxGenerated = true;
    }
    skyboxMesh->bindVao();

    glm::mat4 Projection;
    glm::mat4 View;
    UniformBuffer basicMatriciesBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert( UniformMetadata( &Projection, GL_FLOAT_MAT4 ) )
            .insert( UniformMetadata( &View, GL_FLOAT_MAT4 ) )
            .make();



    /*Create framebuffers*/
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubeMapSize, cubeMapSize);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    /*Render to cube map*/
    glDepthFunc(GL_LEQUAL);
    glViewport(0, 0, cubeMapSize, cubeMapSize);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for(unsigned int face = 0; face < 6; face++){
        Projection = captureProjection;
        View = captureViews[face];

        basicMatriciesBuffer.bindBakeSend();

        glFramebufferTexture2D( GL_FRAMEBUFFER,
                                GL_COLOR_ATTACHMENT0,
                                GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                                cubeMap,
                                0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(skyboxMesh->getIndiciesCount()), GL_UNSIGNED_SHORT, nullptr);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(previousViewport[0],
               previousViewport[1],
               previousViewport[2],
               previousViewport[3]);
    return cubeMap;
}


TextureLoader::TextureLoader() {
    if(!isSkyboxGenerated) {
        skyboxMesh = MeshGenerator::generateSkyBox();
        isSkyboxGenerated = true;
    }
}

void TextureLoader::setEquirToCubemapShaderSet(const std::shared_ptr<Shader> &shader) {
    isEquirToCubemapShaderSet = true;
    equirToCubemapShader = shader;
}

void TextureLoader::setEnviromentToDiffuseShader(const std::shared_ptr<Shader> &shader) {
    isEnviromentToDiffuseShaderSet = true;
    enviromentToDiffuseShader = shader;
}



