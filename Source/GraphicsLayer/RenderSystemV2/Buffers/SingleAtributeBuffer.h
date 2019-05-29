//
// Created by dominik on 29.05.19.
//

#ifndef GAMEENGINE_SINGLEATRIBUTEBUFFER_H
#define GAMEENGINE_SINGLEATRIBUTEBUFFER_H

#include <GL/gl3w.h>
#include <vector>
#include <iostream>

#include "Buffer.h"

template<unsigned int binding, class AttribType>
class SingleAttributeBuffer : public Buffer {
public:
    SingleAttributeBuffer() = default;

    void bufferData(const std::vector<AttribType> data){
        if(!data.empty()) {
            int size = sizeof(data[0]) / sizeof(data[0][0]);
            glBindBuffer(GL_ARRAY_BUFFER, myName);
            glBufferData(GL_ARRAY_BUFFER, sizeof(AttribType) * data.size(), data.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(binding, size, GL_FLOAT, GL_FALSE, sizeof(data[0]), (void *) 0);
            glEnableVertexAttribArray(binding);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        else{
            isDummyArray = true;
            glDeleteBuffers(1, &myName);
            myName = 0;
        }
    }

    void bind(unsigned int vaoName) {
        if(isDummyArray){
            std::cerr << "VBO with binding point " << binding << " is a dummy buffer. Ignored binding.\n";
        }
        glEnableVertexArrayAttrib(vaoName, binding);
    }

    void unbind(unsigned int vaoName) {
        if(isDummyArray){
            std::cerr << "VBO with binding point " << binding << " is a dummy buffer. Ignored unbinding.\n";
        }
        glDisableVertexArrayAttrib(vaoName, binding);
    }

    bool isDummyArray = false;
};

#endif //GAMEENGINE_SINGLEATRIBUTEBUFFER_H
