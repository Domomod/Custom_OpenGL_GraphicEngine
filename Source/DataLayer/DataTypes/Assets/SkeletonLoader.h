//
// Created by dominik on 23.04.19.
//

#ifndef GAMEENGINE_SKELETONLOADER_H
#define GAMEENGINE_SKELETONLOADER_H

#include <memory>
#include <assimp/scene.h>
#include <map>
#include "Model.h"

class SkeletonLoader{
public:
    SkeletonLoader(const aiScene *scene);

    void loadSkeleton(const aiMesh *aMesh);

    const std::map<std::__cxx11::string, int> &getBoneNameToboneIdMap() const;

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
    std::map<std::__cxx11::string, NodeNecessityRecord> nodesNeededForSkeleton;
    std::map<std::__cxx11::string, int> boneNameToboneIdMap;
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

#endif //GAMEENGINE_SKELETONLOADER_H
