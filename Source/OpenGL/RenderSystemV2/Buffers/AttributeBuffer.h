//
// Created by dominik on 06.03.19.
//

#ifndef GAMEENGINE_ATTRIBUTEBUFFER_H
#define GAMEENGINE_ATTRIBUTEBUFFER_H

#include <GL/gl3w.h>
#include <vector>

#include "Source/EntitySystem/Mesh.h"
#include "Source/OpenGL/RenderSystemV2/Metadata/AttributeMetadata.h"

class AttributeBuffer {
public:
    template<class VertexClass>
    void sendIfVaoEnabled(GLuint verticesVbo, std::vector<VertexClass> vertices){
        glBindBuffer(GL_ARRAY_BUFFER, verticesVbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
        for(const auto& metadata : attributeMetadataTable){
            glEnableVertexAttribArray(metadata.index);
            glVertexAttribPointer(metadata.index, metadata.numberOfGenericValuesInSingleAttribute, metadata.dataType, metadata.normalized, sizeof(vertices[0]),
                                  reinterpret_cast<void*>(metadata.offset));
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
private:
    std::vector<AttributeMetadata> attributeMetadataTable;
};


#endif //GAMEENGINE_ATTRIBUTEBUFFER_H
