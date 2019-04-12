//
// Created by dominik on 11.04.19.
//

#ifndef GAMEENGINE_SKELETON_H
#define GAMEENGINE_SKELETON_H

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "SkeletonConstants.h"

namespace Skeleton {

/* A bone represents a transformation
 * */
    struct Bone {
        /* If a bone has no name, this means it's not a real bone (no vertices connected to it),
         * and it was only used by the artist for easier rigging of the model.
         * By using a bone name we can find corresponding animation's transformation.
         * */
        std::string name;
        glm::mat4 toParentSpaceMatrix;
        std::shared_ptr<Bone> Parent;
        std::vector<std::shared_ptr<Bone>> Children;
    };


/* By traversing a Skeleton (also called a bone hierarchy/ bone tree), we can calculate the final transformations
 * that we will load into the shader.
 * */
    class Skeleton {
    public:
    private:
        Bone rootBone;
        glm::mat4 finalTransformations[MAX_BONES];
    };

}
#endif //GAMEENGINE_SKELETON_H
