//
// Created by dominik on 23.04.19.
//

#include "MeshLoader.h"

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <sstream>

#include "Mesh.h"
#include "Assets/Model.h"
#include "Assets/AssimpConversion.h"
#include "Assets/Textures/MaterialsLoader.h"
#include "MyExceptions.h"

void MeshLoader::loadBasicMeshInfo(const aiMesh *aMesh)
{
    assimpMesh = aMesh;
    matId = assimpMesh->mMaterialIndex;

    for (unsigned int vertIdx = 0; vertIdx < assimpMesh->mNumVertices; vertIdx++)
    {
        if (!assimpMesh->HasPositions() || !assimpMesh->HasNormals())
        {
            std::stringstream ss;
            ss << "hasPositions : " << std::boolalpha << assimpMesh->HasPositions() << "\n"
               << "hasNormal : " << std::boolalpha << assimpMesh->HasNormals() << "\n";

            throw MeshLoadingException("Loaded mesh is invalid " + assimpToEngine(assimpMesh->mName) + "\n"
                                                                                                       "Mesh state:\n" +
                                       ss.str());
        }
        /*Positions are always included, Normals and Tangents are included or calculated*/
        aiVector3D position = assimpMesh->mVertices[vertIdx];
        aiVector3D normal = assimpMesh->mNormals[vertIdx];

        meshFactory.addPosition(glmCast(position));
        meshFactory.addNormal(glmCast(normal));

        /*Always add an color, because it's cheap*/
        aiColor4D color = assimpMesh->HasVertexColors(0) ? assimpMesh->mColors[0][vertIdx] :
                          aiColor4D(1, 0, 0, 1);
        meshFactory.addColor(glmCast(color));

        /*Decided to not create my own tex coords, they won't have any sense when visualised anyway.
         * Only textured objects have tangents*/
        if (assimpMesh->HasTextureCoords(0))
        {
            aiVector3D uv = assimpMesh->mTextureCoords[0][vertIdx];
            meshFactory.addUv(glmTexCoordCast(uv));

            aiVector3D tangent = assimpMesh->mTangents[vertIdx];
            meshFactory.addTangent(glmCast(tangent));

        }

    }

    for (unsigned int f = 0; f < assimpMesh->mNumFaces; f++)
    {
        aiFace face = assimpMesh->mFaces[f];
        meshFactory.addFace(face.mIndices[0],
                            face.mIndices[1],
                            face.mIndices[2]);
    }
}

void MeshLoader::addBoneInfo(const std::map<std::__cxx11::string, int> &boneNameToIndexMap)
{
    /* At this point Model Loader should have already filled the
     * basic mesh info (positions, color, etc...). If not bone data
     * this stage will be ignored.
     * */
    try
    {
        /*Bone info will be added unordered (because of the way
        * assimp stores bone weights) therefore we need to allocate memory
        * for bone data.
        * */
        meshFactory.allocateMemoryForBoneData();
        /* For each bone in mesh;
         * */
        for (int i = 0; i < assimpMesh->mNumBones; i++)
        {
            aiBone *bone = assimpMesh->mBones[i];
            std::string boneName = assimpToEngine(bone->mName);
            auto iteratorToBone = boneNameToIndexMap.find(boneName);
            int boneIdx = iteratorToBone->second;
            /* For each vertex the bone affects
             * */
            for (int j = 0; j < bone->mNumWeights; j++)
            {
                aiVertexWeight weight = bone->mWeights[j];
                meshFactory.addBoneData(
                        weight.mVertexId,
                        boneIdx,
                        weight.mWeight
                );
            }
        }
    }
    catch (MeshLoadingException &e)
    {
        std::cerr << "Ignored mesh bone data because of an error:\n" << e.what();
    }
}


std::shared_ptr<Mesh> MeshLoader::make()
{
    std::string name = assimpToEngine(assimpMesh->mName);
    return meshFactory.make(name);
}

MeshLoader::MeshLoader()
{
}

void MeshLoader::addNormalTextures(const std::vector<std::shared_ptr<Texture2D>> &textures)
{
    meshFactory.setNormalMap(textures[matId]);
}

void MeshLoader::addBaseColorTexture(const std::vector<std::shared_ptr<Texture2D>> &textures)
{
    meshFactory.setAlbedoMap(textures[matId]);
}

void MeshLoader::addAOTextures(const std::vector<std::shared_ptr<Texture2D>> &textures)
{
    meshFactory.setAoMap(textures[matId]);
}

void MeshLoader::addMetallnessTexture(const std::vector<std::shared_ptr<Texture2D>> &textures)
{
    meshFactory.setMetallnessMap(textures[matId]);
}

void MeshLoader::addRoughnessTexture(const std::vector<std::shared_ptr<Texture2D>> &textures)
{
    meshFactory.setRoughnessMap(textures[matId]);
}

void MeshLoader::addMaterial(const MaterialsLoader &materialLoader, unsigned int matID)
{
    meshFactory.setNormalMap(materialLoader.normalMaps[matID]);
    meshFactory.setAlbedoMap(materialLoader.albedoMaps[matID]);
    meshFactory.setAoMap(materialLoader.ambientMaps[matID]);
    meshFactory.setMetallnessMap(materialLoader.metalnessMaps[matID]);
    meshFactory.setRoughnessMap(materialLoader.roughnessMaps[matID]);
}
