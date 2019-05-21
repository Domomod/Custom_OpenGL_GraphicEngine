//
// Created by dominik on 24.04.19.
//

#include "SkeletonAnimationLoader.h"

#include <assimp/scene.h>

#include "SkeletalAnimation.h"
#include "Source/DataLayer/DataTypes/Assets/AssimpConversion.h"

using namespace SkeletalSystem;

void SkeletonAnimationLoader::loadAnimation( aiAnimation *aAnimation,
                                             const std::map<std::__cxx11::string, int> &boneNameToIndexMap ) {
    if(aAnimation->mNumChannels == 0){
        throw AnimationLoadingException("Tried to load a non-skeletal animation as a skeletal one.");
    }

    assimpAnimation = aAnimation;
    constructedAnimation = std::make_shared<SkeletalSystem::SkeletalAnimation>();

    constructedAnimation->animationName = assimpToEngine(assimpAnimation->mName);
    constructedAnimation->durationInTicks = assimpAnimation->mDuration;
    constructedAnimation->ticksPerSecond = assimpAnimation->mTicksPerSecond;

    unsigned long bonesCount = boneNameToIndexMap.size();

    for(int i = 0; i < assimpAnimation->mNumChannels; i ++){
        aiNodeAnim *nodeAnimation = assimpAnimation->mChannels[i];
        auto boneIterator = boneNameToIndexMap.find( assimpToEngine(nodeAnimation->mNodeName) );
        int boneIndex = boneIterator->second;

        constructedAnimation->idToBoneAnimMap.emplace(
                boneIndex,
                SkeletalSystem::BoneAnimation(translateKeyframes<SkeletalSystem::KeyFrame<glm::vec3>>(nodeAnimation->mNumPositionKeys,
                                                                                                      nodeAnimation->mPositionKeys     ),
                                              translateKeyframes<SkeletalSystem::KeyFrame<glm::quat>>(nodeAnimation->mNumRotationKeys,
                                                                                                      nodeAnimation->mRotationKeys     ),
                                              translateKeyframes<SkeletalSystem::KeyFrame<glm::vec3>>(nodeAnimation->mNumScalingKeys,
                                                                                                      nodeAnimation->mScalingKeys      )
                               )
                );

        constructedAnimation->nameToBoneAnimMap.emplace(
                assimpToEngine(nodeAnimation->mNodeName),
                SkeletalSystem::BoneAnimation(translateKeyframes<SkeletalSystem::KeyFrame<glm::vec3>>(nodeAnimation->mNumPositionKeys,
                                                                                                      nodeAnimation->mPositionKeys     ),
                                              translateKeyframes<SkeletalSystem::KeyFrame<glm::quat>>(nodeAnimation->mNumRotationKeys,
                                                                                                      nodeAnimation->mRotationKeys     ),
                                              translateKeyframes<SkeletalSystem::KeyFrame<glm::vec3>>(nodeAnimation->mNumScalingKeys,
                                                                                                      nodeAnimation->mScalingKeys      )
                )
        );
    }
}

template<class EngineKeyframe, class AssimpKeyframe>
std::vector<EngineKeyframe> SkeletonAnimationLoader::translateKeyframes(unsigned int numKeyframes,
                                                                        AssimpKeyframe *assimpKeyframes ) {
    std::vector<EngineKeyframe> engineKeyframes;
    for(int keyFrameId = 0; keyFrameId < numKeyframes; keyFrameId ++){
        AssimpKeyframe& keyFrame = assimpKeyframes[keyFrameId];
        double timeStamp = keyFrame.mTime;
        typename EngineKeyframe::ValueType translation = glmCast(keyFrame.mValue);
        engineKeyframes.emplace_back(timeStamp, translation);
    }
    return engineKeyframes;
}

std::shared_ptr<SkeletalSystem::SkeletalAnimation> SkeletonAnimationLoader::make() {
    if(constructedAnimation == nullptr){
        throw AnimationLoadingException("Tried to create an empty animation. Possibly tried to return the same animation twice.");
    }
    auto returnAnimation = constructedAnimation;
    constructedAnimation = nullptr;

    return returnAnimation;
}

/* Because I exactly know what types those functions will have I can use forward declaration.
 * Using this 'trick' I can declare template in .h and define it in .cpp, in result creating
 * cleaner code.
 * */
template std::vector< KeyFrame<glm::vec3> >
SkeletonAnimationLoader::translateKeyframes< KeyFrame<glm::vec3> , aiVectorKey >(
unsigned int numKeyframes, aiVectorKey *assimpKeyframes);

template std::vector< KeyFrame<glm::quat> >
SkeletonAnimationLoader::translateKeyframes< KeyFrame<glm::quat>, aiQuatKey >(
unsigned int numKeyframes, aiQuatKey *assimpKeyframes);