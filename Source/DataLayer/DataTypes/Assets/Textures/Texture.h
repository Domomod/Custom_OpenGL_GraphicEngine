//
// Created by dominik on 29.03.19.
//

#ifndef GAMEENGINE_TEXTURE_H
#define GAMEENGINE_TEXTURE_H

#include <GL/gl3w.h>

class Texture {
    friend class TextureLoader;
public:
    GLuint texID;
    void bind(unsigned int bindPoint);

    virtual ~Texture();

    Texture(const int &width, const int &height, unsigned char *data);
private:
};
#endif //GAMEENGINE_TEXTURE_H
