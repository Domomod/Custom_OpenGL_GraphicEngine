//
// Created by dominik on 10.03.19.
//

#ifndef GAMEENGINE_UNIFORMBUFFER_H
#define GAMEENGINE_UNIFORMBUFFER_H

#include <vector>

#include "Buffer.h"
/* If user includes Uniform Buffer he will always want to include Uniform Metadata,
 * that's why i chose to inclde it instead of forward declare it.*/
#include "Metadata/UniformMetadata.h"

/* To work properly, uniform block in a shader schould have
 * a layout std140 and binding point specified by programmer
 * */

class UniformBuffer : public Buffer
{
    friend class UniformBufferFactory;

public:
    void bind();

    void unbind();

    void bindBakeSend();

    void sendBufferToGPU();

    void bakeData();

private:
    GLuint binding;
    size_t bakedDataSize;
    std::vector<GLubyte> bakedData;
    std::vector<UniformMetadata> uniformsMetadata;

public:
    UniformBuffer(GLuint binding, size_t bakedDataSize, const std::vector<GLubyte> &bakedData,
                  const std::vector<UniformMetadata> &uniformsMetadata);
};

class UniformBufferFactory
{
public:
    UniformBuffer make();

    UniformBufferFactory &setBinding(GLuint binding);

    UniformBufferFactory &insert(const UniformMetadata &uniformMetadata);

private:
    bool uniformsAdded = false;
    bool bindingSet = false;

    GLuint binding;
    size_t bakedDataSize;
    std::vector<GLubyte> bakedData;
    std::vector<UniformMetadata> uniformsMetadata;
};

#endif //GAMEENGINE_UNIFORMBUFFER_H