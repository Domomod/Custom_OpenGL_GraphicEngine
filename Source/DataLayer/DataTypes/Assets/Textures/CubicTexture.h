//
// Created by dominik on 12.05.19.
//

#ifndef GAMEENGINE_CUBICTEXTURE_H
#define GAMEENGINE_CUBICTEXTURE_H

#include <GL/gl3w.h>
#include <cmath>

#include "Source/MyExceptions.h"

class CubicTexture {
    friend class TextureLoader;
public:
    unsigned int texID = 0;

    CubicTexture(unsigned int texture);

    CubicTexture(int width, int height, unsigned char **data);

    void setFaceTexture(unsigned int face, int width, int height, unsigned char *data);

    void attachFaceToFramebuffer(unsigned int face);

    void generateMipmaps();

    void bind(unsigned int textureUnit);
};

#endif //GAMEENGINE_CUBICTEXTURE_H