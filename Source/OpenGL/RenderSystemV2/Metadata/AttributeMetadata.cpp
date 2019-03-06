//
// Created by dominik on 06.03.19.
//

#include "AttributeMetadata.h"

AttributeMetadata::AttributeMetadata(GLuint index, GLint numberOfGenericValuesInSingleAttribute, GLenum dataType, size_t offset,
                                     GLboolean normalized) : index(index), numberOfGenericValuesInSingleAttribute(arraySize), dataType(dataType),
                                                             offset(offset), normalized(normalized) {}

AttributeMetadata::~AttributeMetadata() {

}
