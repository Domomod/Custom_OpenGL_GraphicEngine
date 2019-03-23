//
// Created by dominik on 10.03.19.
//

#include "UniformMetadata.h"

UniformMetadata::UniformMetadata(void *pointerToData, GLenum dataType) : pointerToData(pointerToData), dataType(dataType) {
    switch(dataType){
        case GL_FLOAT_MAT4:
            allign = size = 4 * 4 * sizeof(float);
            break;
        case GL_FLOAT_VEC2:
            allign = size = 2 * sizeof(float);
            break;
        case GL_FLOAT_VEC3:
            allign = 4 * sizeof(float);
            size = 2 * sizeof(float);
            break;
        case GL_FLOAT_VEC4:
            allign = size = 4 * sizeof(float);
            break;
        case GL_INT:
        case GL_BOOL:
        case GL_FLOAT:
            allign = size = sizeof(float);
            break;
        default:
            throw InvalidData("GLenum dataType provided in UniformMetadata constructor is not implemented");
    }
}

UniformMetadata::~UniformMetadata() {

}

UniformMetadata::UniformMetadata() {}
