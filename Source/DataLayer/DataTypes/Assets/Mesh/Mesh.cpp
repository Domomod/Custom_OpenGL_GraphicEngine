//
// Created by dominik on 23.02.19.
//

#include "Mesh.h"

#include "Source/DataLayer/DataTypes/Assets/Textures/Texture.h"
#include "Source/MyExceptions.h"


Mesh::Mesh(const std::string &name,
           const std::vector<glm::vec3> &positions,   const std::vector<glm::vec4 > &colors,
           const std::vector<glm::vec2> &uvs,          const std::vector<glm::vec3 > &normals,
           const std::vector<glm::vec3> &tangents,    const std::vector<glm::ivec4> &boneIds,
           const std::vector<glm::vec4> &boneWeights, const std::vector<GLushort> &indicies,

           const std::shared_ptr<Texture> &normalMap, const std::shared_ptr<Texture> &aoMap,
           const std::shared_ptr<Texture> &albedoMap, const std::shared_ptr<Texture> &metallnessMap,
           const std::shared_ptr<Texture> &roughnessMap):  name(name),
                                                           normalMap(normalMap), aoMap(aoMap), albedoMap(albedoMap),
                                                           metallnessMap(metallnessMap), roughnessMap(roughnessMap) {

    vao.bind();

    positionsB.bufferData(positions);
    colorsB.bufferData(colors);
    uvB.bufferData(uvs);
    normalsB.bufferData(normals);
    tangentsB.bufferData(tangents);
    boneIdsB.bufferData(boneIds);
    boneWeightsB.bufferData(boneWeights);
    indiciesB.bind();
    indiciesB.sendIfVaoEnabled(indicies);
    indiciesCount = static_cast<unsigned int>(indicies.size());

    hasColors = !colors.empty();
    hasUvs    = !uvs.empty();
    hasBones  = !boneIds.empty();

    if(hasUvs && hasColors)
        unbindColors();
    vao.unbind();
}

unsigned int Mesh::getIndiciesCount() const {
    return indiciesCount;
}

void Mesh::bindTexturesPBR() {
    albedoMap->bind(0);
    aoMap->bind(1);
    metallnessMap->bind(2);
    roughnessMap->bind(3);
    normalMap->bind(4);
}
