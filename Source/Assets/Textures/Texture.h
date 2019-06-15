//
// Created by dominik on 29.03.19.
//

#ifndef GAMEENGINE_TEXTURE_H
#define GAMEENGINE_TEXTURE_H

#include <GL/gl3w.h>
#include <string>

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

protected:
    unsigned short mipmapCount = 1;
public:
    unsigned short getMipmapCount() const;

};

class TextureCube {
    friend class TextureLoader;
public:
    static TextureCube* Create(int width, int height, int valuesPerColor, unsigned char** data, char flags = 0);
    static TextureCube* Create(int width, int height, int valuesPerColor, float** data, char flags = 0);
    static TextureCube* Create(unsigned int textureName, unsigned short mipmapCount);
    static TextureCube* CreateKTX(const std::string & path);

    virtual  ~TextureCube() = default;;


    virtual void bind(unsigned int textureUnit) = 0;

protected:
    unsigned short mipmapCount = 1;
public:
    unsigned short getMipmapCount() const;
};

#endif //GAMEENGINE_TEXTURE_H
