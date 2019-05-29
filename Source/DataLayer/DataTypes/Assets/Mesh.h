// Created by dominik on 23.02.19.
//

#ifndef GAMEENGINE_MESH_H
#define GAMEENGINE_MESH_H

#include <vector>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <memory>

class Texture;

class Mesh {
public:
    Mesh() = default;
    std::string name;

    Mesh(const std::string &name,
         const std::vector<glm::vec3> &positions,   const std::vector<glm::vec4 > &colors,
         const std::vector<glm::vec2> &uv,          const std::vector<glm::vec3 > &normals,
         const std::vector<glm::vec3> &tangents,    const std::vector<glm::ivec4> &boneIds,
         const std::vector<glm::vec4> &boneWeights, const std::vector<GLushort> &indicies,

         const std::shared_ptr<Texture> &normalMap, const std::shared_ptr<Texture> &aoMap,
         const std::shared_ptr<Texture> &albedoMap, const std::shared_ptr<Texture> &metallnessMap,
         const std::shared_ptr<Texture> &roughnessMap);


    std::vector<glm::vec3>  positions;
    std::vector<glm::vec4>  colors;
    std::vector<glm::vec2>  uv;
    std::vector<glm::vec3>  normals;
    std::vector<glm::vec3>  tangents;
    std::vector<glm::ivec4> boneIds;
    std::vector<glm::vec4>  boneWeights;
    std::vector<GLushort>   indicies;

    std::shared_ptr<Texture> normalMap;
    std::shared_ptr<Texture> aoMap;
    std::shared_ptr<Texture> albedoMap;
    std::shared_ptr<Texture> metallnessMap;
    std::shared_ptr<Texture> roughnessMap;

    bool hasColors;
    bool hasUvs;
    bool hasNormals;
    bool hasBones;

    void addBoneData(int vertId, int boneId, float weight);
};


#endif //GAMEENGINE_MESH_H