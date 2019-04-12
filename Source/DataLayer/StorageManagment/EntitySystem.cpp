//
// Created by dominik on 21.03.19.
//

#include "EntitySystem.h"

void EntitySystem::addEntity(const std::string &entityName, const std::shared_ptr<Entity> & entityPtr) {
    entityStorageManager.addObject(entityName,entityPtr);
}

void EntitySystem::removeEntity(const std::string &entityName) {
}

void EntitySystem::addModel(const std::string &modelName, const std::shared_ptr<Model> &modelPtr) {
    modelStorageManager.addObject(modelName, modelPtr);
    modelToModelSpaceMatriceslMap.addModel(modelName);
}

std::shared_ptr<Model> EntitySystem::getModel(const std::string &modelName) {
    return modelStorageManager.getObject(modelName);
}


const std::vector<glm::mat4> * EntitySystem::getAllFromModelSpaceMatricesForModel(const std::string &meshName) const {
    return &modelToModelSpaceMatriceslMap.getModelsByReference(meshName);
}

EntitySystem::EntitySystem() {
    entityFactory.setAssociatedMeshStorageManager(modelStorageManager);
    entityFactory.setAssociatedMeshToModelMap(modelToModelSpaceMatriceslMap);
}

EntitySystem::~EntitySystem() {

}