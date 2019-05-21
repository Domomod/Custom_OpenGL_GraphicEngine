//
// Created by dominik on 23.04.19.
//


#include "SkeletonLoader.h"
using namespace SkeletalSystem;

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>

#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/Skeleton.h"
#include "Source/DataLayer/DataTypes/Assets/AssimpConversion.h"

#include "Source/MyExceptions.h"

void SkeletonLoader::loadSkeleton(aiMesh **assimpMeshTable, unsigned int tableSize) {


    bool hasSkeleton = false;
    for(unsigned int idx = 0; idx < tableSize; idx++) {
        assimpMesh = assimpMeshTable[idx];
        if(assimpMesh->HasBones()) {
            hasSkeleton = true;
        }
    }
    if(hasSkeleton == false){
        constructedSkeleton = nullptr;
        return;
    }

    constructedSkeleton = std::make_shared<SkeletalSystem::Skeleton>();
    nextBoneIndexToBeAssigned = 0;

    initialiseNodeMaps(scene->mRootNode);

    for(unsigned int idx = 0; idx < tableSize; idx++) {
        assimpMesh = assimpMeshTable[idx];
        if(assimpMesh->HasBones()) {
            std::string meshName = assimpToEngine(assimpMesh->mName);
            aiNode *meshRootNode = nodesNeededForSkeleton[meshName].node;
            aiNode *meshRootParentNode = meshRootNode->mParent;
            findNodesRepresentingThisSkeletonBones(meshRootNode, meshRootParentNode);
        }
    }

    aiNode* skeletonRoot = findSkeletonRootNode(scene->mRootNode);

    constructedSkeleton->rootBone = assimpNodeToEngineBone(skeletonRoot);
    constructedSkeleton->globalInverseTransformation = glm::inverse(constructedSkeleton->rootBone->toParentSpaceMatrix);

}

void SkeletonLoader::findNodesRepresentingThisSkeletonBones(aiNode *meshRootNode,
                                                            aiNode *meshRootParentNode) {
    aiBone** bones = assimpMesh->mBones;
    unsigned int numBones = assimpMesh->mNumBones;
    for(int boneId = 0; boneId < numBones; boneId++){
        aiBone* bone = bones[boneId];
        aiNode* boneNode = nodesNeededForSkeleton[assimpToEngine(bone->mName)].node;
        markNeededUntilMeshRootOrRootParentFound(
                boneNode,
                meshRootNode,
                meshRootParentNode
                );

        boneNameToOffsetMap.emplace(
                assimpToEngine(bone->mName),
                glmCast(bone->mOffsetMatrix)
                );
    }
}

void SkeletonLoader::initialiseNodeMaps(aiNode *parentNode) {
    nodesNeededForSkeleton.emplace(
            assimpToEngine(parentNode->mName),
            NodeNecessityRecord(parentNode, false)
            );

    aiNode** children = parentNode->mChildren;
    unsigned int numChildren = parentNode->mNumChildren;
    for(int i = 0; i < numChildren; i++){
        initialiseNodeMaps(children[i]);
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
        auto& necessityRecord = nodesNeededForSkeleton[assimpToEngine(leaf->mName) ];

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

    auto& necessityRecord = nodesNeededForSkeleton[assimpToEngine(node->mName) ];

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
    auto nodeName = assimpToEngine(node->mName);
    auto& necessityRecord = nodesNeededForSkeleton[ nodeName ];

    if(necessityRecord.necessary){
        auto thisBone = std::make_shared<SkeletalSystem::Bone>();

        /* Initially bone parent is a nullptr. It's father will set himself as a parent when going back
         * from the recursion. This way the root bone will have a null Parent as expected.
         * */
        thisBone->Parent = nullptr;

        thisBone->name = nodeName;

        thisBone->toParentSpaceMatrix = glmCast(node->mTransformation);

        auto iterator = boneNameToOffsetMap.find(nodeName);
        thisBone->offset =  iterator == boneNameToOffsetMap.end() ? glm::mat4(1)
                                                                  : (*iterator).second;


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
         * they would mark this node as unnecessary.
         * */
        return nullptr;
    }

}

/* Returns a nullptr if no skeleton was loaded, it's not a problem
 * because if a model does not have skeleton it would be represented
 * by a nullptr anyway. So assigning a nullptr to model changes nothing.
 * */
std::shared_ptr<SkeletalSystem::Skeleton> SkeletonLoader::make() {
    auto returnSkeleton = constructedSkeleton;
    returnedInitialisedSkeleton = constructedSkeleton != nullptr;
    constructedSkeleton = nullptr;
    return returnSkeleton;
}

const std::map<std::__cxx11::string, int> &SkeletonLoader::getBoneNameToboneIdMap() const {
    if(isSkeletonInitialised() == false){
        throw SkeletonLoadingException("Asked for bone name to id map before skeleton was initialised.");
    }
    return boneNameToboneIdMap;
}

void SkeletonLoader::setScene(const aiScene *scene) {
    SkeletonLoader::scene = scene;
}

bool SkeletonLoader::isSkeletonInitialised() const { return (constructedSkeleton != nullptr) || returnedInitialisedSkeleton; }
