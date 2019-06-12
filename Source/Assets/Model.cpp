//
// Created by dominik on 12.04.19.
//

#include "Model.h"

#include "Assets/Mesh/Mesh.h"
#include "Assets/SkeletalSystem/Skeleton.h"
#include "Assets/SkeletalSystem/SkeletalAnimation.h"
#include "Assets/SkeletalSystem/SkeletalAnimator.h"
#include "Assets/Textures/Texture.h"
#include "MyExceptions.h"

ModelFactory &ModelFactory::addMesh(const std::shared_ptr<Mesh> &mesh)
{
    meshes.push_back(mesh);
    addedMesh = true;
    return *this;
}

std::shared_ptr<Model> ModelFactory::make()
{
    std::shared_ptr<Model> returnModel = std::make_shared<Model>();

    if (!addedMesh)
    {
        throw InvalidData("ModelFactroy was not given a model but was ordered to create a Mesh");
    }

    returnModel->meshes = meshes;

    addedMesh = false;

    return returnModel;
}
