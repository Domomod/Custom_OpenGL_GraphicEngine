//
// Created by dominik on 29.03.19.
//

#ifndef GAMEENGINE_OGLTEXTURE_H
#define GAMEENGINE_OGLTEXTURE_H

#include <GL/gl3w.h>

#include "Assets/Textures/Texture.h"

class OGL_Texture2D : public Texture2D
{
    friend class TextureLoader;

public:

    OGL_Texture2D(int width, int height, int valuesPerColor, unsigned char *data, char flags = 0);

    OGL_Texture2D(int widt, int height, int valuesPerColor, float *data, char flags = 0);

    ~OGL_Texture2D();

    virtual void bind(unsigned int textureUnit);

private:
    GLuint textureName;

};

class OGL_TextureCube : public TextureCube
{
    friend class TextureLoader;

public:
    OGL_TextureCube(int width, int height, int valuesPerColor, unsigned char **data, char flags = 0);

    OGL_TextureCube(int width, int height, int valuesPerColor, float **data, char flags = 0);

    explicit OGL_TextureCube(GLuint textureName);

    virtual void bind(unsigned int textureUnit);

    ~OGL_TextureCube();

private:
    GLuint textureName;
};

#endif //GAMEENGINE_OGLTEXTURE_H
