//
// Created by dominik on 06.03.19.
//

#ifndef GAMEENGINE_ELEMENTARRAYBUFFER_H
#define GAMEENGINE_ELEMENTARRAYBUFFER_H

#include <GL/gl3w.h>
#include <vector>

class ElementArrayBuffer {
public:
    void sendIfVaoEnabled(GLuint indiciesVbo, const std::vector<GLushort> indicies){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesVbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(indicies[0]), indicies.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};


#endif //GAMEENGINE_ELEMENTARRAYBUFFER_H
