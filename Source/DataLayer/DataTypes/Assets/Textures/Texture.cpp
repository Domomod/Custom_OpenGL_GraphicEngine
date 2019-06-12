//
// Created by dominik on 29.03.19.
//

#include "Texture.h"
#include <SOIL/SOIL.h>
#include <cmath>
#include <cassert>

GLuint format[] = {GL_STENCIL, GL_RED, GL_RG, GL_RGB, GL_RGBA};

void Texture::bind(unsigned int textureUnit)
{
    glBindTextureUnit(textureUnit, textureName);
}

Texture::Texture()
{
    glGenTextures(1, &textureName);
}



Texture2D::Texture2D(int width, int height, int valuesPerColor, bool compress, unsigned char *data)
{
    glBindTexture(GL_TEXTURE_2D, Texture::textureName);


    GLuint internalFormat;
    if(compress)
    {
        internalFormat = valuesPerColor < 4?  GL_COMPRESSED_RGB_S3TC_DXT1_EXT : GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    }
    else
    {
        internalFormat = valuesPerColor < 4?  GL_RGB8 : GL_RGBA8;
    }

    glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format[valuesPerColor], GL_UNSIGNED_BYTE, data);

//    if(mipmas)
//    {
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(int width, int height, int valuesPerColor, bool compress, float *data)
{
    glBindTexture(GL_TEXTURE_2D, Texture::textureName);


    GLuint internalFormat;
    if(compress)
    {
        internalFormat = valuesPerColor < 4?  GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT : GL_RGBA16F;
    }
    else
    {
        internalFormat = valuesPerColor < 4?  GL_RGB16F : GL_RGBA16F;
    }

    glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format[valuesPerColor], GL_UNSIGNED_BYTE, data);

//    if(mipmas)
//    {
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{

}


TextureCube::TextureCube(GLuint textureName)  {}

TextureCube::~TextureCube() {

}

