//
// Created by dominik on 29.03.19.
//

#ifndef GAMEENGINE_TEXTURE_H
#define GAMEENGINE_TEXTURE_H

#include <GL/gl3w.h>


enum {
    RUM_COMPRESS = 1 << 0,
    RUM_GEN_MIPMAPS = 1 << 1
};

class Texture2D {
    friend class TextureLoader;
public:
    static Texture2D* Create(int width, int height, int valuesPerColor,  unsigned char* data, char flags = 0) ;
    static Texture2D* Create(int widt, int height, int valuesPerColor,  float* data, char flags = 0);

    virtual ~Texture2D() = default;

    virtual void bind(unsigned int textureUnit) = 0;
};

class TextureCube {
    friend class TextureLoader;
public:
    static TextureCube* Create(int width, int height, int valuesPerColor, unsigned char** data, char flags = 0);
    static TextureCube* Create(int width, int height, int valuesPerColor, float** data, char flags = 0);
    static TextureCube* Create(unsigned int textureName);

    virtual  ~TextureCube() = default;;


    virtual void bind(unsigned int textureUnit) = 0;

};

#endif //GAMEENGINE_TEXTURE_H
