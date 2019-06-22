//
// Created by dominik on 29.05.19.
//

#ifndef GAMEENGINE_MESHFACTORY_H
#define GAMEENGINE_MESHFACTORY_H

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

#include "Assets/Textures/Texture.h"
#include "Mesh.h"

class MeshFactory
{
public:
    MeshFactory &addPosition(glm::vec3 p);

    MeshFactory &addColor(glm::vec4 c);

    MeshFactory &addUv(glm::vec2 u);

    MeshFactory &addNormal(glm::vec3 n);

    MeshFactory &addTangent(glm::vec3 t);

    MeshFactory &allocateMemoryForBoneData();

    MeshFactory &addBoneData(int vertId, int boneId, float weight);

    MeshFactory &addFace(unsigned short i1,
                         unsigned short i2,
                         unsigned short i3);

    std::shared_ptr<Mesh> make(const std::string &name)
    {
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
                name,
                matID,
                positions,
                colors,
                uv,
                normals,
                tangents,
                boneIds,
                boneWeights,
                indicies
        );
        clear();
        return mesh;
    }

    void setMatID(unsigned int matID);

private:
    void clear()
    {
        positions.clear();
        colors.clear();
        uv.clear();
        normals.clear();
        tangents.clear();
        boneIds.clear();
        boneWeights.clear();
        indicies.clear();
    };

    std::vector<glm::vec3> positions;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::ivec4> boneIds;
    std::vector<glm::vec4> boneWeights;
    std::vector<GLushort> indicies;

    std::string name;
    unsigned int matID;
};


#endif //GAMEENGINE_MESHFACTORY_H
