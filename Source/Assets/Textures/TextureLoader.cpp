//
// Created by dominik on 06.04.19.
//

#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include "MyExceptions.h"
#include "Platform/OpenGL/Buffers/VertexArrayObject.h"
#include "Platform/OpenGL/Buffers/AttributeBuffer.h"
#include "Platform/OpenGL/Buffers/UniformBuffer.h"
#include "Platform/OpenGL/Buffers/ElementArrayBuffer.h"
#include "Assets/Mesh/Mesh.h"

#include "Texture.h"

#define RUM_COMPRESS true


TextureLoader::TextureLoader()
{
    equirToCubemapShader.loadFromFile(Shader::VERTEX,
                                       "../Shaders/CubeMaps/centered_cube_map_vertex.glsl");
    equirToCubemapShader.loadFromFile(Shader::FRAGMENT,
                                       "../Shaders/CubeMaps/EquirSkyBox/fragment.glsl");
    equirToCubemapShader.createAndLinkProgram();

    diffuseMapShader.loadFromFile(Shader::VERTEX, "../Shaders/CubeMaps/centered_cube_map_vertex.glsl");
    diffuseMapShader.loadFromFile(Shader::FRAGMENT,
                                   "../Shaders/CubeMaps/DiffuseIrradianceMap/fragment.glsl");
    diffuseMapShader.createAndLinkProgram();

    prefilteredMapShader.loadFromFile(Shader::VERTEX, "../Shaders/CubeMaps/centered_cube_map_vertex.glsl");
    prefilteredMapShader.loadFromFile(Shader::FRAGMENT,
                                       "../Shaders/CubeMaps/PrefilteredEnviromentMap/fragment.glsl");
    prefilteredMapShader.createAndLinkProgram();

    defaultTexture = std::shared_ptr<Texture2D>(loadTexture2D(defaultTexturePath));

    skyboxMesh = MeshGenerator::generateSkyBox();
}


std::shared_ptr<Texture2D> TextureLoader::loadTexture2D(const std::string &filePath)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, valuesPerColor;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &valuesPerColor, 0);

    if (data == nullptr)
    {
        throw FileNotFound(filePath + " error while loading\n" + stbi__g_failure_reason);
    }

    Texture2D *texture = Texture2D::Create(width, height, valuesPerColor, data, RUM_COMPRESS);

    stbi_image_free(data);
    return std::shared_ptr<Texture2D>(texture);
}

std::shared_ptr<Texture2D> TextureLoader::loadTexture2D_F(const std::string &filePath)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, valuesPerColor;
    float *data = stbi_loadf(filePath.c_str(), &width, &height, &valuesPerColor, 0);

    if (data == nullptr)
    {
        throw FileNotFound(filePath + " error while loading\n" + stbi__g_failure_reason);
    }

    Texture2D *texture = Texture2D::Create(width, height, valuesPerColor, data);

    stbi_image_free(data);
    return std::shared_ptr<Texture2D>(texture);
}




std::shared_ptr<Texture2D> TextureLoader::getDefaultTexture()
{
    return defaultTexture;
}


std::shared_ptr<TextureCube> TextureLoader::loadCubicTexture(const std::vector<std::string> &filePaths)
{
    stbi_set_flip_vertically_on_load(true);

    if (filePaths.size() < 6)
    {
        throw InvalidData("To create a cubic texture you need to pass at least 6 paths texture\n.");
    }

    int width[6], height[6], valuesPerColor[6];
    unsigned char *data[6];

    for (int i = 0; i < 6; i++)
    {
        data[i] = stbi_load(filePaths[i].c_str(), &width[i], &height[i], &valuesPerColor[i], 0);


        if (width[0] != width[i] || height[0] != height[i] || valuesPerColor[0] != valuesPerColor[i])
        {
            throw InvalidData("Provided images do not have the same size or same number of channels.");
        }
    }

    TextureCube *texture = TextureCube::Create(width[0], height[0], valuesPerColor[0], data, RUM_COMPRESS);

    stbi_image_free(data[0]);
    stbi_image_free(data[1]);
    stbi_image_free(data[2]);
    stbi_image_free(data[3]);
    stbi_image_free(data[4]);
    stbi_image_free(data[5]);

    return std::shared_ptr<TextureCube>(texture);
}

std::shared_ptr<TextureCube> TextureLoader::loadCubicTexture_F(const std::vector<std::string> &filePaths)
{
    stbi_set_flip_vertically_on_load(true);

    if (filePaths.size() < 6)
    {
        throw InvalidData("To create a cubic texture you need to pass at least 6 paths texture\n.");
    }

    int width[6], height[6], valuesPerColor[6];
    float *data[6];

    for (int i = 0; i < 6; i++)
    {
        data[i] = stbi_loadf(filePaths[i].c_str(), &width[i], &height[i], &valuesPerColor[i], 0);


        if (width[0] != width[i] || height[0] != height[i] || valuesPerColor[0] != valuesPerColor[i])
        {
            throw InvalidData("Provided images do not have the same size or same number of channels.");
        }
    }

    TextureCube *texture = TextureCube::Create(width[0], height[0], valuesPerColor[0], data, RUM_COMPRESS);

    stbi_image_free(data[0]);
    stbi_image_free(data[1]);
    stbi_image_free(data[2]);
    stbi_image_free(data[3]);
    stbi_image_free(data[4]);
    stbi_image_free(data[5]);

    return std::shared_ptr<TextureCube>(texture);
}


std::shared_ptr<TextureCube> TextureLoader::loadCubicTexture_KTX(const std::string &filePath)
{
    TextureCube *texture = TextureCube::CreateKTX(filePath);

    return std::shared_ptr<TextureCube>(texture);
}


std::shared_ptr<TextureCube> TextureLoader::calculateCubeMapFromEquirectangularTexture(
        const std::shared_ptr<Texture2D> &texture)
{
    texture->bind(0);
    equirToCubemapShader.use();
    unsigned int cubeMap = createCubeMap(1024);
    texture->bind(0);
    cubeMap = renderToCubeMap(cubeMap, 1024, true);

    equirToCubemapShader.unuse();
    return std::shared_ptr<TextureCube>(TextureCube::Create(cubeMap, static_cast<unsigned short>(log(1024))));
}


std::shared_ptr<TextureCube> TextureLoader::calculateDiffuseIrradianceMapFromEnviromentMap(
        const std::shared_ptr<TextureCube> &enviromentMap)
{
    diffuseMapShader.use();
    unsigned int cubeMap = createCubeMap(512);
    enviromentMap->bind(0);
    cubeMap = renderToCubeMap(cubeMap, 512);

    diffuseMapShader.unuse();
    return std::shared_ptr<TextureCube>(TextureCube::Create(cubeMap, 1));
}

unsigned int TextureLoader::createCubeMap(unsigned int cubeMapSize)
{
    unsigned int cubeMap;

    /*Create cubemap*/
    glGenTextures(1, &cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    /*TODO: add mipmaps*/
    for (unsigned int i = 0; i < 6; i++)
    {
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


unsigned int TextureLoader::renderToCubeMap(unsigned int cubeMap, unsigned int cubeMapSize, bool mipmaps)
{
    GLint previousViewport[4];
    glGetIntegerv(GL_VIEWPORT, previousViewport);

    skyboxMesh->bindVao();

    glm::mat4 Projection;
    glm::mat4 View;
    UniformBuffer basicMatriciesBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert(UniformMetadata(&Projection, GL_FLOAT_MAT4))
            .insert(UniformMetadata(&View, GL_FLOAT_MAT4))
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
    for (unsigned int face = 0; face < 6; face++)
    {
        Projection = captureProjection;
        View = captureViews[face];

        basicMatriciesBuffer.bindBakeSend();

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                               cubeMap,
                               0);

        if(mipmaps)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP ,cubeMap);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        }

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

std::shared_ptr<TextureCube>
TextureLoader::calculatePrefilteredEnviromentMap(const std::shared_ptr<TextureCube> &enviromentMap)
{
    GLint previousViewport[4];
    glGetIntegerv(GL_VIEWPORT, previousViewport);

    unsigned int cubeMapSize = 512;


    unsigned int prefilterMap;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubeMapSize, cubeMapSize, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);


    glm::mat4 Projection = captureProjection;
    glm::mat4 View;
    UniformBuffer basicMatriciesBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert(UniformMetadata(&Projection, GL_FLOAT_MAT4))
            .insert(UniformMetadata(&View, GL_FLOAT_MAT4))
            .make();

    float roughness;
    UniformBuffer roughnessBuffer = UniformBufferFactory()
            .setBinding(1)
            .insert(UniformMetadata(&roughness, GL_FLOAT))
            .make();

    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubeMapSize, cubeMapSize);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;

    prefilteredMapShader.use();
    enviromentMap->bind(0);
    skyboxMesh->bindVao();
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth  = cubeMapSize * std::pow(0.5, mip);
        unsigned int mipHeight = cubeMapSize * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        roughness = (float)mip / (float)(maxMipLevels - 1);
        roughnessBuffer.bindBakeSend();
        for (unsigned int i = 0; i < 6; ++i)
        {
            View = captureViews[i];
            basicMatriciesBuffer.bindBakeSend();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(skyboxMesh->getIndiciesCount()), GL_UNSIGNED_SHORT, nullptr);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(previousViewport[0],
               previousViewport[1],
               previousViewport[2],
               previousViewport[3]);
    return std::shared_ptr<TextureCube>(TextureCube::Create(prefilterMap, maxMipLevels));
}
