//
// Created by dominik on 23.04.19.
//

#include "MeshLoader.h"

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>

#include "AssimpConversion.h"

void MeshLoader::loadBasicMeshInfo(const aiMesh *aMesh) {
    assimpMesh = aMesh;
    constructedMesh = std::make_shared<Mesh>();

    constructedMesh->name = assimpToEngine(aMesh->mName);
    constructedMesh->matId = aMesh->mMaterialIndex;

    for(unsigned int vertIdx = 0; vertIdx < assimpMesh->mNumVertices; vertIdx++){

        aiVector3D pos = assimpMesh->mVertices[vertIdx];
        constructedMesh->positions.emplace_back(
                pos.x,
                pos.y,
                pos.z,
                1
        );

        aiVector3D normals = assimpMesh->mNormals[vertIdx];

        constructedMesh->normals.emplace_back(
                normals.x,
                normals.y,
                normals.z,
                0
        );


        if(assimpMesh->HasTextureCoords(0)){
            aiVector3D uvs = assimpMesh->mTextureCoords[0][vertIdx];
            /* OpenGl describes textures startign from left down corner while most
             * file formats start from left upper corner. Flipping the coordinates is
             * faster than flipping actual images.
             * */
            constructedMesh->uv.emplace_back(
                    uvs.x,
                    1 - uvs.y
            );
        } else {
            constructedMesh->uv.emplace_back(
                    0,
                    0
            );
        }

        if(assimpMesh->HasVertexColors(0)){
            aiColor4D color = assimpMesh->mColors[0][vertIdx];
            constructedMesh->colors.emplace_back(
                    color.r,
                    color.g,
                    color.b,
                    color.a
            );
        }
        else {
            constructedMesh->colors.emplace_back(
                    1,
                    0,
                    0,
                    1
            );
        }
    }

    for (unsigned int f = 0; f<assimpMesh->mNumFaces; f++)
    {
        aiFace face = assimpMesh->mFaces[f];
        constructedMesh->indicies.push_back(static_cast<unsigned short &&>(face.mIndices[0]));
        constructedMesh->indicies.push_back(static_cast<unsigned short &&>(face.mIndices[1]));
        constructedMesh->indicies.push_back(static_cast<unsigned short &&>(face.mIndices[2]));
    }
}


void MeshLoader::addBoneInfo(const std::map<std::__cxx11::string, int> &boneNameToIndexMap) {
    if(isMeshInitialised() == false){
        throw MeshLoadingException("Tried to add per vertex bone info to mesh before adding basic vertex info.");
    }
    /* At this point Model Loader has already filled the
     * basic mesh info (positions, color, etc...). Bone info
     * will be added in a very chaotic way (because of the way
     * assimp stores bone weights) therefore we need resize boneInfo
     * vectors right now (to avoid SIGABRT :<).
     * */
    auto vertexCount = constructedMesh->positions.size();
    assert(vertexCount!=0);
    constructedMesh->boneIds.resize(vertexCount, glm::ivec4(0,0,0,0));
    constructedMesh->boneWeights.resize(vertexCount, glm::vec4(0.f,0.f,0.f,0.f));
    /* For each bone in mesh;
     * */
    for(int i = 0; i < assimpMesh->mNumBones; i++){
        aiBone* bone = assimpMesh->mBones[i];
        std::string boneName = assimpToEngine(bone->mName);
        auto iteratorToBone = boneNameToIndexMap.find(boneName);
        int boneIdx = iteratorToBone->second;
        /* For each vertex the bone affects
         * */
        for(int j = 0; j < bone->mNumWeights; j++){
            aiVertexWeight weight =  bone->mWeights[j];
            constructedMesh->addBoneData(
                    weight.mVertexId,
                    boneIdx,
                    weight.mWeight
            );
        }
    }
}


std::shared_ptr<Mesh> MeshLoader::make() {
    if( isMeshInitialised()==false ){
        throw MeshLoadingException("Tried to return an invalid (empty) mesh");
    }
    std::shared_ptr returnPtr = constructedMesh;
    constructedMesh = nullptr;
    return returnPtr;
}

MeshLoader::MeshLoader() {}