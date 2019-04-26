//
// Created by dominik on 11.04.19.
//

#include "SkeletalAnimation.h"

#include "Source/MyExceptions.h"

SkeletalSystem::BoneAnimation SkeletalSystem::interpolate(const SkeletalSystem::BoneAnimation &startBone,
                                                          const SkeletalSystem::BoneAnimation &endBone,
                                                          float time) {
    if(time > 1.0f){
        throw AnimationInterpolationError("Interpolation result not in between desired Bones (progrssionFraction >1).");
    }


    return SkeletalSystem::BoneAnimation();
}
