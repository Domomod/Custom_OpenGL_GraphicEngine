//
// Created by dominik on 11.04.19.
//

#include "SkeletalAnimation.h"

#include "Source/MyExceptions.h"
#include <glm/glm.hpp>

using namespace SkeletalSystem;

const std::string &SkeletalAnimation::getAnimationName() const {
    return animationName;
}

double SkeletalAnimation::getDurationInTicks() const {
    return durationInTicks;
}

double SkeletalAnimation::getTicksPerSecond() const {
    return ticksPerSecond;
}

template<class T>
T BoneAnimation::calculateCurrent(float time, std::vector<KeyFrame<T>> &keyframes) {
    if(keyframes.size() == 0){
        throw AnimationInterpolationError("Tried to calculate current keyframe from an empty keyframes vector");
    }
    else if (keyframes.size() == 1){
        return keyframes[0].value;
    }
    else{
        return calculateInterpolated<T>(time, keyframes);
    }
}


template<class T>
T BoneAnimation::calculateInterpolated(float time, std::vector<KeyFrame<T>> &keyframes) {
    if (time < 0) {
        throw AnimationInterpolationError("Program was asked to calculate a negative time animation pose.");
    }
    if (keyframes.size() <= 1) {
        throw AnimationInterpolationError("Program was asked to find two nearest keyFrames "
                                          "while Node Animation did not have at least two keyframes.");
    }
    T nearestPreviousValue;
    T nearestAfterValue;
    float prevTime = -1;
    float afterTime;
    for (KeyFrame<T> &keyframe : keyframes) {
        nearestAfterValue = keyframe.value;
        afterTime = keyframe.timeStamp;
        if (keyframe.timeStamp > time) {
            if(prevTime == -1){
                throw AnimationInterpolationError("First keyframe is already ahead of animation time.");
            }
            float distanceBetweenKeyframes = afterTime - prevTime;
            float timeAheadFirstKeyframe = time - prevTime;
            float progressionPercent = timeAheadFirstKeyframe / distanceBetweenKeyframes;

            return interpolation(nearestPreviousValue, nearestAfterValue, progressionPercent);
        }
        nearestPreviousValue = keyframe.value;
        prevTime = keyframe.timeStamp;
    }
    /* Reaching this point would mean given time exceeded animation time
     * */
    throw AnimationInterpolationError("Time exceeded animation time while searching nearest keyframes");
}

template<class T>
T BoneAnimation::calculateNearest(float time, std::vector<KeyFrame<T>> &keyframes) {
    if (time < 0) {
        throw AnimationInterpolationError("Program was asked to calculate a negative time animation pose.");
    }
    if (keyframes.size() <= 1) {
        throw AnimationInterpolationError("Program was asked to find two nearest keyFrames "
                                          "while Node Animation did not have at least two keyframes.");
    }
    T nearestPreviousValue;
    T nearestAfterValue;
    float prevTime = -1;
    float afterTime;
    for (KeyFrame<T> &keyframe : keyframes) {
        nearestAfterValue = keyframe.value;
        afterTime = keyframe.timeStamp;
        if (keyframe.timeStamp > time) {
            if(prevTime == -1){
                throw AnimationInterpolationError("First keyframe is already ahead of animation time.");
            }
            float distanceBetweenKeyframes = afterTime - prevTime;
            float timeAheadFirstKeyframe = time - prevTime;
            float progressionPercent = timeAheadFirstKeyframe / distanceBetweenKeyframes;

            if(time - prevTime > afterTime - time)
                return nearestAfterValue;
            else
                return nearestPreviousValue;
        }
        nearestPreviousValue = keyframe.value;
        prevTime = keyframe.timeStamp;
    }
    /* Reaching this point would mean given time exceeded animation time
     * */
    throw AnimationInterpolationError("Time exceeded animation time while searching nearest keyframes");
}

glm::vec3 BoneAnimation::interpolation(const glm::vec3 &first, const glm::vec3 &second, float progression) {
    return glm::mix(first, second, progression);
}


glm::quat BoneAnimation::interpolation(const glm::quat &first, const glm::quat &second, float progression) {
    return glm::slerp(first, second, progression);
}


glm::vec3 BoneAnimation::calculateInterpolatedTransaltions(float time) {
    return calculateCurrent<glm::vec3>(time, translations);
}


glm::quat BoneAnimation::calculateInterpolatedRotation(float time) {
    return calculateCurrent<glm::quat>(time, rotations);

}


glm::vec3 BoneAnimation::calculateInterpolatedScaling(float time) {
    return calculateCurrent<glm::vec3>(time, scalings);

}

glm::vec3 BoneAnimation::calculateNearestTransaltions(float time) {
    if(translations.size() == 0){
        throw AnimationInterpolationError("Tried to calculate current keyframe from an empty keyframes vector");
    }
    else if (translations.size() == 1){
        return translations[0].value;
    }
    else{
        return calculateNearest<glm::vec3>(time, translations);
    }
}

glm::quat BoneAnimation::calculateNearestRotation(float time) {
    if(rotations.size() == 0){
        throw AnimationInterpolationError("Tried to calculate current keyframe from an empty keyframes vector");
    }
    else if (rotations.size() == 1){
        return rotations[0].value;
    }
    else{
        return calculateNearest<glm::quat>(time, rotations);
    }
}

glm::vec3 BoneAnimation::calculateNearestScaling(float time) {
    if(scalings.size() == 0){
        throw AnimationInterpolationError("Tried to calculate current keyframe from an empty keyframes vector");
    }
    else if (scalings.size() == 1){
        return scalings[0].value;
    }
    else{
        return calculateNearest<glm::vec3>(time, scalings);
    }
}

/* Explicit templates specialisation allowing template definition in a separate file.
 * */

template
glm::vec3 SkeletalSystem::BoneAnimation::calculateCurrent<glm::vec3>(float time,
                                                                     std::vector<KeyFrame<glm::vec3>> &keyframes);

template
glm::quat SkeletalSystem::BoneAnimation::calculateCurrent<glm::quat>(float time,
                                                                     std::vector<KeyFrame<glm::quat>> &keyframes);

template
glm::vec3 SkeletalSystem::BoneAnimation::calculateInterpolated<glm::vec3>(float time,
                                                                          std::vector<KeyFrame<glm::vec3>> &keyframes);

template
glm::quat SkeletalSystem::BoneAnimation::calculateInterpolated<glm::quat>(float time,
                                                                          std::vector<KeyFrame<glm::quat>> &keyframes);

template
glm::vec3 SkeletalSystem::BoneAnimation::calculateNearest<glm::vec3>(float time,
                                                                     std::vector<KeyFrame<glm::vec3>> &keyframes);

template
glm::quat SkeletalSystem::BoneAnimation::calculateNearest<glm::quat>(float time,
                                                                     std::vector<KeyFrame<glm::quat>> &keyframes);