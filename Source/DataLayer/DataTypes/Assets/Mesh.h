//kaczmarski rejtan        Source/DataLayer/DataTypes/Assets/Textures/Model.cpp Source/DataLayer/DataTypes/Assets/Textures/Model.h
// Created by dominik on 23.02.19.
//

#ifndef GAMEENGINE_MESH_H
#define GAMEENGINE_MESH_H

#include <vector>
#include <GL/gl3w.h>
#include <glm/glm.hpp>

class Mesh {
public:
    std::vector<glm::vec4>  positions;
    std::vector<glm::vec4>  colors;
    std::vector<glm::vec2>  uv;
    std::vector<glm::vec4>  normals;
    std::vector<glm::ivec4> boneIds;
    std::vector<glm::vec4>  boneWeights;
    std::vector<GLushort>   indicies;

    bool hasColors;
    bool hasUvs;
    bool hasNormals;
    bool hasBones;

    void addBoneData(int vertId, int boneId, float weight);

    Mesh() = default;
};


#endif //GAMEENGINE_MESH_H
