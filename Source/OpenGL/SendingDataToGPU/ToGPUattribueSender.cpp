//
// Created by dominik on 26.02.19.
//

#include "ToGPUattribueSender.h"

ToGPUattribueSender::~ToGPUattribueSender() {
    glDeleteBuffers(1, &vboIndiciesId);
    glDeleteBuffers(1, &vboVerticesId);
    glDeleteVertexArrays(1, &vaoId);
}

void ToGPUattribueSender::createBuffers() {
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboVerticesId);
    glGenBuffers(1, &vboIndiciesId);
}

void ToGPUattribueSender::addShader(std::shared_ptr<Shader> shader) {
    shaderToAttributeLoadingInfoMap.emplace(shader, std::vector<VertexLoadingInfo>());
}

void ToGPUattribueSender::sendGeometryAndTopology(const std::shared_ptr<Shader> &shader, Mesh &mesh) {
    glBindVertexArray(vaoId);
    sendGeometry(shader, mesh);
    sendTopology(mesh);
}

void ToGPUattribueSender::sendGeometry(const std::shared_ptr<Shader> &shader, Mesh &mesh) {
    GLsizei stride = mesh.getVerticiesStride();

    size_t verticesSize = mesh.getVerticiesSizeInBytes();

    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId);
    for(auto& info : shaderToAttributeLoadingInfoMap[shader]){
        glBufferData(GL_ARRAY_BUFFER, mesh.getVerticiesSizeInBytes(), mesh.getVerticiesDataPtr(), info.usage);
        glEnableVertexAttribArray(info.index);
        glVertexAttribPointer(info.index, info.numComponents, info.type, info.normalized, stride, reinterpret_cast<void*>(info.offset));
    }
}

void ToGPUattribueSender::sendTopology(Mesh &mesh) {
    size_t indicesSize = mesh.getIndiciesSizeInBytes();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndiciesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, mesh.getIndiciesDataPtr(), GL_STATIC_DRAW);
}
