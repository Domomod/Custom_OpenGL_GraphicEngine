#include "Texture.h"

#include "Source/Platform/OpenGL/Texture.h"

TextureCube* TextureCube::Create(int width, int height, int valuesPerColor, unsigned char **data, char flags) {
    return new OGL_TextureCube(width, height, valuesPerColor, data, flags);
}

TextureCube* TextureCube::Create(int width, int height, int valuesPerColor, float **data, char flags) {
    return new OGL_TextureCube(width, height, valuesPerColor, data, flags);
}

TextureCube* TextureCube::Create(unsigned int textureName) {
    return new OGL_TextureCube(textureName);

}

Texture2D* Texture2D::Create(int width, int height, int valuesPerColor, unsigned char *data, char flags) {
    return new OGL_Texture2D(width, height, valuesPerColor, data, flags);

}

Texture2D* Texture2D::Create(int width, int height, int valuesPerColor, float *data, char flags) {
    return new OGL_Texture2D(width, height, valuesPerColor, data, flags);
}


