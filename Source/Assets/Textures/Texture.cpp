#include "Texture.h"

#include "Source/Platform/OpenGL/Texture/Texture.h"

TextureCube* TextureCube::Create(int width, int height, int valuesPerColor, unsigned char **data, char flags) {
    return new OGL_TextureCube(width, height, valuesPerColor, data, flags);
}


TextureCube* TextureCube::Create(int width, int height, int valuesPerColor, float **data, char flags) {
    return new OGL_TextureCube(width, height, valuesPerColor, data, flags);
}


TextureCube* TextureCube::Create(unsigned int textureName, unsigned short mipmapCount) {
    TextureCube* t = new OGL_TextureCube(textureName);
    t->mipmapCount = mipmapCount;
    return t;

}


TextureCube *TextureCube::CreateKTX(const std::string &path)
{
    /*TODO: if implementing more apis than openGL check if current api is OpenGL*/
    return new OGL_TextureCube(path);
}


unsigned short TextureCube::getMipmapCount() const
{
    return mipmapCount;
}


Texture2D* Texture2D::Create(int width, int height, int valuesPerColor, unsigned char *data, char flags) {
    return new OGL_Texture2D(width, height, valuesPerColor, data, flags);

}


Texture2D* Texture2D::Create(int width, int height, int valuesPerColor, float *data, char flags) {
    return new OGL_Texture2D(width, height, valuesPerColor, data, flags);
}


unsigned short Texture2D::getMipmapCount() const
{
    return mipmapCount;
}


