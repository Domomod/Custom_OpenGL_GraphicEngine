//
// Created by dominik on 28.03.19.
//

#include "ModelLoader.h"

#include <map>

#include "Utility.h"
#include "MyExceptions.h"
#include "myXML.h"
#include "UsedDirectories.h"

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != tinyxml2::XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif

std::shared_ptr<Model> ModelLoader::loadModel(const std::string &path)
{
    using namespace tinyxml2;

    hasSkeleton = false;

    XMLDocument documentXML;
    XMLError resultXML = documentXML.LoadFile((MODEL_DIRECTORY + path).c_str());
    XMLCheckResult(resultXML);

    XMLElement * modelXML = documentXML.RootElement();

    std::string modelName = modelXML->Attribute("name");

    XMLElement * geometry = modelXML->FirstChildElement("geometry");

    scene = importer.ReadFile( (GEOMETRY_DIRECTORY + elementTextToString(geometry)).c_str(),
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
        throw ModelLoadingException("Could not load file: " + (GEOMETRY_DIRECTORY + elementTextToString(geometry)));
    }

    auto debugInsightScene = scene;


    model = std::make_shared<Model>();

    loadSkeleton();
    loadSkeletalAnimations();

    loadMeshes();

    XMLElement * firstMaterialAlias = modelXML->FirstChildElement("alias");
    loadMaterials(firstMaterialAlias);

    materialsLoader.clear();

    return model;
}

void ModelLoader::loadMeshes()
{
    for (unsigned int idx = 0; idx < scene->mNumMeshes; idx++)
    {
        aiMesh *assimpMesh = scene->mMeshes[idx];
        meshLoader.loadBasicMeshInfo(assimpMesh);
        if (hasSkeleton)
            meshLoader.addBoneInfo(skeletonLoader.getBoneNameToboneIdMap());

        model->meshes.push_back(meshLoader.make());
    }
}

void ModelLoader::loadSkeletalAnimations()
{
    if (hasSkeleton)
    {
        for (unsigned int idx = 0; idx < scene->mNumAnimations; idx++)
        {
            animationLoader.loadAnimation(scene->mAnimations[idx],
                                          skeletonLoader.getBoneNameToboneIdMap());
            model->skeletalAnimations.push_back(animationLoader.make());
        }
    }
}

void ModelLoader::loadSkeleton()
{
    skeletonLoader.setScene(scene);
    skeletonLoader.loadSkeleton(scene->mMeshes, scene->mNumMeshes);
    hasSkeleton = skeletonLoader.isSkeletonInitialised();
    model->skeleton = skeletonLoader.make();
    if (hasSkeleton)
        model->animator = std::make_shared<SkeletalSystem::SkeletalAnimator>(model->skeleton);
}

void ModelLoader::loadMaterials(tinyxml2::XMLElement *materialAlias)
{
    while(materialAlias != nullptr){
        materialsLoader.loadMaterials(materialAlias);
        model->metrialAliasses.push_back(materialsLoader.getMaterials());
        materialsLoader.clear();
        materialAlias = materialAlias->NextSiblingElement("alias");
    }
}

