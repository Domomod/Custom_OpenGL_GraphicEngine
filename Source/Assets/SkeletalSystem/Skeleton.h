//
// Created by dominik on 11.04.19.
//

#ifndef GAMEENGINE_SKELETON_H
#define GAMEENGINE_SKELETON_H

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>


namespace SkeletalSystem
{
    class SkeletonLoader;

/* A bone represents a transformation
 * */
    struct Bone
    {
        /* If a bone has no name, this means it's not a real bone (no vertices connected to it),
         * and it was only used by the artist for easier rigging of the model.
         * By using a bone name we can find corresponding animation's transformation.
         * */
        int idx;
        std::string name;
        /* Transformation in realtion to parent
         * */
        glm::mat4 toParentSpaceMatrix;
        /* Transformation from mesh space to bone space.
         * */
        glm::mat4 offset;
        std::shared_ptr<Bone> Parent;
        std::vector<std::shared_ptr<Bone>> Children;
    };


/* By traversing a SkeletalSystem (also called a bone hierarchy/ bone tree), we can calculate the final transformations
 * that we will load into the shader.
 * */
    class Skeleton
    {
        friend class SkeletonLoader;

    public:
        const glm::mat4 &getGlobalInverseTransformation() const;

        const std::shared_ptr<Bone> &getRootBone() const;

    private:
        std::shared_ptr<Bone> rootBone;
        glm::mat4 globalInverseTransformation;
    };

}
#endif //GAMEENGINE_SKELETON_H
