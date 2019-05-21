//
// Created by dominik on 11.03.19.
//

#include "AttributeBuffer.h"

#include "Source/GraphicsLayer/RenderSystemV2/Metadata/AttributeMetadata.h"
#include "Source/MyExceptions.h"

void AttributeBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, Buffer::vbo);

    for(const auto& metadata : attributeMetadataTable){
        glEnableVertexAttribArray(metadata.index);
        glVertexAttribPointer(metadata.index, metadata.numberOfGenericValuesInSingleAttribute, metadata.dataType, metadata.normalized,
                              static_cast<GLsizei>(metadata.stride),
                              reinterpret_cast<void*>(metadata.offset));
        if (metadata.divisor!= 0){
            glVertexAttribDivisor(metadata.index, metadata.divisor);
        }
    }
}

void AttributeBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

AttributeBuffer::AttributeBuffer(const std::vector<AttributeMetadata> &attributeMetadataTable) : attributeMetadataTable(
        attributeMetadataTable) {}


AttributeBuffer AttributeBufferFactory::make() {
    if(!attributesAdded)
        throw InvalidData("Tried to make an attribute buffer without adding any attributes");
    attributesAdded = false;
    return AttributeBuffer(attributeMetadataTable);
}

AttributeBufferFactory &AttributeBufferFactory::insert(const AttributeMetadata &attributeMetadata) {
    if(!attributesAdded){
        attributesAdded = true;
        attributeMetadataTable.clear();
    }
    attributeMetadataTable.push_back(attributeMetadata);
    return *this;
}


