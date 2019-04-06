//
// Created by dominik on 29.03.19.
//

#include "Texture.h"
#include <SOIL/SOIL.h>
#include <cmath>

Texture::Texture(const int &width, const int &height, unsigned char *data) {
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);

    //Allocate memory and fill it with the texture
    glTextureStorage2D( texID,
                        static_cast<GLsizei>(log2(width)), //make space for all mipmaps
                        GL_RGBA32F,
                        width,height
                        );

    glTextureSubImage2D( texID,
                         0,            //mipmap level
                         0,0,
                         width, height, //size
                         GL_RGBA,       //format of pixel data
                         GL_UNSIGNED_BYTE,      //data type
                         data
                         );

    //A mipmap does not take more than 1/3 original image, so I decided
    //to generate on for all loaded textures
    glGenerateMipmap(GL_TEXTURE_2D);


    //Each texture object has a sampler in it, for convenience I decided
    //to store the data in it, ideally it schould be transfered to a diffrent class,
    // allowing using same sample for multiple textures (less OpenGl context changes)
    glTextureParameteri( texID,
                         GL_TEXTURE_WRAP_R,
                         GL_REPEAT);

    //Trilinear interpolation for scaling the mipmapped texture up and down
    glTextureParameteri( texID,
                         GL_TEXTURE_MAG_FILTER,
                         GL_LINEAR
                         );

    glTextureParameteri( texID,
                         GL_TEXTURE_MIN_FILTER,
                         GL_LINEAR
    );
}

Texture::~Texture() {
    glDeleteTextures(1, &texID);
}

void Texture::bind() {
    glBindTextureUnit(0, texID);
}
