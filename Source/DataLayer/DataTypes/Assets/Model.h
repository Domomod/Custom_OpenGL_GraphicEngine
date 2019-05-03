//
// Created by dominik on 12.04.19.
//

#ifndef GAMEENGINE_MODEL_H
#define GAMEENGINE_MODEL_H

#include "Mesh.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/Skeleton.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimation.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimator.h"
#include "Source/DataLayer/DataTypes/Assets/Textures/Texture.h"
#include "Source/MyExceptions.h"

class Model {
public:
    Model() {}

    std::vector< std::shared_ptr<Mesh> > meshes;
    std::vector< std::shared_ptr<Texture> > diffuseTextures;

    /* A Model might or might not have a Skeletal system
     * */
    bool hasSkeletonAndAnimation = false;
    std::shared_ptr<SkeletalSystem::SkeletalAnimator> animator;
    std::shared_ptr<SkeletalSystem::Skeleton> skeleton;
    std::vector<std::shared_ptr<SkeletalSystem::SkeletalAnimation>> skeletalAnimations;
};

class ModelFactory {
public:
    ModelFactory& addMesh(const std::shared_ptr<Mesh>& mesh){
        meshes.push_back(mesh);
        addedMesh = true;
        return *this;
    }

    std::shared_ptr<Model> make(){
        std::shared_ptr<Model> returnModel = std::make_shared<Model>();

        if( !addedMesh ){
            throw InvalidData("ModelFactroy was not given a model but was ordered to create a Mesh");
        }

        returnModel->meshes = meshes;

        addedMesh = false;

        return returnModel;
    }

private:
    std::vector< std::shared_ptr<Mesh> > meshes;
    bool addedMesh = false;
};

#endif //GAMEENGINE_MODEL_H
