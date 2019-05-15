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

class TextureLoader {
public:
    static std::shared_ptr<Texture> loadTexture(const std::string& filePath);
    static std::shared_ptr<Texture> getDefaultTexture();

    static std::shared_ptr<CubicTexture> loadCubicTexture(const std::vector<std::string> & filePaths);

    inline static std::string defaultTexturePath = "Textures/default.png"; // NOLINT(cert-err58-cpp)
    inline static std::shared_ptr<Texture> defaultTexture;
    inline static bool isDefaultTextureLoaded = false;
};

#endif //GAMEENGINE_TEXTURELOADER_H
