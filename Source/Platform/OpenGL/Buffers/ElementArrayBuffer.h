//
// Created by dominik on 06.03.19.
//

#ifndef GAMEENGINE_ELEMENTARRAYBUFFER_H
#define GAMEENGINE_ELEMENTARRAYBUFFER_H

#include <GL/gl3w.h>
#include <vector>
#include "Buffer.h"

class ElementArrayBuffer : public Buffer
{
public:
    ElementArrayBuffer()
    {
    }

    ElementArrayBuffer(std::vector<GLushort> indicies)
    {
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(indicies[0]), indicies.data(), GL_STATIC_DRAW);
        unbind();
    }

    void bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer::myName);

    }

    void unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }

    void sendIfVaoEnabled(const std::vector<GLushort> indicies)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(indicies[0]), indicies.data(), GL_STATIC_DRAW);
    }
};


#endif //GAMEENGINE_ELEMENTARRAYBUFFER_H
