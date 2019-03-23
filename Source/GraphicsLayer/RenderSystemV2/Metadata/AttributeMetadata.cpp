//
// Created by dominik on 06.03.19.
//

#include "AttributeMetadata.h"

AttributeMetadata::AttributeMetadata(GLuint index, GLint numberOfGenericValuesInSingleAttribute, GLenum dataType, size_t offset,
                                     size_t stride, GLuint divisor, GLboolean normalized)
        : index(index), numberOfGenericValuesInSingleAttribute(numberOfGenericValuesInSingleAttribute), dataType(dataType),
          offset(offset), stride(stride), divisor(divisor), normalized(normalized) {}

AttributeMetadata::~AttributeMetadata() {

}
