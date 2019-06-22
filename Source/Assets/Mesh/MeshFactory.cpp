//
// Created by dominik on 29.05.19.
//

#include "MeshFactory.h"
#include <iostream>

#include "MyExceptions.h"

MeshFactory &MeshFactory::addPosition(glm::vec3 p)
{
    positions.push_back(p);
    return *this;
}

MeshFactory &MeshFactory::addColor(glm::vec4 c)
{
    colors.push_back(c);
    return *this;
}

MeshFactory &MeshFactory::addUv(glm::vec2 u)
{
    uv.push_back(u);
    return *this;
}

MeshFactory &MeshFactory::addNormal(glm::vec3 n)
{
    normals.push_back(n);
    return *this;
}

MeshFactory &MeshFactory::addTangent(glm::vec3 t)
{
    tangents.push_back(t);
    return *this;
}

MeshFactory &MeshFactory::addFace(unsigned short i1, unsigned short i2, unsigned short i3)
{
    indicies.push_back(i1);
    indicies.push_back(i2);
    indicies.push_back(i3);
    return *this;
}

MeshFactory &MeshFactory::allocateMemoryForBoneData()
{
    if (positions.empty())
    {
        throw MeshLoadingException(
                "Tried to allocate memory for bone data before simple vertex data (positions, colors)"
                "Program does not know how much memory to allocate. Bone data will be ignored."
        );
    }
    boneIds.resize(positions.size());
    return *this;
}

MeshFactory &MeshFactory::addBoneData(int vertId, int boneId, float weight)
{
    if (positions.empty())
    {
        throw MeshLoadingException("Bone data not allocated. Bone data ignored.");
    }
    /* Each vertex might be affected by up to 4.
     * */

    for (int i = 0; i < 4; i++)
    {
        if (boneIds[vertId][i] == 0)
        {
            boneIds[vertId][i] = boneId;
            boneWeights[vertId][i] = weight;
            return *this;
        }
    }
    throw MeshLoadingException("Vertex affected by more than 4 bones.");
}

void MeshFactory::setMatID(unsigned int matID)
{
    MeshFactory::matID = matID;
}
