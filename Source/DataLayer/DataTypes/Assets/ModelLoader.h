//
// Created by dominik on 28.03.19.
//

#ifndef GAMEENGINE_MODELLOADER_H
#define GAMEENGINE_MODELLOADER_H

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <map>

class Model;
class MeshLoader;
class MaterialsLoader;
class SkeletonLoader;

namespace SkeletalSystem{
    class SkeletonAnimationLoader;
}

/* Assimp is able to load whole scenes containing hierarchy of meshes etc ... (formats 3ds, collada)
 * I decided the Game Engine will only load single Models, and leave the scene making
 * to the engine. Therefore my engine is assuming loaded model is a single model.
 * */
class ModelLoader {
public:
    static std::shared_ptr<Model> loadModel(const std::string &path);
private:
    inline static const aiScene *scene;
    inline static bool hasSkeleton = false;
    inline static std::string directory;

    static MeshLoader meshLoader;
    static SkeletonLoader skeletonLoader;
    static SkeletalSystem::SkeletonAnimationLoader animationLoader;
    static MaterialsLoader materialsLoader;


    /*Using imporeter because it will handle resource cleaning.
    */
    inline static Assimp::Importer importer;

    static void loadSkeleton(const std::shared_ptr<Model> &thisModel);

    static void loadSkeletalAnimations(const std::shared_ptr<Model> &thisModel);

    static void loadMeshes(const std::shared_ptr<Model> &thisModel);

    static void loadMaterials();
};



#endif //GAMEENGINE_MODELLOADER_H