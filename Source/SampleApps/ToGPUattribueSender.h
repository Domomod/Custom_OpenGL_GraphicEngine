//
// Created by dominik on 26.02.19.
//

#ifndef GAMEENGINE_TOGPUDATALOADER_H
#define GAMEENGINE_TOGPUDATALOADER_H

#include <GL/gl3w.h>
#include <map>

#include "Mesh.h"
#include "../View/Shader.h"
#include "../MyExceptions.h"

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


class ToGPUattribueSender {
private:
    std::map<std::shared_ptr<Shader>,std::vector<VertexLoadingInfo>> shaderToAttributeLoadingInfoMap;
    GLuint vaoId;
    GLuint vboVerticesId;
    GLuint vboIndiciesId;

public:
    ToGPUattribueSender() {

    }

    virtual ~ToGPUattribueSender() {
        glDeleteBuffers(1, &vboIndiciesId);
        glDeleteBuffers(1, &vboVerticesId);
        glDeleteVertexArrays(1, &vaoId);
    }

    void createBuffers(){
        glGenVertexArrays(1, &vaoId);
        glGenBuffers(1, &vboVerticesId);
        glGenBuffers(1, &vboIndiciesId);
    }

    void addShader(std::shared_ptr<Shader> shader){
        shaderToAttributeLoadingInfoMap.emplace(shader, std::vector<VertexLoadingInfo>());
    }

    template<class ... Type>
    void addAttribute(std::shared_ptr<Shader> shader, Type ... args) {
        shaderToAttributeLoadingInfoMap[shader].emplace_back(args...);
    }

    void sendGeometryAndTopology(const std::shared_ptr<Shader>& shader, Mesh& mesh){
        glBindVertexArray(vaoId);
        sendGeometry(shader, mesh);
        sendTopology(mesh);
    }

    void sendGeometry(const std::shared_ptr<Shader>& shader, Mesh& mesh) {
        GLsizei stride = mesh.getVerticiesStride();

        size_t verticesSize = mesh.getVerticiesSizeInBytes();

        glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId);
        for(auto& info : shaderToAttributeLoadingInfoMap[shader]){
            glBufferData(GL_ARRAY_BUFFER, mesh.getVerticiesSizeInBytes(), mesh.getVerticiesDataPtr(), info.usage);
            glEnableVertexAttribArray(info.index);
            glVertexAttribPointer(info.index, info.numComponents, info.type, info.normalized, stride, reinterpret_cast<void*>(info.offset));
        }
    }

    void sendTopology(Mesh& mesh){
        size_t indicesSize = mesh.getIndiciesSizeInBytes();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndiciesId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, mesh.getIndiciesDataPtr(), GL_STATIC_DRAW);
    }
};


#endif //GAMEENGINE_TOGPUDATALOADER_H
