//
// Created by dominik on 06.03.19.
//

#include "AttributeMetadata.h"

AttributeMetadata::AttributeMetadata(GLuint index, GLint numberOfGenericValuesInSingleAttribute, GLenum dataType, size_t offset,
                                     size_t stride, GLboolean normalized)
        : index(index), numberOfGenericValuesInSingleAttribute(numberOfGenericValuesInSingleAttribute), dataType(dataType),
          offset(offset), stride(stride), normalized(normalized) {}

AttributeMetadata::~AttributeMetadata() {

}
