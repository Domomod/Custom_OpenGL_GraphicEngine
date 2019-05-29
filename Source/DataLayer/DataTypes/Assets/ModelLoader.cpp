//
// Created by dominik on 28.03.19.
//

#include "ModelLoader.h"

#include <map>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Model.h"
#include "Source/DataLayer/DataTypes/Assets/Mesh/MeshLoader.h"
#include "Source/DataLayer/DataTypes/Assets/Textures/MaterialsLoader.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletonLoader.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimation.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimator.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletonAnimationLoader.h"

#include "Utility.h"
#include "Source/MyExceptions.h"


bool        ModelLoader::hasSkeleton = false;
std::string ModelLoader::directory;

MeshLoader      ModelLoader::meshLoader;
MaterialsLoader ModelLoader::materialsLoader;

SkeletalSystem::SkeletonLoader          ModelLoader::skeletonLoader;
SkeletalSystem::SkeletonAnimationLoader ModelLoader::animationLoader;

const aiScene   *ModelLoader::scene;
Assimp::Importer ModelLoader::importer;


std::shared_ptr<Model> ModelLoader::loadModel( const std::string &path ) {
    std::shared_ptr<Model> thisModel = std::make_shared<Model>();

    directory = getFileDir(path);

    scene = importer.ReadFile(path.c_str(),
                              aiProcess_CalcTangentSpace |
                              aiProcess_GenSmoothNormals |
                              aiProcess_JoinIdenticalVertices |
                              aiProcess_ImproveCacheLocality |
                              aiProcess_LimitBoneWeights |
                              aiProcess_RemoveRedundantMaterials |
                              aiProcess_SplitLargeMeshes |
                              aiProcess_Triangulate |
                              aiProcess_GenUVCoords |
                              aiProcess_SortByPType |
                              aiProcess_FindDegenerates |
                              aiProcess_FindInvalidData |
                              aiProcess_FindInstances |
                              aiProcess_ValidateDataStructure |
                              aiProcess_OptimizeMeshes |
                              aiProcess_Debone |
                              0
    );

    if (!scene) {
        throw ModelLoadingException("Could not load file: " + path);
    }

    auto debugInsightScene = scene;

    loadSkeleton(thisModel);
    loadSkeletalAnimations(thisModel);
    loadMaterials();
    loadMeshes(thisModel);

    return thisModel;
}

void ModelLoader::loadMeshes(const std::shared_ptr<Model> &thisModel) {
    for(unsigned int idx = 0; idx < scene->mNumMeshes; idx++) {
        aiMesh *assimpMesh = scene->mMeshes[idx];
        meshLoader.loadBasicMeshInfo(assimpMesh);
        if(hasSkeleton)
            meshLoader.addBoneInfo(skeletonLoader.getBoneNameToboneIdMap());

        meshLoader.addNormalTextures(    materialsLoader.normalMaps    );
        meshLoader.addAOTextures(        materialsLoader.ambientMaps   );
        meshLoader.addBaseColorTexture(  materialsLoader.albedoMaps    );
        meshLoader.addMetallnessTexture( materialsLoader.metalnessMaps );
        meshLoader.addRoughnessTexture(  materialsLoader.roughnessMaps );

        thisModel->meshes.push_back(meshLoader.make());
    }
}

void ModelLoader::loadSkeletalAnimations(const std::shared_ptr<Model> &thisModel) {
    if(hasSkeleton) {
        for (unsigned int idx = 0; idx < scene->mNumAnimations; idx++) {
            animationLoader.loadAnimation(scene->mAnimations[idx],
                                          skeletonLoader.getBoneNameToboneIdMap());
            thisModel->skeletalAnimations.push_back(animationLoader.make());
        }
    }
}

void ModelLoader::loadSkeleton(const std::shared_ptr<Model> &thisModel) {
    skeletonLoader.setScene(scene);
    skeletonLoader.loadSkeleton(scene->mMeshes, scene->mNumMeshes);
    hasSkeleton = skeletonLoader.isSkeletonInitialised();
    thisModel->skeleton = skeletonLoader.make();
    if(hasSkeleton)
        thisModel->animator = std::make_shared<SkeletalSystem::SkeletalAnimator>(thisModel->skeleton);
}

void ModelLoader::loadMaterials() {
    materialsLoader.setScene(scene);
    materialsLoader.setDirectory(directory);
    materialsLoader.loadMaterials();
}

