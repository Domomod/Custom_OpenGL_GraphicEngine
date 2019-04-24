//
// Created by dominik on 23.04.19.
//

#ifndef GAMEENGINE_ASSIMPCONVERSION_H
#define GAMEENGINE_ASSIMPCONVERSION_H

#include <glm/glm.hpp>
#include <string>
#include <assimp/scene.h>

glm::mat4 assimpToEngine(aiMatrix4x4 matrix);

std::string assimpToEngine(const aiString &assimpString);

glm::quat assimpToEngine(aiQuaternion assimpQuaternion);;

glm::vec4 assimpToEngine(aiVector3D vector3d, float fourthValue);

glm::vec3 assimpToEngine(aiVector3D vector3d);

#endif //GAMEENGINE_ASSIMPCONVERSION_H
