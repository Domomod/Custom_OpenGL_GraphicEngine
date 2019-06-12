//
// Created by dominik on 24.04.19.
//

#ifndef GAMEENGINE_SKELETONANIMATIONLOADER_H
#define GAMEENGINE_SKELETONANIMATIONLOADER_H

#include <memory>
#include <map>
#include <vector>

class aiAnimation;

namespace SkeletalSystem
{
    class SkeletalAnimation;

    class SkeletonAnimationLoader
    {
    public:
        void loadAnimation(aiAnimation *aAnimation, const std::map<std::__cxx11::string, int> &boneNameToIndexMap);

        std::shared_ptr<SkeletalSystem::SkeletalAnimation> make();

    private:
        template<class EngineKeyframe, class AssimpKeyframe>
        std::vector<EngineKeyframe> translateKeyframes(unsigned int numKeyframes, AssimpKeyframe *assimpKeyframes);

        void doCleanup();

        aiAnimation *assimpAnimation;
        std::shared_ptr<SkeletalSystem::SkeletalAnimation> constructedAnimation;
    };
}
#endif //GAMEENGINE_SKELETONANIMATIONLOADER_H
