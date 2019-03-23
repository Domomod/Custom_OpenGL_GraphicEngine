//
// Created by dominik on 21.03.19.
//

#include "EntitySystem.h"

void EntitySystem::addEntity(const std::string &entityName, const std::shared_ptr<Entity> & entityPtr) {
    entityStorageManager.addObject(entityName,entityPtr);
}

void EntitySystem::removeEntity(const std::string &entityName) {
}

void EntitySystem::addMesh(const std::string &meshName, const std::shared_ptr<Mesh> & meshPtr) {
    meshStorageManager.addObject(meshName, meshPtr);
    meshToModelMap.addMesh(meshName);
}

std::shared_ptr<Mesh> EntitySystem::getMesh(const std::string &meshName) {
    return meshStorageManager.getObject(meshName);
}


const std::vector<glm::mat4> * EntitySystem::getAllModelsForMesh(const std::string &meshName) const {
    return &meshToModelMap.getModelsByReference(meshName);
}

EntitySystem::EntitySystem() {
    entityFactory.setAssociatedMeshStorageManager(meshStorageManager);
    entityFactory.setAssociatedMeshToModelMap(meshToModelMap);
}

EntitySystem::~EntitySystem() {

}