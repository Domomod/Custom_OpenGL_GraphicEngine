//
// Created by dominik on 12.05.19.
//

#include "CubicTexture.h"


CubicTexture::CubicTexture(int width, int height, unsigned char **data) {
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    /*Allocate memory and fill it with the texture*/
    glTextureStorage2D( texID,
                        static_cast<GLsizei>(log2(width)), //make space for all mipmaps
                        GL_RGBA32F,
                        width,height
    );

    if(data == nullptr){
        char baseColor[] = {127, 127, 127, 127};
        glClearTexSubImage( texID,
                            0,   //mipmap level
                            0,0, //x,y offset
                            0,   //start from 1st face
                            width, height,
                            6,   //Clear 6 faces
                            GL_RGBA,
                            GL_UNSIGNED_BYTE,
                            baseColor
        );
    }
    else{
        for(unsigned int i = 0; i < 6; i ++){
            setFaceTexture(i, width, height, data[i]);
        }

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);


        /*Set sampler parameters*/
        glTextureParameteri( texID,
                             GL_TEXTURE_WRAP_R,
                             GL_REPEAT);

        glTextureParameteri( texID,
                             GL_TEXTURE_MAG_FILTER,
                             GL_LINEAR
        );

        glTextureParameteri( texID,
                             GL_TEXTURE_MIN_FILTER,
                             GL_LINEAR
        );
    }
}


void CubicTexture::setFaceTexture(unsigned int face, int width, int height, unsigned char *data) {
    if(face > 5){
        throw InvalidData("Cube face num > 5 passed as an argument in setFaceTexture.");
    }

    glTexSubImage2D(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face), //which face
                    0,   //mipmap level
                    0, 0, // x,y offset
                    width, height,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    data
    );
}


void CubicTexture::bind(unsigned int textureUnit) {
    glBindTextureUnit(textureUnit, texID);
}
