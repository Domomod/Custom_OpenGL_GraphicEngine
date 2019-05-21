//
// Created by dominik on 06.04.19.
//

#ifndef GAMEENGINE_TEXTURELOADER_H
#define GAMEENGINE_TEXTURELOADER_H

#include <string>
#include <memory>
#include <vector>

#include "Texture.h"
#include "CubicTexture.h"

class Texture;
class CubicTexture;
class Shader;

#include "Source/GraphicsLayer/ShaderProgram/Shader.h"

class TextureLoader {
public:
    static std::shared_ptr<Texture> loadTexture(const std::string& filePath);
    static std::shared_ptr<Texture> getDefaultTexture();

    static std::shared_ptr<CubicTexture> loadCubicTexture(const std::vector<std::string> & filePaths);
    static std::shared_ptr<CubicTexture> loadCubicTextureFromEquirectangluar(const std::shared_ptr<Texture> &texture);

    inline static std::string defaultTexturePath = "Textures/default.png"; // NOLINT(cert-err58-cpp)
    inline static std::shared_ptr<Texture> defaultTexture;
    inline static bool isDefaultTextureLoaded = false;

    static void setEquirToCubemapShaderSet(const std::shared_ptr<Shader> &equirToCubemapShaderSet);

private:
    inline static bool isEquirToCubemapShaderSet = false;
    inline static std::shared_ptr<Shader> equirToCubemapShader;
};

#endif //GAMEENGINE_TEXTURELOADER_H
