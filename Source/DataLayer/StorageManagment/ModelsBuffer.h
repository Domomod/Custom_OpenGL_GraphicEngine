//
// Created by dominik on 20.03.19.
//

#ifndef GAMEENGINE_MODELSBUFFER_H
#define GAMEENGINE_MODELSBUFFER_H

#include <vector>
#include <glm/glm.hpp>

/*This class stores 4x4 matrices contiguous to each other in one block of memory. It's motivation is the possibilty of
 * sending them to GPU without the need to bake the data.
 * Class provides an abstraction of accessing the models, allowing outer objects to point to an element of the vector,
 * and if the data gets sorted or shuffled, the pointers are rearranged to point to the same objects as before. This was
 * motivated by the ability of an outer object to own the matrix as it's own. The amount of objects pointing to a single
 * elements is unrestricted.
 * */
class ModelsBuffer {
public:
    glm::mat4** addModel(const glm::mat4 &mat4){
        matrices.push_back(mat4);
        proxy.push_back( &*std::prev(matrices.end()) );
        proxyId.push_back(matrices.size());
        return &*std::prev(proxy.end());
    }

    const std::vector<glm::mat4> &getMatrices() const {
        return matrices;
    }

private:
    std::vector<glm::mat4*> proxy;
    std::vector<unsigned long > proxyId;
    std::vector<glm::mat4> matrices;
};


#endif //GAMEENGINE_MODELSBUFFER_H
