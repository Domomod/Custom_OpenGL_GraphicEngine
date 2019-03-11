//
// Created by dominik on 06.03.19.
//

#ifndef GAMEENGINE_ATTRIBUTEBUFFER_H
#define GAMEENGINE_ATTRIBUTEBUFFER_H

#include <GL/gl3w.h>
#include <vector>

#include "Source/EntitySystem/Mesh.h"
#include "Source/OpenGL/RenderSystemV2/Metadata/AttributeMetadata.h"
#include "Buffer.h"

class AttributeBuffer : public Buffer {
    friend class AttributeBufferFactory;
public:
    void bind() override;

    void unbind() override;

    void enableAllAttribsAndSpecifyTheirOffsetsIfVaoBinded();

    template<class VertexClass>
    void sendBufferToGPUifVaoBinded(std::vector<VertexClass> vertices){
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    }
private:
    std::vector<AttributeMetadata> attributeMetadataTable;
    AttributeBuffer(const std::vector<AttributeMetadata> &attributeMetadataTable);
};

class AttributeBufferFactory{
public:
    AttributeBuffer make();
    AttributeBufferFactory& insert(const AttributeMetadata &attributeMetadata);
private:
    bool attributesAdded = false;
    std::vector<AttributeMetadata> attributeMetadataTable;
};


#endif //GAMEENGINE_ATTRIBUTEBUFFER_H
