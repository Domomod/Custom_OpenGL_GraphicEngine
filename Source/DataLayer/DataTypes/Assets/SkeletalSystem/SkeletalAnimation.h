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

    class SkeletonAnimationLoader;

    template<class T>
    struct KeyFrame {
        double timeStamp;
        T value;

        typedef T ValueType;

        KeyFrame() = default;

        KeyFrame(double timeStamp, T value) : timeStamp(timeStamp), value(value) {}
    };

    class BoneAnimation {
    public:
        BoneAnimation() = default;

        BoneAnimation(std::vector<KeyFrame<glm::vec3>> translations,
                      std::vector<KeyFrame<glm::quat>> rotation    ,
                      std::vector<KeyFrame<glm::vec3>> scaling     )
                : translations(std::move(translations)), rotations(std::move(rotation)), scalings(std::move(scaling)) {}

        glm::vec3 calculateInterpolatedTransaltions(float time);
        glm::quat calculateInterpolatedRotation(float time);
        glm::vec3 calculateInterpolatedScaling(float time);

    private:
        template<class T>
        T calculateCurrent(float time, std::vector<KeyFrame<T>> &keyframes);

        template<class T>
        T calculateInterpolated(float time, std::vector<KeyFrame<T>> &keyframes);

        glm::vec3 interpolation(const glm::vec3 & first, const glm::vec3 & second, float progression);
        glm::quat interpolation(const glm::quat & first, const glm::quat & second, float progression);

        std::vector<KeyFrame<glm::vec3>> translations;
        std::vector<KeyFrame<glm::quat>> rotations;
        std::vector<KeyFrame<glm::vec3>> scalings;
    };


    class SkeletalAnimation {
        friend class SkeletonAnimationLoader;
    public:
        BoneAnimation* findBoneAnimation(int id){
            auto iterator = idToBoneAnimMap.find(id);
            if(iterator != idToBoneAnimMap.end()){
                auto& boneAnim = (*iterator).second;
                return &boneAnim;
            }
            else {
                return nullptr;
            }
        }

        const std::string &getAnimationName() const;

        double getDurationInTicks() const;

        double getTicksPerSecond() const;

    private:
        std::string animationName;
        double durationInTicks;
        double ticksPerSecond;
        std::map<int,BoneAnimation> idToBoneAnimMap;
    };


    /* Template functions definitions
     * */




}

#endif //GAMEENGINE_SKELETALANIMATION_H
