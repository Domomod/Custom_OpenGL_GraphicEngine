//
// Created by dominik on 06.03.19.
//

#ifndef GAMEENGINE_ATTRIBUTEMETADATA_H
#define GAMEENGINE_ATTRIBUTEMETADATA_H

#include <GL/gl3w.h>
#include <cstdio>

struct AttributeMetadata
{
    GLuint index;
    GLint numberOfGenericValuesInSingleAttribute;
    GLenum dataType;
    size_t offset;
    size_t stride;
    GLuint divisor;
    GLboolean normalized;

    AttributeMetadata(GLuint index, GLint numberOfGenericValuesInSingleAttribute, GLenum dataType, size_t offset,
                      size_t stride, GLuint divisor = 0, GLboolean normalized = GL_FALSE);

    virtual ~AttributeMetadata();
};


#endif //GAMEENGINE_ATTRIBUTEMETADATA_H
