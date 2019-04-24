//
// Created by dominik on 23.04.19.
//

#include "ModelLoader.h"
#include "AssimpConversion.h"
#include <glm/gtc/type_ptr.hpp>

glm::mat4 assimpToEngine(aiMatrix4x4 matrix) {

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

std::string assimpToEngine(const aiString &assimpString) {
    return std::string(assimpString.C_Str());
}

glm::quat assimpToEngine(aiQuaternion assimpQuaternion) {
    /* For some reason glm initializes quaternions in order  wxyz instead xyzw (as id does with vec4).
     * */
    return glm::quat(assimpQuaternion.w, assimpQuaternion.x, assimpQuaternion.y, assimpQuaternion.z);
}

glm::vec4 assimpToEngine(aiVector3D vector3d, float fourthValue) {
    return glm::vec4(vector3d.x, vector3d.y, vector3d.z, fourthValue);
}

glm::vec3 assimpToEngine(aiVector3D vector3d) {
    return glm::vec3(vector3d.x, vector3d.y, vector3d.z);
}
