// Created by dominik on 23.02.19.
//

#ifndef GAMEENGINE_MESH_H
#define GAMEENGINE_MESH_H

#include <vector>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <memory>

#include "Source/DataLayer/DataTypes/Assets/Textures/Texture.h"

class Mesh {
public:
    std::vector<glm::vec4>  positions;
    std::vector<glm::vec4>  colors;
    std::vector<glm::vec2>  uv;
    std::vector<glm::vec4>  normals;
    std::vector<glm::ivec4> boneIds;
    std::vector<glm::vec4>  boneWeights;
    std::vector<GLushort>   indicies;

    std::string name;

    std::shared_ptr<Texture> normalMap;
    std::shared_ptr<Texture> AOMap;
    std::shared_ptr<Texture> albedoMap;
    std::shared_ptr<Texture> metallnessMap;
    std::shared_ptr<Texture> roughnessMap;

    bool hasColors;
    bool hasUvs;
    bool hasNormals;
    bool hasBones;

    void addBoneData(int vertId, int boneId, float weight);

    Mesh() = default;
};


#endif //GAMEENGINE_MESH_H
