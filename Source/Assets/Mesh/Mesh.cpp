//
// Created by dominik on 23.02.19.
//

#include "Mesh.h"

#include "Assets/Textures/Texture.h"
#include "MyExceptions.h"
#include "Assets/Textures/MaterialsLoader.h"


Mesh::Mesh(const std::string &name,
           unsigned int matId,
           const std::vector<glm::vec3> &positions,
           const std::vector<glm::vec4> &colors,
           const std::vector<glm::vec2> &uvs,
           const std::vector<glm::vec3> &normals,
           const std::vector<glm::vec3> &tangents,
           const std::vector<glm::ivec4> &boneIds,
           const std::vector<glm::vec4> &boneWeights,
           const std::vector<GLushort> &indicies
           ) : name(name),matId(matId)
{
    vao.bind();

    hasColors = !colors.empty();
    hasUvs = !uvs.empty();
    hasBones = !boneIds.empty();
    hasTangents = !tangents.empty();

    positionsB.bufferData(positions);
    normalsB.bufferData(normals);

    if (hasColors)
        colorsB.bufferData(colors);

    if (hasUvs)
        uvB.bufferData(uvs);

    if (hasTangents)
        tangentsB.bufferData(tangents);

    if (hasBones)
    {
        boneIdsB.bufferData(boneIds);
        boneWeightsB.bufferData(boneWeights);
    }
    indiciesB.bind();
    indiciesB.sendIfVaoEnabled(indicies);
    indiciesCount = static_cast<unsigned int>(indicies.size());


    if (hasUvs && hasColors)
        unbindColors();
    vao.unbind();
}

void Mesh::bindVao()
{
    vao.bind();
}

unsigned int Mesh::getIndiciesCount() const
{
    return indiciesCount;
}

void Mesh::bindPositon()
{
    positionsB.bind(vao.getVaoName());
}

void Mesh::bindNormals()
{
    normalsB.bind(vao.getVaoName());
}

void Mesh::bindTangents()
{
    tangentsB.bind(vao.getVaoName());
}

void Mesh::bindColors()
{
    colorsB.bind(vao.getVaoName());
}

void Mesh::bindUvs()
{
    uvB.bind(vao.getVaoName());
}

void Mesh::bindBoneInfo()
{
    boneIdsB.bind(vao.getVaoName());
    boneWeightsB.bind(vao.getVaoName());
}

void Mesh::unbindPositon()
{
    positionsB.unbind(vao.getVaoName());
}

void Mesh::unbindNormals()
{
    normalsB.unbind(vao.getVaoName());
}

void Mesh::unbindTangents()
{
    tangentsB.unbind(vao.getVaoName());
}

void Mesh::unbindColors()
{
    colorsB.unbind(vao.getVaoName());
}

void Mesh::unbindUvs()
{
    uvB.unbind(vao.getVaoName());
}

void Mesh::unbindBoneInfo()
{
    boneIdsB.unbind(vao.getVaoName());
    boneWeightsB.unbind(vao.getVaoName());
}

unsigned int Mesh::getMatId() const
{
    return matId;
}

