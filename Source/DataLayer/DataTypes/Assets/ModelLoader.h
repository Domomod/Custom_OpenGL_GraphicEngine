//
// Created by dominik on 28.03.19.
//

#ifndef GAMEENGINE_MODELLOADER_H
#define GAMEENGINE_MODELLOADER_H

#include <memory>
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



/* Assimp is able to load whole scenes containing hierarchy of meshes etc ... (formats 3ds, collada)
 * I decided the Game Engine will only load single Models, and leave the scene making
 * to the engine. Therefore my engine is assuming loaded model is a single model.
 * */
class ModelLoader {
public:
    std::shared_ptr<Model> loadModel(const std::string &path);

    ModelLoader& loadEmbededMaterials(){
        usingEmbededMaterials = true;
        usingSpecifiedMaterial = false;
        return *this;
    }
    ModelLoader& loadSpecificMaterial(const std::string& path){
        usingEmbededMaterials = false;
        usingSpecifiedMaterial = true;
        materialPath = path;
        return *this;
    }

private:
    const aiScene *scene;
    bool hasSkeleton;
    std::string directory;

    bool usingEmbededMaterials = false;
    bool usingSpecifiedMaterial = false;
    std::string materialPath;

    MeshLoader meshLoader;
    SkeletalSystem::SkeletonLoader skeletonLoader;
    SkeletalSystem::SkeletonAnimationLoader animationLoader;
    MaterialsLoader materialsLoader;

    Assimp::Importer importer;

    void loadSkeleton(const std::shared_ptr<Model> &thisModel);

    void loadSkeletalAnimations(const std::shared_ptr<Model> &thisModel);

    void loadMeshes(const std::shared_ptr<Model> &thisModel);

    void loadMaterials();
};



#endif //GAMEENGINE_MODELLOADER_H