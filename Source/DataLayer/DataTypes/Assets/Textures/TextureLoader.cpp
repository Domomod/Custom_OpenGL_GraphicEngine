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

    std::shared_ptr<Texture> returnTexture = std::shared_ptr<Texture>(new Texture(width, height, data));
    SOIL_free_image_data(data);
    return returnTexture;
}

std::shared_ptr<Texture> TextureLoader::getDefaultTexture() {
    if(!isDefaultTextureLoaded){
        defaultTexture = loadTexture(defaultTexturePath);
    }
    return defaultTexture;
}
