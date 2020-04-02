#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by dominik on 06.04.19.
//

#ifndef GAMEENGINE_TEXTURELOADER_H
#define GAMEENGINE_TEXTURELOADER_H

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Assets/Mesh/MeshGenerator.h"
#include "Platform/OpenGL/ShaderProgram/Shader.h"

class Texture2D;
class TextureCube;
class Shader;

#include "Platform/OpenGL/ShaderProgram/Shader.h"

class TextureLoader {
public:
    TextureLoader();
    std::shared_ptr<Texture2D> loadTexture2D(const std::string &filePath);
    std::shared_ptr<Texture2D> loadTexture2D_F(const std::string &filePath);

    std::shared_ptr<TextureCube> loadCubicTexture(const std::vector<std::string> &filePaths);
    std::shared_ptr<TextureCube> loadCubicTexture_F(const std::vector<std::string> &filePaths);
    std::shared_ptr<TextureCube> loadCubicTexture_KTX(const std::string &filePath);

    std::shared_ptr<Texture2D>         getDefaultTexture();

    std::shared_ptr<TextureCube> calculateCubeMapFromEquirectangularTexture(
            const std::shared_ptr<Texture2D> &texture);

    std::shared_ptr<TextureCube>    calculateDiffuseIrradianceMapFromEnviromentMap(
                                                    const std::shared_ptr<TextureCube>& enviromentMap );

    std::shared_ptr<TextureCube>    calculatePrefilteredEnviromentMap(
            const std::shared_ptr<TextureCube>& enviromentMap );

private:
    unsigned int renderToCubeMap(unsigned int cubeMap, unsigned int cubeMapSize, bool mipmaps = false);
    unsigned int createCubeMap(unsigned int cubeMapSize);

    Shader equirToCubemapShader;
    Shader diffuseMapShader;
    Shader prefilteredMapShader;


     inline static glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
     inline static glm::mat4 captureViews[] =
            {
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
            };

    std::shared_ptr<Mesh> skyboxMesh;
    bool isSkyboxGenerated = false;

    std::string defaultTexturePath = "Textures/default.png"; // NOLINT(cert-err58-cpp)
    std::shared_ptr<Texture2D> defaultTexture;
    bool isDefaultTextureLoaded = false;
};



#endif //GAMEENGINE_TEXTURELOADER_H

#pragma clang diagnostic pop