//
// Created by dominik on 29.03.19.
//

#include "Texture.h"
#include "sb7ktx.h"


#include <cmath>
#include <cassert>


GLuint format[] = {GL_STENCIL, GL_RED, GL_RG, GL_RGB, GL_RGBA};


void OGL_Texture2D::bind(unsigned int textureUnit)
{
    glBindTextureUnit(textureUnit, textureName);
}

void OGL_TextureCube::bind(unsigned int textureUnit)
{
    glBindTextureUnit(textureUnit, textureName);
}


OGL_Texture2D::OGL_Texture2D(int width, int height, int valuesPerColor, unsigned char *data, char flags)
{
    glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);

    GLuint internalFormat;
    if (flags & RUM_COMPRESS)
    {
        internalFormat = valuesPerColor < 4 ? GL_COMPRESSED_RGB_S3TC_DXT1_EXT : GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    }
    else
    {
        internalFormat = valuesPerColor < 4 ? GL_RGB8 : GL_RGBA8;
    }

    if (flags & RUM_GEN_MIPMAPS)
    {
        Texture2D::mipmapCount = static_cast<unsigned short>(log(width));
    }

    glTexStorage2D(GL_TEXTURE_2D, Texture2D::mipmapCount, internalFormat, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format[valuesPerColor], GL_UNSIGNED_BYTE, data);

    if (flags & RUM_GEN_MIPMAPS)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OGL_Texture2D::OGL_Texture2D(int width, int height, int valuesPerColor, float *data, char flags)
{
    glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);


    GLuint internalFormat;
    if (flags & RUM_COMPRESS)
    {
        internalFormat = valuesPerColor < 4 ? GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT : GL_RGBA16F;
    }
    else
    {
        internalFormat = valuesPerColor < 4 ? GL_RGB16F : GL_RGBA16F;
    }

    if (flags & RUM_GEN_MIPMAPS)
    {
        Texture2D::mipmapCount = static_cast<unsigned short>(log(width));
    }

    glTexStorage2D(GL_TEXTURE_2D, Texture2D::mipmapCount, internalFormat, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format[valuesPerColor], GL_FLOAT, data);

    if (flags & RUM_GEN_MIPMAPS)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OGL_Texture2D::~OGL_Texture2D()
{

}


OGL_TextureCube::OGL_TextureCube(int width, int height, int valuesPerColor, unsigned char **data, char flags)
{
    glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);


    GLuint internalFormat;
    if (flags & RUM_COMPRESS)
    {
        internalFormat = valuesPerColor < 4 ? GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT : GL_RGBA16F;
    }
    else
    {
        internalFormat = valuesPerColor < 4 ? GL_RGB16 : GL_RGBA16;
    }

    if (flags & RUM_GEN_MIPMAPS)
    {
        TextureCube::mipmapCount = static_cast<unsigned short>(log(width));
    }

    glTexStorage2D(GL_TEXTURE_2D, TextureCube::mipmapCount, internalFormat, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format[valuesPerColor], GL_UNSIGNED_BYTE, data);

    if (flags & RUM_GEN_MIPMAPS)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OGL_TextureCube::OGL_TextureCube(int width, int height, int valuesPerColor, float **data, char flags)
{

    assert(data[5]);

    glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);

    GLuint internalFormat;
    if (flags & RUM_COMPRESS)
    {
        internalFormat = valuesPerColor < 4 ? GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT : GL_RGBA16F;
    }
    else
    {
        internalFormat = valuesPerColor < 4 ? GL_RGB16F : GL_RGBA16F;
    }

    if (flags & RUM_GEN_MIPMAPS)
    {
        TextureCube::mipmapCount = static_cast<unsigned short>(log(width));
    }


    glTexStorage2D(GL_TEXTURE_2D, TextureCube::mipmapCount, internalFormat, width, height);
    for (unsigned int i = 0; i < 6; i++)
    {
        glTexSubImage2D(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, 0, 0,
                        width, height, format[valuesPerColor], GL_FLOAT, data[i]);
    }

    if (flags & RUM_GEN_MIPMAPS)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

}

OGL_TextureCube::OGL_TextureCube(const std::string & path)
{
    glGenTextures(1, &textureName);
    sb7::ktx::file::load(path.c_str(), textureName);
    TextureCube::mipmapCount = 7; /*TODO: */
}

OGL_TextureCube::~OGL_TextureCube()
{

}

OGL_TextureCube::OGL_TextureCube(GLuint textureName) : textureName(textureName)
{
}


