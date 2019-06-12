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
#include "Source/DataLayer/DataTypes/Assets/Mesh/MeshGenerator.h"

class Texture2D;
class TextureCube;
class Shader;

#include "Source/GraphicsLayer/ShaderProgram/Shader.h"

class TextureLoader {
public:
    TextureLoader();

    static void                             setEquirToCubemapShaderSet(const std::shared_ptr<Shader> &shader);

    static void                             setEnviromentToDiffuseShader(const std::shared_ptr<Shader> &shader);

    static std::shared_ptr<Texture2D> loadTexture2D(const std::string &filePath);

    static std::shared_ptr<TextureCube> loadCubicTexture(const std::vector<std::string> &filePaths);

    static std::shared_ptr<Texture2D>         getDefaultTexture();

    static std::shared_ptr<TextureCube> calculateCubeMapFromEquirectangularTexture(
            const std::shared_ptr<Texture2D> &texture);

    static std::shared_ptr<TextureCube>    calculateDiffuseIrradianceMapFromEnviromentMap(
                                                    const std::shared_ptr<TextureCube>& enviromentMap );


    inline static std::string defaultTexturePath = "Textures/default.png"; // NOLINT(cert-err58-cpp)
    inline static std::shared_ptr<Texture2D> defaultTexture;
    inline static bool isDefaultTextureLoaded = false;

private:
    static unsigned int renderToCubeMap(unsigned int cubeMap,  unsigned int cubeMapSize);
    static unsigned int createCubeMap(unsigned int cubeMapSize);

    inline static bool isEquirToCubemapShaderSet = false;
    inline static std::shared_ptr<Shader> equirToCubemapShader;

    inline static bool isEnviromentToDiffuseShaderSet = false;
    inline static std::shared_ptr<Shader> enviromentToDiffuseShader;


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

    inline static std::shared_ptr<Mesh> skyboxMesh;
    inline static bool isSkyboxGenerated = false;


};



#endif //GAMEENGINE_TEXTURELOADER_H

#pragma clang diagnostic pop