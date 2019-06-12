//
// Created by dominik on 29.03.19.
//

#ifndef GAMEENGINE_TEXTURE_H
#define GAMEENGINE_TEXTURE_H

#include <GL/gl3w.h>


class Texture {
public:
    Texture();

    virtual ~Texture() = default;;

    void bind(unsigned int textureUnit);

protected:
    GLuint textureName;
};

class Texture2D : public Texture{
    friend class TextureLoader;
public:

    Texture2D(int width, int height, int valuesPerColor, bool compress, unsigned char* data);
    Texture2D(int widt, int height, int valuesPerColor, bool compress, float* data);

    ~Texture2D() override;

private:
};

class TextureCube : public Texture{
    friend class TextureLoader;
public:
    explicit TextureCube(GLuint textureName);

    ~TextureCube() override;


};

#endif //GAMEENGINE_TEXTURE_H
