//
// Created by dominik on 06.03.19.
//

#include <cstring>
#include "UniformBuffer.h"

void UniformBuffer::acquire(std::string &bufferName, Shader &shader) {
    binding = glGetUniformBlockIndex(shader.getProgram(), bufferName.c_str());
    acquire(binding, shader);
}

void UniformBuffer::acquire(GLuint _binding, Shader &shader) {
    binding = _binding;
    isAcquired = true;
    GLuint shaderProgramId = shader.getProgram();
    retrieveBlockSizeFromProgram(shaderProgramId);
    retrieveUniformOrderFromProgram(shaderProgramId);
    retrieveUniformOffsetsFromProgram(shaderProgramId);
    allocateBytesTable();
}

void UniformBuffer::retrieveBlockSizeFromProgram(GLuint program) {
    glGetActiveUniformBlockiv(program, binding,
                              GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
}

void UniformBuffer::retrieveUniformOrderFromProgram(GLuint program) {
    uniformOrder.resize(uniformNames.size());
    glGetUniformIndices(
            program,
            uniformNames.size(),
            uniformNames.data(),
            uniformOrder.data()
    );
}

void UniformBuffer::retrieveUniformOffsetsFromProgram(GLuint program) {
    uniformOffsets.resize(uniformNames.size());
    glGetActiveUniformBlockiv(
            program,
            binding,
            GL_UNIFORM_OFFSET,
            uniformOffsets.data()
    );
}

void UniformBuffer::allocateBytesTable() {
    bytesTable = (GLubyte *) malloc(blockSize);
}

void UniformBuffer::releaseBytesTable() {
    free(bytesTable);
}

void UniformBuffer::release() {
    isAcquired = false;
    releaseBytesTable();
}

UniformBuffer::~UniformBuffer() {
    if(isAcquired)
        release();
}

GLubyte *UniformBuffer::getBakedData() {
    if(!isAcquired)
        throw std::string("Buffer not acquired");
    bakeData();
    return bytesTable;
}

void UniformBuffer::addUniform(std::string name, void *data, size_t size) {
    GLchar* newName = new GLchar[name.size() + 1];
    strcpy(newName, name.c_str());
    uniformNames.push_back(newName);
    uniformData.push_back(data);
    uniformSizes.push_back(size);
}

void UniformBuffer::bakeData() {
    for(int i = 0; i < uniformNames.size(); i++){
        memcpy(bytesTable + uniformOffsets[i], uniformData[i], uniformSizes[i]);
    }
}
