//
// Created by dominik on 23.04.19.
//

#ifndef GAMEENGINE_ASSIMPCONVERSION_H
#define GAMEENGINE_ASSIMPCONVERSION_H

#include <glm/glm.hpp>
#include <string>
#include <assimp/scene.h>

glm::mat4 assmipMatToGlmMat(aiMatrix4x4 matrix);

/* Assimp glm conversion */
std::string assimpToStdString(const aiString &assimpString);

#include <memory>
#include <assimp/scene.h>
#include "Model.h"

#endif //GAMEENGINE_ASSIMPCONVERSION_H
