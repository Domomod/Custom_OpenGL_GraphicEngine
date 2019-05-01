//std::shared_ptr<SkeletalAnimation>();
// Created by dominik on 25.04.19.
//

#include "SkeletalAnimator.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <GLFW/glfw3.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

SkeletalSystem::SkeletalAnimator::SkeletalAnimator(const std::shared_ptr<SkeletalSystem::Skeleton> &itsSkeleton)
        : itsSkeleton(itsSkeleton) {}

void SkeletalSystem::SkeletalAnimator::setCurrentAnimation(
        const std::shared_ptr<SkeletalSystem::SkeletalAnimation> &currentAnimation) {
    SkeletalAnimator::currentAnimation = currentAnimation;
    startingTime = glfwGetTime();
}

void SkeletalSystem::SkeletalAnimator::calculateCurrentPose() {
    currentTime = fmod( glfwGetTime() - startingTime, currentAnimation->getDurationInTicks() / currentAnimation->getTicksPerSecond());
    currentTimeInTicks = static_cast<float>(currentTime / currentAnimation->getTicksPerSecond());

    glm::mat4 IdentityMatrix = glm::mat4(1);

    calculateHierarchyTransforms(itsSkeleton->getRootBone(), IdentityMatrix);
}

void
SkeletalSystem::SkeletalAnimator::calculateHierarchyTransforms(const std::shared_ptr<SkeletalSystem::Bone> &bone, const glm::mat4 & parentTransformation) {
    glm::mat4 localTransformation = bone->toParentSpaceMatrix;

    SkeletalSystem::BoneAnimation* boneAnimation = currentAnimation->findBoneAnimation(bone->name);

    if(boneAnimation != nullptr) {
        glm::vec3 translation = boneAnimation->calculateInterpolatedTransaltions(currentTimeInTicks);
        glm::quat rotation = boneAnimation->calculateInterpolatedRotation(currentTimeInTicks);
        glm::vec3 scaling = boneAnimation->calculateInterpolatedScaling(currentTimeInTicks);

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1), translation);
        glm::mat4 rotationMatrix = glm::toMat4(rotation);
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1), scaling);

        localTransformation = translationMatrix * rotationMatrix * scalingMatrix;
    }

    glm::mat4 globalTransformation = parentTransformation * localTransformation ;
    glm::mat4 inverseTransformation = itsSkeleton->getGlobalInverseTransformation();
    glm::mat4 offsetTransformation = bone->offset;

    currentPoseTransformation[bone->idx] = inverseTransformation * globalTransformation * offsetTransformation;

    for(auto& child : bone->Children){
        calculateHierarchyTransforms(child, globalTransformation);
    }
}

glm::mat4 *SkeletalSystem::SkeletalAnimator::getCurrentPoseTransformation() {
    return currentPoseTransformation;
}


