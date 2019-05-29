//
// Created by dominik on 12.04.19.
//

#include "Model.h"

#include "Source/DataLayer/DataTypes/Assets/Mesh/Mesh.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/Skeleton.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimation.h"
#include "Source/DataLayer/DataTypes/Assets/SkeletalSystem/SkeletalAnimator.h"
#include "Source/DataLayer/DataTypes/Assets/Textures/Texture.h"
#include "Source/MyExceptions.h"

ModelFactory &ModelFactory::addMesh(const std::shared_ptr<Mesh> &mesh) {
    meshes.push_back(mesh);
    addedMesh = true;
    return *this;
}

std::shared_ptr<Model> ModelFactory::make() {
    std::shared_ptr<Model> returnModel = std::make_shared<Model>();

    if( !addedMesh ){
        throw InvalidData("ModelFactroy was not given a model but was ordered to create a Mesh");
    }

    returnModel->meshes = meshes;

    addedMesh = false;

    return returnModel;
}
