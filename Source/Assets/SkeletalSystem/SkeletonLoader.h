//
// Created by dominik on 23.04.19.
//

#ifndef GAMEENGINE_SKELETONLOADER_H
#define GAMEENGINE_SKELETONLOADER_H

#include <memory>
#include <map>
#include <glm/glm.hpp>

class aiScene;

class aiMesh;

class aiNode;

namespace SkeletalSystem
{
    class Skeleton;

    class Bone;
}

/* Skeleton Loader is designed to load one Skeleton at a time. It can load
 * multiple skeletons one after another. Loaded skeleton is owned by the loader
 * until released to the system by make() function. After that Loader
 * goes back to the state after initialisation, therefore it is not possible
 * to return multiple copies of the same skeleton by calling make multiple times.
 * This way it's easier to assure that functions are called in right order.
 * */

namespace SkeletalSystem
{
    class SkeletonLoader
    {
    public:
        SkeletonLoader() = default;

        void setScene(const aiScene *scene);

        void loadSkeleton(aiMesh **assimpMeshTable, unsigned int tableSize);

        const std::map<std::__cxx11::string, int> &getBoneNameToboneIdMap() const;

        std::shared_ptr<SkeletalSystem::Skeleton> make();

        bool isSkeletonInitialised() const;

    private:
        void initialiseNodeMaps(aiNode *parentNode);

        void findNodesRepresentingThisSkeletonBones(aiNode *meshRootNode, aiNode *meshRootParentNode);

        void markNeededUntilMeshRootOrRootParentFound(aiNode *leaf, aiNode *meshRoot, aiNode *meshRootParent);

        aiNode *findSkeletonRootNode(aiNode *node);

        aiNode *searchForSkeletonRootNode(aiNode *node);

        std::shared_ptr<SkeletalSystem::Bone> assimpNodeToEngineBone(aiNode *node);

        const aiScene *scene;
        const aiMesh *assimpMesh;

        /* Assimp node hierarchy may contains nodes for many meshes, we will check which nodes
        * are the bones of our skeleton.
        * */
        struct NodeNecessityRecord;
        std::map<std::string, NodeNecessityRecord> nodesNeededForSkeleton;
        std::map<std::string, int> boneNameToboneIdMap;
        std::map<std::string, glm::mat4> boneNameToOffsetMap;
        int nextBoneIndexToBeAssigned;
        bool returnedInitialisedSkeleton;
        std::shared_ptr<SkeletalSystem::Skeleton> constructedSkeleton;

        /* Helper structure for searching for skeleton in assimp node hierarchy.
         * */
        struct NodeNecessityRecord
        {
            NodeNecessityRecord() = default;

            explicit NodeNecessityRecord(aiNode *node, bool necessary = false) : node(node), necessary(necessary)
            {
            }

            aiNode *node;
            bool necessary;
        };

    };
}
#endif //GAMEENGINE_SKELETONLOADER_H
