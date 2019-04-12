#include <utility>

//
// Created by dominik on 21.03.19.
//

#ifndef GAMEENGINE_ENTITY_H
#define GAMEENGINE_ENTITY_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Source/DataLayer/DataTypes/Assets/Model.h"

/*This class stores the most basic object in Game Logic an Entity.
 * An entity represents an in game object in both the physics system and render system.
 * */

class Entity {
public:
    Entity(std::shared_ptr<Model> model, glm::mat4 **modelMatrixPtrPtr, glm::vec3 position) : model(std::move(model)),
                                                                                            modelMatrixPtrPtr(modelMatrixPtrPtr),
                                                                                            position(position){
        **modelMatrixPtrPtr = glm::translate(**modelMatrixPtrPtr, position);
    }

    const std::shared_ptr<Model> getModel() const {
        return model;
    }

    const glm::mat4 getModelSpaceMatrixPtrPtr() const {
        return **modelMatrixPtrPtr;
    }

    void setModel(const std::shared_ptr<Model> &model) {
        Entity::model = model;
    }

    void setModelMatrixPtrPtr(glm::mat4 modelMatrixPtrPtr) {
        **Entity::modelMatrixPtrPtr = modelMatrixPtrPtr;
    }

private:
    std::shared_ptr<Model> model;
    glm::mat4** modelMatrixPtrPtr;
    glm::vec3 position;
};


#endif //GAMEENGINE_ENTITY_H
