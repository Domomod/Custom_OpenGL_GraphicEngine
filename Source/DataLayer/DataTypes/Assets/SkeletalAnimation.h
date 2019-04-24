#include <utility>

//
// Created by dominik on 11.04.19.
//

#ifndef GAMEENGINE_SKELETALANIMATION_H
#define GAMEENGINE_SKELETALANIMATION_H

#include "Skeleton.h"

#include <map>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include "Source/MyExceptions.h"

namespace SkeletalSystem {
/* Each bone Animation stores transformation info for each keyframe (instead of keyframes storing transformation
 * for each bone). That's the way assimp stores it's animations so I decided it's best not to change the format as
 * it doesn't really matter much.
 * */

    template<class T>
    struct KeyFrame {
        double timeStamp;
        T value;

        typedef T ValueType;

        KeyFrame() = default;

        KeyFrame(double timeStamp, T value) : timeStamp(timeStamp), value(value) {}
    };

    struct BoneAnimation {
        std::vector<KeyFrame<glm::vec3>> translations;
        std::vector<KeyFrame<glm::quat>> rotation;
        std::vector<KeyFrame<glm::vec3>> scaling;

        template<class T>
        std::pair<T, T> findTwoNearest(float time, std::vector<KeyFrame<T>> &keyframes);

        BoneAnimation() = default;

        BoneAnimation(std::vector<KeyFrame<glm::vec3>> translations,
                      std::vector<KeyFrame<glm::quat>> rotation    ,
                      std::vector<KeyFrame<glm::vec3>> scaling     )
                : translations(std::move(translations)), rotation(std::move(rotation)), scaling(std::move(scaling)) {}
    };

    BoneAnimation interpolate(const BoneAnimation &startBone,
                              const BoneAnimation &endBone,
                              float time);


    class SkeletalAnimation {
    public:
        std::string animationName;
        double durationInTicks;
        double ticksPerSecond;
        std::map<int,BoneAnimation> idToBoneAnimMap;
    };


    /* Template functions definitions
     * */

    template<class T>
    std::pair<T, T> BoneAnimation::findTwoNearest(float time, std::vector<KeyFrame<T>> &keyframes) {
        if (time < 0) {
            throw AnimationInterpolationError("Program was asked to calculate a negative time animation pose.");
        }
        if (keyframes.size() <= 1) {
            throw AnimationInterpolationError("Program was asked to find two nearest keyFrames "
                                              "while Node Animation did not have at least two keyframes.");
        }
        T nearestPreviousValue;
        T nearestAfterValue;
        for (KeyFrame<T> &keyframe : keyframes) {
            nearestAfterValue = keyframe.value;
            if (keyframe.timeStamp > time) {
                return std::make_pair<T, T>(nearestPreviousValue, nearestAfterValue);
            }
            nearestPreviousValue = keyframe.value;
        }
        /* Reaching this point would mean given time exceeded animation time
         * */
        throw AnimationInterpolationError("Time exceeded animation time while searching nearest keyframes");
    }
}

#endif //GAMEENGINE_SKELETALANIMATION_H
