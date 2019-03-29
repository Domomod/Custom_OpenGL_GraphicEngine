//
// Created by dominik on 07.03.19.
//

#ifndef GAMEENGINE_VERTEXARRAYOBJECT_H
#define GAMEENGINE_VERTEXARRAYOBJECT_H

#include <GL/gl3w.h>

#include "AttributeBuffer.h"

class VertexArrayObject {
private:
    GLuint vao;
public:
    void bind(){
        glBindVertexArray(vao);
    }

    void unbind(){
        glBindVertexArray(0);
    }

    VertexArrayObject() {
        glGenVertexArrays(1,&vao);
    }

    virtual ~VertexArrayObject() {
        glDeleteVertexArrays(1,&vao);
    }
};


#endif //GAMEENGINE_VERTEXARRAYOBJECT_H
