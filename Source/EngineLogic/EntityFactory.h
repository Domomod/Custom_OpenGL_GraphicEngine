//
// Created by dominik on 22.03.19.
//

#ifndef GAMEENGINE_ENTITYFACTORY_H
#define GAMEENGINE_ENTITYFACTORY_H

#include <memory>
#include <string>
#include <glm/glm.hpp>

class Entity;

class ModelStorageManager;

class ModelToModelSpaceMatricesMap;


class EntityFactory
{
    friend class EntitySystem;

public:
    std::shared_ptr<Entity>
    make(const std::string &modelName, const glm::vec3 &position, const glm::vec3 &rotation = glm::vec3(0.f, 0.f, 0.f),
         const glm::vec3 &scaling = glm::vec3(1.f, 1.f, 1.f)) const;

    std::shared_ptr<Entity> make(const std::string &modelName, const glm::vec3 &position, const float scaling,
                                 const glm::vec3 &rotation = glm::vec3(0.f, 0.f, 0.f)) const;

private:
    void setAssociatedMeshStorageManager(ModelStorageManager &associatedMeshStorageManager);

public:
    void setAssociatedMeshToModelMap(ModelToModelSpaceMatricesMap &associatedMeshToModelMap);

private:

    ModelStorageManager *associatedMeshStorageManager;
    ModelToModelSpaceMatricesMap *associatedMeshToModelMap;

};


#endif //GAMEENGINE_ENTITYFACTORY_H
