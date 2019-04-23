//
// Created by dominik on 11.04.19.
//

#ifndef GAMEENGINE_SKELETALANIMATION_H
#define GAMEENGINE_SKELETALANIMATION_H

#include "Skeleton.h"

#include <map>
#include <vector>
#include <glm/gtc/quaternion.hpp>

namespace SkeletalSystem {
/* Each bone Animation stores transformation info for each keyframe (instead of keyframes storing transformation
 * for each bone). That's the way assimp stores it's animations so I decided it's best not to change the format as
 * it doesn't really matter much.
 * */

    struct BoneTransformation{
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scalling;

        BoneTransformation() = default;
        BoneTransformation(const glm::vec3 &translation, const glm::quat &rotation, const glm::vec3 &scalling)
                : translation(translation), rotation(rotation), scalling(scalling) {}
    };

    BoneTransformation interpolate(const BoneTransformation &startBone,
                                   const BoneTransformation &endBone,
                                   float progressionFraction);

    struct KeyFrame {
        float timeStamp;
        std::vector<BoneTransformation> boneTransformations;
    };

    KeyFrame interpolate(const KeyFrame &startFrame,
                         const KeyFrame &endFrame,
                         float progression);

    class SkeletalAnimation {
    public:
        std::string animationName;
        double durationInTicks;
        double ticksPerSecond;
        std::vector<KeyFrame> keyFrames;
    };
}

#endif //GAMEENGINE_SKELETALANIMATION_H
