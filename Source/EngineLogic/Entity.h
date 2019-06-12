#include <utility>

//
// Created by dominik on 21.03.19.
//

#ifndef GAMEENGINE_ENTITY_H
#define GAMEENGINE_ENTITY_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model;


/*This class stores the most basic object in Game Logic an Entity.
 * An entity represents an in game object in both the physics system and render system.
 * */
class Entity {
public:
    Entity(std::shared_ptr<Model> model, glm::mat4 **modelMatrixPtrPtr,
            glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    const std::shared_ptr<Model> getModel() const;

    const glm::mat4 getModelSpaceMatrix() const;

    void setModel(const std::shared_ptr<Model> &model);

    void setModelMatrix(glm::mat4 modelMatrixPtrPtr);

private:
    std::shared_ptr<Model> model;
    glm::mat4** modelMatrixPtrPtr;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};


#endif //GAMEENGINE_ENTITY_H
