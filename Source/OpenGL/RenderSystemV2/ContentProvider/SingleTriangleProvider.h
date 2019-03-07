//
// Created by dominik on 06.03.19.
//

#ifndef GAMEENGINE_SINGLETRIANGLEPROVIDER_H
#define GAMEENGINE_SINGLETRIANGLEPROVIDER_H

#include "Source/EntitySystem/Mesh.h"
#include "Source/OpenGL/RenderSystemV2/Buffers/AttributeBuffer.h"

class SingleTriangleProvider {
public:
    void setVao(GLuint vao) {
        SingleTriangleProvider::vao = vao;
    }

    void setVerticesVbo(GLuint verticesVbo) {
        SingleTriangleProvider::verticesVbo = verticesVbo;
    }

    void setIndicesVbo(GLuint indicesVbo) {
        SingleTriangleProvider::indicesVbo = indicesVbo;
    }

private:
    Mesh triangle;
    AttributeBuffer attributeBuffer;
    GLuint vao;
    GLuint verticesVbo;
    GLuint indicesVbo;
};


#endif //GAMEENGINE_SINGLETRIANGLEPROVIDER_H
