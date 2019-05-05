//
// Created by dominik on 28.03.19.
//

#include "ModelLoader.h"

#include <map>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Source/MyExceptions.h"
#include "MeshLoader.h"
#include "AssimpConversion.h"
#include "Source/DataLayer/DataTypes/Assets/Textures/TextureLoader.h"

#include "Utility.h"

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
    loadMeshes(thisModel);
    loadMaterials(thisModel);
    assignMaterials(thisModel);

    return thisModel;
}

void ModelLoader::loadMeshes(const std::shared_ptr<Model> &thisModel) {
    for(unsigned int idx = 0; idx < scene->mNumMeshes; idx++) {
        aiMesh *assimpMesh = scene->mMeshes[idx];
        meshLoader.loadBasicMeshInfo(assimpMesh);
        if(hasSkeleton)
            meshLoader.addBoneInfo(skeletonLoader.getBoneNameToboneIdMap());
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

void ModelLoader::loadMaterials(const std::shared_ptr<Model> &thisModel) {
    for(int idx = 0; idx < scene->mNumMaterials; idx++){
        aiMaterial* material = scene->mMaterials[idx];
        if(material->GetTextureCount(aiTextureType_DIFFUSE)){
            aiString path;
            if(AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0,
                                                  &path, nullptr, nullptr, nullptr, nullptr, nullptr)){
                std::string stdPath = assimpToEngine(path);
                std::string fullPath = directory;
#ifdef __linux__
                fullPath += "/";
#elif _WIN32
                fullPath += "\\";
#else
#error "OS not supported"
#endif
                fullPath += stdPath;

                thisModel->diffuseTextures.push_back(TextureLoader::loadTexture(fullPath));
            } else {
                /* If model does not have this texture load default texture
                * */
                thisModel->diffuseTextures.push_back(TextureLoader::getDefaultTexture());
            }
        } else {
            /* If model does not have this texture load default texture
            * */
            thisModel->diffuseTextures.push_back(TextureLoader::getDefaultTexture());
        }
    }
}

void ModelLoader::assignMaterials(const std::shared_ptr<Model> &thisModel) {
    if(thisModel->meshes.size() < 1){
        throw ModelLoadingException("Tried to assign materials to meshes before loading meshes.");
    }
    if(thisModel->diffuseTextures.size() < 1){
        throw ModelLoadingException("Tried to assign materials to meshes before loading materials.");
    }

    /* Assign materials to meshes
     * */
    for(auto& mesh : thisModel->meshes){
        mesh->diffuse = thisModel->diffuseTextures[mesh->matId];
    }
}


