//
// Created by dominik on 28.03.19.
//

#include "MeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Source/MyExceptions.h"

std::shared_ptr<Mesh> MeshLoader::loadMesh(const std::string &path) {
    std::shared_ptr<Mesh> returnMesh = std::make_shared<Mesh>();

    //Using imporeter because it will handle resource cleaning.
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path.c_str(),
        aiProcess_CalcTangentSpace          |
        aiProcess_GenSmoothNormals          |
        aiProcess_JoinIdenticalVertices     |
        aiProcess_ImproveCacheLocality      |
        aiProcess_LimitBoneWeights          |
        aiProcess_RemoveRedundantMaterials  |
        aiProcess_SplitLargeMeshes          |
        aiProcess_Triangulate               |
        aiProcess_GenUVCoords               |
        aiProcess_SortByPType               |
        aiProcess_FindDegenerates           |
        aiProcess_FindInvalidData           |
        aiProcess_FindInstances             |
        aiProcess_ValidateDataStructure     |
        aiProcess_OptimizeMeshes            |
        aiProcess_Debone                    |
        0
        );

    if(!scene){
        throw MeshLoadingException("Could not load file: " + path);
    }

    //At this moment we are only loading single meshes
    aiMesh* mesh = scene->mMeshes[0];

    for(unsigned int vertIdx = 0; vertIdx < mesh->mNumVertices; vertIdx++){
        aiVector3D pos = mesh->mVertices[vertIdx];
        returnMesh->positions.emplace_back(
                pos.x,
                pos.y,
                pos.z,
                1
                );

        aiVector3D normals = mesh->mNormals[vertIdx];

        returnMesh->normals.emplace_back(
                normals.x,
                normals.y,
                normals.z,
                0
                );

        if(mesh->HasTextureCoords(0)){
            aiVector3D uvs = mesh->mTextureCoords[0][vertIdx];
            returnMesh->uv.emplace_back(
                    uvs.x,
                    uvs.y
                    );
        } else {
            returnMesh->uv.emplace_back(
                    0,
                    0
            );
        }

        if(mesh->HasVertexColors(0)){
            aiColor4D color = mesh->mColors[0][vertIdx];
            returnMesh->colors.emplace_back(
                    color.r,
                    color.g,
                    color.b,
                    color.a
                    );
        }
        else {
            returnMesh->colors.emplace_back(
                    1,
                    0,
                    0,
                    1
            );
        }
    }

    for (unsigned int f = 0; f<mesh->mNumFaces; f++)
    {
        aiFace face = mesh->mFaces[f];
        returnMesh->indicies.push_back(static_cast<unsigned short &&>(face.mIndices[0]));
        returnMesh->indicies.push_back(static_cast<unsigned short &&>(face.mIndices[1]));
        returnMesh->indicies.push_back(static_cast<unsigned short &&>(face.mIndices[2]));
    }

    return returnMesh;
}
