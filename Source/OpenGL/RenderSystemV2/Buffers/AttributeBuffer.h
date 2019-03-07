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
public:
    void bind() override {
        glBindBuffer(GL_ARRAY_BUFFER, Buffer::vbo);

    }

    void unbind() override {
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    void addAttributeMetadata(const AttributeMetadata& attributeMetadata){
        attributeMetadataTable.push_back(attributeMetadata);
    }

    void enableAllAttribsAndSpecifyTheirOffsetsIfVaoBinded(){
        for(const auto& metadata : attributeMetadataTable){
            glEnableVertexAttribArray(metadata.index);
            glVertexAttribPointer(metadata.index, metadata.numberOfGenericValuesInSingleAttribute, metadata.dataType, metadata.normalized,
                                  static_cast<GLsizei>(metadata.stride),
                                  reinterpret_cast<void*>(metadata.offset));
        }
    }

    template<class VertexClass>
    void sendBufferToGPUifVaoBinded(std::vector<VertexClass> vertices){
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    }
private:
    std::vector<AttributeMetadata> attributeMetadataTable;
};


#endif //GAMEENGINE_ATTRIBUTEBUFFER_H
