//
// Created by dominik on 26.02.19.
//

#ifndef GAMEENGINE_TOGPUDATALOADER_H
#define GAMEENGINE_TOGPUDATALOADER_H

#include <GL/gl3w.h>
#include <set>

#include "Mesh.h"
#include "../MyExceptions.h"

class ToGPUdataLoader {
private:
    class AttributeLoadingInfo{
    public:
        GLuint vbo;
        GLenum target;
        GLenum usage;
        GLuint location;
        GLint vectorSize;
        size_t offset;

        AttributeLoadingInfo(GLenum target, GLenum usage, GLuint location, GLint vectorSize, size_t offset)
                :target(target), usage(usage), location(location), vectorSize(vectorSize), offset(offset) {
            if(vectorSize > 4 || vectorSize < 1)
                throw InvalidData("AttributeLoadingInfo vector size value outside of <1,4>");
            glGenBuffers(1, &vbo);
        }
    };

    std::set<AttributeLoadingInfo> attributesLoadingInfo;
    GLuint vaoId;
    GLuint vboIndiciesId;

public:
    ToGPUdataLoader() {
        glGenVertexArrays(1, &vaoId);
        glGenBuffers(1, &vboIndiciesId);
    }

    void addAttributeWithoutLocationUniquenessChecking(GLuint location, GLenum target, GLenum usage, GLint vectorSize, size_t offset) {
        attributesLoadingInfo.emplace(target, usage, location, vectorSize, offset);
    }

    void insertGeometry(Mesh& mesh) {
        GLsizei stride = mesh.getVerticiesStride();

        size_t verticesSize = mesh.getVerticiesSizeInBytes();
        size_t positionOffset = mesh.getPositionOffset();
        size_t colorOffset = mesh.getColorOffset();

        glBindVertexArray(vaoId);

        for(auto& info : attributesLoadingInfo){

            GLuint vbo = info.vbo;
            GLenum target = info.target;
            GLuint location = info.location;
            GLint vectorSize = info.vectorSize;
            size_t offset = info.offset;

            glBindBuffer(info.target, info.vbo);
            glBufferData(info.target, verticesSize, mesh.getVerticiesDataPtr(), info.usage);
            glEnableVertexAttribArray(info.location);
            glVertexAttribPointer(info.location, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(info.usage));
        }
    }

    void insertTopology(Mesh& mesh){
        size_t indicesSize = mesh.getIndiciesSizeInBytes();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndiciesId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, mesh.getIndiciesDataPtr(), GL_STATIC_DRAW);
    }
};


#endif //GAMEENGINE_TOGPUDATALOADER_H
