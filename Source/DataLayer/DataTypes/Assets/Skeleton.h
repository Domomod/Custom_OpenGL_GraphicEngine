//
// Created by dominik on 11.04.19.
//

#ifndef GAMEENGINE_SKELETON_H
#define GAMEENGINE_SKELETON_H

#include <string>
#include <glm/glm.hpp>

/* A bone represents a transformation
 * */
struct Bone{
    /* If a bone has no name, this means it's not a real bone (no vertices coneccted to it),
     * and it was only used by the artist for easier rigging of the model.
     * */
    std::string name;
    glm::mat4 toParentSpaceMatrix;
    Bone* Parent;
    Bone** Children;
};


/* By traversing a Skeleton (also called a bone hierarchy/ bone tree), we can calculate the final transformations
 * that we will load into the shader.
 * */
class Skeleton {
public:
    Bone rootBone;
private:
};


#endif //GAMEENGINE_SKELETON_H
