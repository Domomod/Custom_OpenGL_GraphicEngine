//
// Created by dominik on 06.04.19.
//

#include "TextureLoader.h"

#include <SOIL/SOIL.h>

#include "Source/MyExceptions.h"


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