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
#include "Assets/Mesh/MeshLoader.h"
#include "Assets/Textures/MaterialsLoader.h"
#include "Assets/SkeletalSystem/SkeletonLoader.h"
#include "Assets/SkeletalSystem/SkeletalAnimation.h"
#include "Assets/SkeletalSystem/SkeletalAnimator.h"
#include "Assets/SkeletalSystem/SkeletonAnimationLoader.h"



/* Assimp is able to load whole scenes containing hierarchy of meshes etc ... (formats 3ds, collada)
 * I decided the Game Engine will only load single Models, and leave the scene making
 * to the engine. Therefore my engine is assuming loaded model is a single model.
 * */
class ModelLoader {
public:
    std::shared_ptr<Model> loadModel(const std::string &path);

private:
    std::shared_ptr<Model> model;

    const aiScene *scene;
    bool hasSkeleton;

    MeshLoader meshLoader;
    SkeletalSystem::SkeletonLoader skeletonLoader;
    SkeletalSystem::SkeletonAnimationLoader animationLoader;
    MaterialsLoader materialsLoader;

    Assimp::Importer importer;

    void loadSkeleton();

    void loadSkeletalAnimations();

    void loadMeshes();

    void loadMaterials(tinyxml2::XMLElement *materialAlias);
};



#endif //GAMEENGINE_MODELLOADER_H