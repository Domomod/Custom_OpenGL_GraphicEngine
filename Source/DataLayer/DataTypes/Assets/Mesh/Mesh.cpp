//
// Created by dominik on 23.02.19.
//

#include "Mesh.h"

#include "Source/MyExceptions.h"


void Mesh::addBoneData(int vertId, int boneId, float weight) {
    /* Each vertex might be affected by up to 4.
     * */
    for(int i = 0; i < 4; i ++){
        if(boneIds[vertId][i] == 0 ){
            boneIds[vertId][i] = boneId;
            boneWeights[vertId][i] = weight;
            return;
        }
    }
    throw MeshLoadingException("Vertex affected by more than 4 bones.");
}

Mesh::Mesh(const std::string &name,
           const std::vector<glm::vec3> &positions,   const std::vector<glm::vec4 > &colors,
           const std::vector<glm::vec2> &uv,          const std::vector<glm::vec3 > &normals,
           const std::vector<glm::vec3> &tangents,    const std::vector<glm::ivec4> &boneIds,
           const std::vector<glm::vec4> &boneWeights, const std::vector<GLushort> &indicies,

           const std::shared_ptr<Texture> &normalMap, const std::shared_ptr<Texture> &aoMap,
           const std::shared_ptr<Texture> &albedoMap, const std::shared_ptr<Texture> &metallnessMap,
           const std::shared_ptr<Texture> &roughnessMap):  positions(positions), colors(colors), uv(uv),
                                                           normals(normals), tangents(tangents), boneIds(boneIds),
                                                           boneWeights(boneWeights), indicies(indicies), name(name),
                                                           normalMap(normalMap), aoMap(aoMap), albedoMap(albedoMap),
                                                           metallnessMap(metallnessMap), roughnessMap(roughnessMap) {}
