//
// Created by dominik on 01.03.19.
//

#ifndef GAMEENGINE_VERTEXLOADINGINFO_H
#define GAMEENGINE_VERTEXLOADINGINFO_H

#include <GL/gl3w.h>
#include <map>

class VertexLoadingInfo{
public:
    GLuint index;
    GLint numComponents;
    GLenum type;
    size_t offset;
    GLboolean normalized;
    GLenum usage;

    VertexLoadingInfo(GLuint index, GLint numComponents, GLenum type, size_t offset, GLenum usage = GL_STATIC_DRAW,
            GLboolean normalized = GL_FALSE) : index(index), numComponents(numComponents), type(type), offset(offset),
                                         normalized(normalized), usage(usage) {}
};

#endif //GAMEENGINE_VERTEXLOADINGINFO_H
