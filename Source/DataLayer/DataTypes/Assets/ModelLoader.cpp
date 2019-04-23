//
// Created by dominik on 28.03.19.
//

#include "ModelLoader.h"

#include <map>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Source/MyExceptions.h"
#include "MeshLoader.h"
#include "SkeletonLoader.h"


std::shared_ptr<Model> ModelLoader::loadModel(const std::string &path) {
    std::shared_ptr<Model> thisModel = std::make_shared<Model>();

    //Using imporeter because it will handle resource cleaning.
    Assimp::Importer importer;

    scene = importer.ReadFile(path.c_str(),
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
        throw ModelLoadingException("Could not load file: " + path);
    }

    /*TODO: for every mesh in assimp scene load it to model*/
    aiMesh* assimpMesh = scene->mMeshes[0];
    auto meshLoader = MeshLoader(scene);
    auto skeletonLoader = SkeletonLoader(scene);


    meshLoader.loadBasicMeshInfo(assimpMesh);

    if(assimpMesh->HasBones()){
        skeletonLoader.loadSkeleton(assimpMesh);

        meshLoader.addBoneInfo( skeletonLoader.getBoneNameToboneIdMap() );

        thisModel->skeleton = skeletonLoader.make();
    }

    thisModel->mesh = meshLoader.make();


    return thisModel;
}


