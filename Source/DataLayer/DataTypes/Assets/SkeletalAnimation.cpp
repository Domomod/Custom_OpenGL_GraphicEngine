//
// Created by dominik on 11.04.19.
//

#include "SkeletalAnimation.h"

#include "Source/MyExceptions.h"

SkeletalSystem::BoneTransformation SkeletalSystem::interpolate(const SkeletalSystem::BoneTransformation &startBone,
                                                               const SkeletalSystem::BoneTransformation &endBone,
                                                               float progressionFraction) {
    if(progressionFraction > 1.0f){
        throw AnimationInterpolationError("Interpolation result not in between desired Bones (progrssionFraction >1).");
    }

    glm::vec3 translation = glm::mix(  startBone.translation, endBone.translation, progressionFraction );
    glm::quat rotation    = glm::lerp( startBone.rotation   , endBone.rotation   , progressionFraction );
    glm::vec3 scalling    = glm::mix(  startBone.scalling   , endBone.scalling   , progressionFraction );

    return SkeletalSystem::BoneTransformation( translation, rotation, scalling );
}

SkeletalSystem::KeyFrame SkeletalSystem::interpolate(const SkeletalSystem::KeyFrame &startFrame,
                                                     const SkeletalSystem::KeyFrame &endFrame,
                                                     float progression) {
    if(startFrame.boneTransformations.size()!=endFrame.boneTransformations.size()){
        throw AnimationInterpolationError("Interpolated keyframes describe different amount of bones.");
    }

    KeyFrame currentFrame;
    auto bonesCount =  startFrame.boneTransformations.size();
    currentFrame.boneTransformations.resize( bonesCount );

    for(int i = 0; i < bonesCount; i++){
        currentFrame.boneTransformations[i] = interpolate( startFrame.boneTransformations[i],
                                                           endFrame.boneTransformations[i]  ,
                                                           progression                      );
    }

    return currentFrame;
}
