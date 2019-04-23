//
// Created by dominik on 23.04.19.
//

#include "ModelLoader.h"
#include "AssimpConversion.h"
#include <glm/gtc/type_ptr.hpp>

glm::mat4 assmipMatToGlmMat(aiMatrix4x4 matrix) {

    float mat[] = {
            matrix.a1, matrix.a2, matrix.a3, matrix.a4,
            matrix.b1, matrix.b2, matrix.b3, matrix.b4,
            matrix.c1, matrix.c2, matrix.c3, matrix.c4,
            matrix.d1, matrix.d2, matrix.d3, matrix.d4
    };

    glm::mat4 glmMat4 = glm::make_mat4(mat);
    /* Assmip is row major while glm is column major
     * */
    transpose(glmMat4);

    return glmMat4;
}

std::string assimpToStdString(const aiString &assimpString) {
    return std::__cxx11::string(assimpString.C_Str());
}