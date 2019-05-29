//
// Created by dominik on 07.03.19.
//

#ifndef GAMEENGINE_BUFFER_H
#define GAMEENGINE_BUFFER_H


#include <GL/gl3w.h>

class Buffer {
protected:
    GLuint myName;
public:
    Buffer() {
        glGenBuffers(1, &myName);
    }

    virtual ~Buffer() {
        glDeleteBuffers(1, &myName);
    }
};


#endif //GAMEENGINE_BUFFER_H
