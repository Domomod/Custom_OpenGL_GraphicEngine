//
// Created by dominik on 28.03.19.
//

#ifndef GAMEENGINE_MESHLOADER_H
#define GAMEENGINE_MESHLOADER_H

#include <memory>
#include <assimp/scene.h>
#include <map>

#include "Model.h"
#include "Skeleton.h"

/* Assimp is able to load whole scenes containing hierarchy of meshes etc ... (formats 3ds, collada)
 * I decided the Game Engine will only load single Models, and leave the scene making
 * to the engine. Therefore my engine is assuming loaded model is a single model.
 *
 * ModelLoader uses additional Loader Classes to do the actual loading.
 * */
class ModelLoader {
public:
    static std::shared_ptr<Model> loadModel(const std::string &path);
private:
    /*Loader State*/
    inline static const aiScene* scene;
};

/* Assimp glm conversion */
std::string assimpToStdString(const aiString &assimpString);

glm::mat4 assmipMatToGlmMat(aiMatrix4x4 matrix);


class MeshLoader{
public:
    explicit MeshLoader(const aiScene *scene);

    void loadBasicMeshInfo(const aiMesh *aMesh);
    void addBoneInfo(const std::map<std::string, int> &boneNameToIndexMap);
    std::shared_ptr<Mesh> make();
private:
    std::shared_ptr<Mesh> constructedMesh;
    bool isMeshInitialised(){ return constructedMesh != nullptr; }

    const aiScene* scene;
    const aiMesh* assimpMesh;
};


class SkeletonLoader{
public:
    SkeletonLoader(const aiScene *scene);

    void loadSkeleton(const aiMesh *aMesh);

    const std::map<std::string, int> &getBoneNameToboneIdMap() const;

    std::shared_ptr<SkeletalSystem::Skeleton> make();
private:
    void initNodesMap(aiNode *parentNode);

    void markNeededUntilMeshRootOrRootParentFound(aiNode *leaf, aiNode *meshRoot, aiNode *meshRootParent);

    aiNode *findSkeletonRootNode(aiNode *node);

    aiNode *searchForSkeletonRootNode(aiNode *node);

    std::shared_ptr<SkeletalSystem::Bone> assimpNodeToEngineBone(aiNode *node);

    bool isSkeletonInitialised() const { return constructedSkeleton != nullptr; }

    const aiScene* scene;
    const aiMesh* assimpMesh;
    struct NodeNecessityRecord;
    std::map<std::string, NodeNecessityRecord> nodesNeededForSkeleton;
    std::map<std::string, int> boneNameToboneIdMap;
    int nextBoneIndexToBeAssigned;
    std::shared_ptr<SkeletalSystem::Skeleton> constructedSkeleton;

    /* Helper structure for searching for skeleton in assimp node hierarchy.
     * */
    struct NodeNecessityRecord{
        NodeNecessityRecord() = default;
        explicit NodeNecessityRecord(aiNode *node, bool necessary = false) : node(node), necessary(necessary) {}
        aiNode* node;
        bool necessary;
    };
};

class SkeletonAnimation{
public:
    void loadAnimation(aiAnimation* assimpAnimation);
    std::shared_ptr<SkeletalSystem::SkeletalAnimation> make();
private:
    void doCleanup();
    aiAnimation* assimpAnimation;
    std::shared_ptr<SkeletalSystem::SkeletalAnimation> constructedAnimation;
};
#endif //GAMEENGINE_MESHLOADER_H
