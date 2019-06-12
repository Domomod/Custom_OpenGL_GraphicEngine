//
// Created by dominik on 12.04.19.
//

#ifndef GAMEENGINE_MODEL_H
#define GAMEENGINE_MODEL_H

#include <vector>
#include <memory>

class Mesh;

class Texture2D;

namespace SkeletalSystem
{
    class SkeletalAnimator;

    class SkeletalAnimation;

    class Skeleton;
}

class Model
{
public:
    Model()
    {
    }

    std::vector<std::shared_ptr<Mesh> > meshes;

    /* A Model might or might not have a Skeletal system
     * */
    bool hasSkeletonAndAnimation = false;
    std::shared_ptr<SkeletalSystem::SkeletalAnimator> animator;
    std::shared_ptr<SkeletalSystem::Skeleton> skeleton;
    std::vector<std::shared_ptr<SkeletalSystem::SkeletalAnimation>> skeletalAnimations;
};

class ModelFactory
{
public:
    ModelFactory &addMesh(const std::shared_ptr<Mesh> &mesh);

    std::shared_ptr<Model> make();

private:
    std::vector<std::shared_ptr<Mesh> > meshes;
    bool addedMesh = false;
};

#endif //GAMEENGINE_MODEL_H
