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
 * Model Loader works as a state-machine.
 * */
class ModelLoader {
public:
    static std::shared_ptr<Model> loadModel(const std::string &path);
private:
    /* Helper structure for searching for skeleton in assimp node hierarchy.
     * */
    struct NodeNecessityRecord{
        NodeNecessityRecord() = default;
        explicit NodeNecessityRecord(aiNode *node, bool necessary = false) : node(node), necessary(necessary) {}
        aiNode* node;
        bool necessary;
    };

    /*Loader State*/
    inline static const aiScene* scene;

    inline static const aiMesh* assimpMesh;

    inline static std::map<std::string, NodeNecessityRecord> nodesNeededForSkeleton;

    /* Loading Mesh */
    static std::shared_ptr<Mesh> loadMesh();

    /*Loading SkeletalSystem*/
    static std::shared_ptr<SkeletalSystem::Skeleton> loadSkeleton();

    static void initNodesMap(aiNode *parentNode);

    static void markNeededUntilMeshRootOrRootParentFound(aiNode *leaf, aiNode *meshRoot, aiNode *meshRootParent);

    static aiNode *findSkeletonRootNode(aiNode *node);

    static aiNode *searchForSkeletonRootNode(aiNode *node);

    static std::shared_ptr<SkeletalSystem::Bone> assimpNodeToEngineBone(aiNode *node);

    static std::string assimpToStdString(const aiString &assimpString);

    static glm::mat4 assmipMatToGlmMat(aiMatrix4x4 matrix);
};


#endif //GAMEENGINE_MESHLOADER_H
