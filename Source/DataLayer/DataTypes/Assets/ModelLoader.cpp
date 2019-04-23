//
// Created by dominik on 28.03.19.
//

#include "ModelLoader.h"

#include <map>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>

#include "Source/MyExceptions.h"


std::shared_ptr<Model> ModelLoader::loadModel(const std::string &path) {
    std::shared_ptr<Model> thisModel = std::make_shared<Model>();

    //Using imporeter because it will handle resource cleaning.
    Assimp::Importer importer;

    scene = importer.ReadFile(path.c_str(),
        aiProcess_CalcTangentSpace          |
        aiProcess_GenSmoothNormals          |
        aiProcess_JoinIdenticalVertices     |
        aiProcess_ImproveCacheLocality      |
        aiProcess_LimitBoneWeights          |
        aiProcess_RemoveRedundantMaterials  |
        aiProcess_SplitLargeMeshes          |
        aiProcess_Triangulate               |
        aiProcess_GenUVCoords               |
        aiProcess_SortByPType               |
        aiProcess_FindDegenerates           |
        aiProcess_FindInvalidData           |
        aiProcess_FindInstances             |
        aiProcess_ValidateDataStructure     |
        aiProcess_OptimizeMeshes            |
        aiProcess_Debone                    |
        0
        );

    if(!scene){
        throw ModelLoadingException("Could not load file: " + path);
    }

    /*TODO: for every mesh in assimp scene load it to model*/
    aiMesh* assimpMesh = scene->mMeshes[0];
    auto meshLoader = MeshLoader(scene);
    auto skeletonLoader = SkeletonLoader(scene);


    meshLoader.loadBasicMeshInfo(assimpMesh);

    if(assimpMesh->HasBones()){
        skeletonLoader.loadSkeleton(assimpMesh);

        meshLoader.addBoneInfo( skeletonLoader.getBoneNameToboneIdMap() );

        thisModel->skeleton = skeletonLoader.make();
    }

    thisModel->mesh = meshLoader.make();


    return thisModel;
}


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

const std::map<std::string, int> &SkeletonLoader::getBoneNameToboneIdMap() const {
    if(isSkeletonInitialised() == false){
        throw SkeletonLoadingException("Asked for bone name to id map before skeleton was initialised.");
    }
    return boneNameToboneIdMap;
}

SkeletonLoader::SkeletonLoader(const aiScene *scene) : scene(scene) {}




glm::mat4 assmipMatToGlmMat(aiMatrix4x4 matrix) {

    float mat[] = {
            matrix.a1, matrix.a2, matrix.a3, matrix.a4,
            matrix.b1, matrix.b2, matrix.b3, matrix.b4,
            matrix.c1, matrix.c2, matrix.c3, matrix.c4,
            matrix.d1, matrix.d2, matrix.d3, matrix.d4
    };

    glm::mat4 glmMat4 = glm::make_mat4(mat);
    /* Assmip is row major while glm is column major
     * */
    glm::transpose(glmMat4);

    return glmMat4;
}

std::string assimpToStdString(const aiString &assimpString) {
    return std::string(assimpString.C_Str());
}


void MeshLoader::loadBasicMeshInfo(const aiMesh *aMesh) {
    assimpMesh = aMesh;
    constructedMesh = std::make_shared<Mesh>();

    for(unsigned int vertIdx = 0; vertIdx < assimpMesh->mNumVertices; vertIdx++){
        aiVector3D pos = assimpMesh->mVertices[vertIdx];
        constructedMesh->positions.emplace_back(
                pos.x,
                pos.y,
                pos.z,
                1
        );

        aiVector3D normals = assimpMesh->mNormals[vertIdx];

        constructedMesh->normals.emplace_back(
                normals.x,
                normals.y,
                normals.z,
                0
        );

        if(assimpMesh->HasTextureCoords(0)){
            aiVector3D uvs = assimpMesh->mTextureCoords[0][vertIdx];
            /* OpenGl describes textures startign from left down corner while most
             * file formats start from left upper corner. Flipping the coordinates is
             * faster than flipping actual images.
             * */
            constructedMesh->uv.emplace_back(
                    uvs.x,
                    1 - uvs.y
            );
        } else {
            constructedMesh->uv.emplace_back(
                    0,
                    0
            );
        }

        if(assimpMesh->HasVertexColors(0)){
            aiColor4D color = assimpMesh->mColors[0][vertIdx];
            constructedMesh->colors.emplace_back(
                    color.r,
                    color.g,
                    color.b,
                    color.a
            );
        }
        else {
            constructedMesh->colors.emplace_back(
                    1,
                    0,
                    0,
                    1
            );
        }
    }

    for (unsigned int f = 0; f<assimpMesh->mNumFaces; f++)
    {
        aiFace face = assimpMesh->mFaces[f];
        constructedMesh->indicies.push_back(static_cast<unsigned short &&>(face.mIndices[0]));
        constructedMesh->indicies.push_back(static_cast<unsigned short &&>(face.mIndices[1]));
        constructedMesh->indicies.push_back(static_cast<unsigned short &&>(face.mIndices[2]));
    }
}

void MeshLoader::addBoneInfo(const std::map<std::string, int> &boneNameToIndexMap) {
    if(isMeshInitialised() == false){
        throw MeshLoadingException("Tried to add per vertex bone info to mesh before adding basic vertex info.");
    }
    /* At this point Model Loader has already filled the
     * basic mesh info (positions, color, etc...). Bone info
     * will be added in a very chaotic way (because of the way
     * assimp stores bone weights) therefore we need resize boneInfo
     * vectors right now (to avoid SIGABRT :<).
     * */
    auto vertexCount = constructedMesh->positions.size();
    assert(vertexCount!=0);
    constructedMesh->boneIds.resize(vertexCount, glm::ivec4(0));
    constructedMesh->boneWeights.resize(vertexCount, glm::vec4(0.f));
    /* For each bone in mesh;
     * */
    for(int i = 0; i < assimpMesh->mNumBones; i++){
        aiBone* bone = assimpMesh->mBones[i];
        std::string boneName = assimpToStdString(bone->mName);
        auto iteratorToBone = boneNameToIndexMap.find(boneName);
        int boneIdx = iteratorToBone->second;
        /* For each vertex the bone affects
         * */
        for(int j = 0; j < bone->mNumWeights; j++){
            aiVertexWeight weight =  bone->mWeights[j];
            constructedMesh->addBoneData(
                    weight.mVertexId,
                    boneIdx,
                    weight.mWeight
            );
        }
    }
}

std::shared_ptr<Mesh> MeshLoader::make() {
    if( isMeshInitialised()==false ){
        throw MeshLoadingException("Tried to return an invalid (empty) mesh");
    }
    std::shared_ptr returnPtr = constructedMesh;
    constructedMesh = nullptr;
    return returnPtr;
}

MeshLoader::MeshLoader(const aiScene *scene) : scene(scene) {}
