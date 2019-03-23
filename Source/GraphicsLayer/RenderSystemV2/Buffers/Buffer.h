//
// Created by dominik on 07.03.19.
//

#ifndef GAMEENGINE_BUFFER_H
#define GAMEENGINE_BUFFER_H


#include <GL/gl3w.h>

class Buffer {
protected:
    GLuint vbo;
public:
    virtual void bind() = 0;

    virtual void unbind() = 0;

    Buffer() {
        glGenBuffers(1, &vbo);
    }

    virtual ~Buffer() {
        glDeleteBuffers(1, &vbo);
    }
};


#endif //GAMEENGINE_BUFFER_H
