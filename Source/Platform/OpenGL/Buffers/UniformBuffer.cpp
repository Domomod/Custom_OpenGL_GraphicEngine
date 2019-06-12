//
// Created by dominik on 10.03.19.
//

#include <cstring>
#include "UniformBuffer.h"
#include "MyExceptions.h"

void UniformBuffer::bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, Buffer::myName);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, Buffer::myName);
}

void UniformBuffer::unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void UniformBuffer::sendBufferToGPU()
{
    glBufferData(GL_UNIFORM_BUFFER, bakedDataSize, bakedData.data(), GL_STATIC_DRAW);
}

void UniformBuffer::bakeData()
{
    size_t offset = 0;
    bakedData.resize(bakedDataSize);
    for (auto &metadata : uniformsMetadata)
    {
        memcpy(bakedData.data() + offset, metadata.pointerToData, metadata.arraySize * metadata.size);
        offset += metadata.arraySize * metadata.allign;
    }
}

UniformBuffer::UniformBuffer(GLuint binding, size_t bakedDataSize, const std::vector<GLubyte> &bakedData,
                             const std::vector<UniformMetadata> &uniformsMetadata) : binding(binding),
                                                                                     bakedDataSize(bakedDataSize),
                                                                                     bakedData(bakedData),
                                                                                     uniformsMetadata(
                                                                                             uniformsMetadata)
{
}

void UniformBuffer::bindBakeSend()
{
    bind();
    bakeData();
    sendBufferToGPU();
}


//FACTORY
UniformBuffer UniformBufferFactory::make()
{
    if (!bindingSet && !uniformsAdded)
        throw InvalidData("Tried to make a uniform buffer without specifying binding point or uniforms");
    bakedDataSize = bakedDataSize;
    bindingSet = false;
    uniformsAdded = false;
    return UniformBuffer(binding, bakedDataSize, bakedData, uniformsMetadata);
}

UniformBufferFactory &UniformBufferFactory::setBinding(GLuint binding)
{
    this->binding = binding;
    return *this;
}

UniformBufferFactory &UniformBufferFactory::insert(const UniformMetadata &uniformMetadata)
{
    if (!uniformsAdded)
    {
        uniformsMetadata.clear();
        bakedDataSize = 0;
    }
    uniformsAdded = true;
    uniformsMetadata.push_back(uniformMetadata);
    bakedDataSize += uniformMetadata.arraySize * uniformMetadata.allign;
    return *this;
}