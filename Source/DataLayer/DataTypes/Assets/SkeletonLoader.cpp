//
// Created by dominik on 23.04.19.
//

#include "SkeletonLoader.h"

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>

#include "AssimpConversion.h"

void SkeletonLoader::loadSkeleton(const aiMesh *aMesh) {
    if(aMesh->HasBones() == false){
        throw SkeletonLoadingException("Tried to load a skeleton for a mesh that does not have any.");
    }
    /* Assimp node hierarchy may contains nodes for many meshes, we will check which nodes
     * are the bones of our skeleton. To find out which nodes are bones we will check the aiMesh bones vector
     * each record in this vector contains a corresponding node name.
     * */
    assimpMesh = aMesh;
    constructedSkeleton = std::make_shared<SkeletalSystem::Skeleton>();

    initNodesMap(scene->mRootNode);


    aiNode* meshRootNode = nodesNeededForSkeleton[assimpToStdString(assimpMesh->mName)].node;
    assert(meshRootNode != nullptr);
    aiNode* meshRootParentNode = meshRootNode->mParent;

    /* Find the nodes needed for the skeleton
     * */
    aiBone** bones = assimpMesh->mBones;
    unsigned int numBones = assimpMesh->mNumBones;
    for(int boneId = 0; boneId < numBones; boneId++){
        aiBone* bone = bones[boneId];
        aiNode* boneNode = nodesNeededForSkeleton[assimpToStdString(bone->mName)].node;
        markNeededUntilMeshRootOrRootParentFound(
                boneNode,
                meshRootNode,
                meshRootParentNode
                );
    }

    aiNode* skeletonRoot = findSkeletonRootNode(scene->mRootNode);

    constructedSkeleton->rootBone = assimpNodeToEngineBone(skeletonRoot);
}

void SkeletonLoader::initNodesMap(aiNode *parentNode) {
    nodesNeededForSkeleton.emplace(
            assimpToStdString(parentNode->mName),
            NodeNecessityRecord(parentNode, false)
            );

    aiNode** children = parentNode->mChildren;
    unsigned int numChildren = parentNode->mNumChildren;
    for(int i = 0; i < numChildren; i++){
        initNodesMap(children[i]);
    }
}

/* Unfortunately there might also be nodes which are not used by a bone in the mesh, but
 * still affect the pose of the skeleton because they have child nodes which are bones.
 *
 * For most file formats and modelling packages the node hierarchy of the skeleton
 * is either a child of the mesh node or a sibling of the mesh node. But this is by no means a requirement.
 *
 * according to ~ Assimp documentation: http://sir-kimmi.de/assimp/lib_html/data.html#bones
 * */
void SkeletonLoader::markNeededUntilMeshRootOrRootParentFound(aiNode *leaf, aiNode *meshRoot, aiNode *meshRootParent) {
    if(leaf != meshRoot && leaf != meshRootParent){
        auto& necessityRecord = nodesNeededForSkeleton[ assimpToStdString(leaf->mName) ];

        /* If a node is already marked as necessary this means, we have already searched it parens.
         * Given that this is the only function modifying nodes necessity.
         * */
        if(necessityRecord.necessary){
            return;
        }

        necessityRecord.necessary = true;
        aiNode* leafParent = leaf->mParent;

        markNeededUntilMeshRootOrRootParentFound(leafParent, meshRoot, meshRootParent);
    }
}

aiNode * SkeletonLoader::findSkeletonRootNode(aiNode *node) {

    aiNode* skeletonRootNode = searchForSkeletonRootNode(node);

    if(skeletonRootNode == nullptr){
        throw SkeletonLoadingException("SkeletalSystem Root Node asked for but not found.\n");
    }

    return skeletonRootNode;
}

/* SkeletalSystem Root node is a necessary node that is nearest to scene Root Node.
 * */
aiNode * SkeletonLoader::searchForSkeletonRootNode(aiNode *node) {
    aiNode* returnNode = nullptr;

    auto& necessityRecord = nodesNeededForSkeleton[ assimpToStdString(node->mName) ];

    /* This begins going back from the recursion.
     * */
    if( necessityRecord.necessary ){
        return necessityRecord.node;
    }

    aiNode** Children = node->mChildren;
    unsigned int numChildren = node->mNumChildren;

    for(int childId = 0; childId < numChildren; childId++){

        aiNode* child = Children[childId];

        returnNode = searchForSkeletonRootNode(child);

        /* If we already found the skeletonRootNode there is no point
         * in searching other branches.
         * */
        if(returnNode != nullptr) break;
    }

    return returnNode;
}

std::shared_ptr<SkeletalSystem::Bone> SkeletonLoader::assimpNodeToEngineBone(aiNode *node) {
    auto nodeName = assimpToStdString(node->mName);
    auto& necessityRecord = nodesNeededForSkeleton[ nodeName ];

    if(necessityRecord.necessary){
        auto thisBone = std::make_shared<SkeletalSystem::Bone>();

        /* Initially bone parent is a nullptr. It's father will set himself as a parent when going back
         * from the recursion. This way the root bone will have a null Parent as expected.
         * */
        thisBone->Parent = nullptr;

        thisBone->name = nodeName;

        thisBone->toParentSpaceMatrix = assmipMatToGlmMat( node->mTransformation );

        /* Bones are assigned an index in order they are visited using
        * depth search.
        * */
        boneNameToboneIdMap.emplace(nodeName, nextBoneIndexToBeAssigned);
        thisBone->idx = nextBoneIndexToBeAssigned;
        nextBoneIndexToBeAssigned++;

        /* Link all children to the hierarchy.
         * */
        aiNode** Children = node->mChildren;
        unsigned int numChildren = node->mNumChildren;

        for(int childId = 0; childId < numChildren; childId++){
            aiNode* childNode = Children[childId];
            std::shared_ptr<SkeletalSystem::Bone> childBone = assimpNodeToEngineBone(childNode);
            thisBone->Children.push_back(childBone);
            childBone->Parent = thisBone;
        }

        return thisBone;

    } else {
        /* If a node is not necessary we can totally ignore it, because if it's children were necessary
         * they would mark this node as necessary.
         * */
        return nullptr;
    }

}

std::shared_ptr<SkeletalSystem::Skeleton> SkeletonLoader::make() {
    nodesNeededForSkeleton.clear();
    boneNameToboneIdMap.clear();
    nextBoneIndexToBeAssigned = 0;

    if(isSkeletonInitialised() == false){
        throw SkeletonLoadingException("Tried to create an invalid (empty) skeleton.");
    }

    std::shared_ptr returnSkeleton = constructedSkeleton;
    constructedSkeleton = nullptr;

    return returnSkeleton;
}

const std::map<std::__cxx11::string, int> &SkeletonLoader::getBoneNameToboneIdMap() const {
    if(isSkeletonInitialised() == false){
        throw SkeletonLoadingException("Asked for bone name to id map before skeleton was initialised.");
    }
    return boneNameToboneIdMap;
}

SkeletonLoader::SkeletonLoader(const aiScene *scene) : scene(scene) {}