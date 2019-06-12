//
// Created by dominik on 11.04.19.
//

#include "Skeleton.h"

const glm::mat4 &SkeletalSystem::Skeleton::getGlobalInverseTransformation() const
{
    return globalInverseTransformation;
}

const std::shared_ptr<SkeletalSystem::Bone> &SkeletalSystem::Skeleton::getRootBone() const
{
    return rootBone;
}