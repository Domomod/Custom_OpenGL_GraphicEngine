//
// Created by dominik on 12.04.19.
//

#ifndef GAMEENGINE_MODEL_H
#define GAMEENGINE_MODEL_H

#include "Mesh.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/Skeleton.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimation.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimator.h"
#include "Source/MyExceptions.h"

class Model {
public:
    Model() {
        /* We have to at least initialise the mesh to make Model a valid state object
         * */
        mesh = std::make_shared<Mesh>();
    }

    std::shared_ptr<Mesh> mesh;

    /* A Model might or might not have a skeleton, if a model has
     * a skeleton it should have an animation (otherwise no point in having a skeleton)
     * */
    bool hasSkeletonAndAnimation = false;
    std::shared_ptr<SkeletalSystem::SkeletalAnimator> animator;
    std::shared_ptr<SkeletalSystem::Skeleton> skeleton;
    std::shared_ptr<SkeletalSystem::SkeletalAnimation> skeletalAnimation;
};

class ModelFactory {
public:
    ModelFactory& addMesh(const std::shared_ptr<Mesh>& _mesh){
        mesh = _mesh;
        addedMesh = true;
        return *this;
    }

    std::shared_ptr<Model> make(){
        std::shared_ptr<Model> returnModel = std::make_shared<Model>();

        if( !addedMesh ){
            throw InvalidData("ModelFactroy was not given a model but was ordered to create a Mesh");
        }

        returnModel->mesh = mesh;

        addedMesh = false;

        return returnModel;
    }

private:
    std::shared_ptr<Mesh> mesh;
    bool addedMesh = false;
};

#endif //GAMEENGINE_MODEL_H
