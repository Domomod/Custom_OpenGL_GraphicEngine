//
// Created by dominik on 28.03.19.
//

#include "ModelLoader.h"

#include <map>

#include "Utility.h"
#include "MyExceptions.h"


std::shared_ptr<Model> ModelLoader::loadModel(const std::string &path)
{
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

    if (!scene)
    {
        throw ModelLoadingException("Could not load file: " + path);
    }

    auto debugInsightScene = scene;

    loadSkeleton(thisModel);
    loadSkeletalAnimations(thisModel);

    loadMaterials();
    loadMeshes(thisModel);

    usingEmbededMaterials = false;
    usingSpecifiedMaterial = false;

    materialsLoader.clear();

    return thisModel;
}

void ModelLoader::loadMeshes(const std::shared_ptr<Model> &thisModel)
{
    for (unsigned int idx = 0; idx < scene->mNumMeshes; idx++)
    {
        aiMesh *assimpMesh = scene->mMeshes[idx];
        meshLoader.loadBasicMeshInfo(assimpMesh);
        if (hasSkeleton)
            meshLoader.addBoneInfo(skeletonLoader.getBoneNameToboneIdMap());

        if (usingEmbededMaterials)
        {
            meshLoader.addNormalTextures(materialsLoader.normalMaps);
            meshLoader.addAOTextures(materialsLoader.ambientMaps);
            meshLoader.addBaseColorTexture(materialsLoader.albedoMaps);
            meshLoader.addMetallnessTexture(materialsLoader.metalnessMaps);
            meshLoader.addRoughnessTexture(materialsLoader.roughnessMaps);
        }
        else if (usingSpecifiedMaterial)
        {
            meshLoader.addMaterial(materialsLoader, 0);
        }

        thisModel->meshes.push_back(meshLoader.make());
    }
}

void ModelLoader::loadSkeletalAnimations(const std::shared_ptr<Model> &thisModel)
{
    if (hasSkeleton)
    {
        for (unsigned int idx = 0; idx < scene->mNumAnimations; idx++)
        {
            animationLoader.loadAnimation(scene->mAnimations[idx],
                                          skeletonLoader.getBoneNameToboneIdMap());
            thisModel->skeletalAnimations.push_back(animationLoader.make());
        }
    }
}

void ModelLoader::loadSkeleton(const std::shared_ptr<Model> &thisModel)
{
    skeletonLoader.setScene(scene);
    skeletonLoader.loadSkeleton(scene->mMeshes, scene->mNumMeshes);
    hasSkeleton = skeletonLoader.isSkeletonInitialised();
    thisModel->skeleton = skeletonLoader.make();
    if (hasSkeleton)
        thisModel->animator = std::make_shared<SkeletalSystem::SkeletalAnimator>(thisModel->skeleton);
}

void ModelLoader::loadMaterials()
{
    if (usingEmbededMaterials)
    {
        materialsLoader.setScene(scene);
        materialsLoader.setDirectory(directory);
        materialsLoader.loadMaterials();
    }
    else if (usingSpecifiedMaterial)
    {
        materialsLoader.setDirectory(getFileDir(materialPath));
        materialsLoader.loadMaterial(materialPath);
    }
}

