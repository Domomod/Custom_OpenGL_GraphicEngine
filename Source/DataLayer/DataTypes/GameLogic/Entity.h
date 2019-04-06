#include <utility>

//
// Created by dominik on 21.03.19.
//

#ifndef GAMEENGINE_ENTITY_H
#define GAMEENGINE_ENTITY_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Source/DataLayer/DataTypes/Assets/Mesh.h"

/*This class stores the most basic object in Game Logic an Entity.
 * An entity represents an in game object in both the physics system and render system.
 * */

class Entity {
public:
    Entity(std::shared_ptr<Mesh> mesh, glm::mat4 **modelMatrixPtrPtr, glm::vec3 position) : mesh(std::move(mesh)),
                                                                                            modelMatrixPtrPtr(modelMatrixPtrPtr),
                                                                                            position(position){
        **modelMatrixPtrPtr = glm::translate(**modelMatrixPtrPtr, position);
    }

    const Mesh getMesh() const {
        return *mesh;
    }

    const glm::mat4 getModelMatrixPtrPtr() const {
        return **modelMatrixPtrPtr;
    }

    void setMesh(const Mesh &mesh) {
        *Entity::mesh = mesh;
    }

    void setModelMatrixPtrPtr(glm::mat4 modelMatrixPtrPtr) {
        **Entity::modelMatrixPtrPtr = modelMatrixPtrPtr;
    }

private:
    std::shared_ptr<Mesh> mesh;
    glm::mat4** modelMatrixPtrPtr;
    glm::vec3 position;
};


#endif //GAMEENGINE_ENTITY_H
