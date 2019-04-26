//
// Created by dominik on 25.04.19.
//

#ifndef GAMEENGINE_SKELETALANIMATOR_H
#define GAMEENGINE_SKELETALANIMATOR_H

#include "SkeletalAnimation.h"
#include "Skeleton.h"
#include "SkeletonConstants.h"

namespace SkeletalSystem {

/*
 * */
    class SkeletalAnimator {
    public:
        SkeletalAnimator(const std::shared_ptr<Skeleton> &itsSkeleton);

        glm::mat4 *getCurrentPoseTransformation();

        void setCurrentAnimation(const std::shared_ptr<SkeletalAnimation> &currentAnimation);

        void calculateCurrentPose();
    private:

        void calculateHierarchyTransforms(const std::shared_ptr<SkeletalSystem::Bone> &bone, const glm::mat4 & parentTransformation);

        const std::shared_ptr<SkeletalSystem::Skeleton> itsSkeleton;
        std::shared_ptr<SkeletalSystem::SkeletalAnimation> currentAnimation;
        glm::mat4 currentPoseTransformation[MAX_BONES] = {glm::mat4(1)};

        double startingTime;
        double currentTime;
        float currentTimeInTicks;
    };
}

#endif //GAMEENGINE_SKELETALANIMATOR_H
