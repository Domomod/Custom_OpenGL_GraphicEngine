//
// Created by dominik on 23.04.19.
//

#ifndef GAMEENGINE_ASSIMPCONVERSION_H
#define GAMEENGINE_ASSIMPCONVERSION_H

#include <glm/glm.hpp>
#include <string>
#include <assimp/scene.h>
#include <glm/gtc/type_ptr.hpp>


std::string assimpToEngine(const aiString &assimpString);

static inline glm::vec4 glmCast(const aiVector3D &v, float fourthValue) { return glm::vec4(v.x, v.y, v.z, fourthValue); }
static inline glm::vec3 glmCast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
static inline glm::vec2 glmVec2Cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); } // it's aiVector3D because assimp's texture coordinates use that
static inline glm::quat glmCast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
static inline glm::mat4 glmCast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }

#endif //GAMEENGINE_ASSIMPCONVERSION_H
