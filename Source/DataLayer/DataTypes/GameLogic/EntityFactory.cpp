//
// Created by dominik on 22.03.19.
//

#include "EntityFactory.h"

std::shared_ptr<Entity> EntityFactory::make(const std::string &meshName, const glm::vec3 &position) const {
    std::shared_ptr<Mesh> mesh = associatedMeshStorageManager->getObject(meshName);
    return std::make_shared<Entity>(
            mesh,
            associatedMeshToModelMap->allocateModel(meshName),
            position
            );
}

void EntityFactory::setAssociatedMeshStorageManager(MeshStorageManager &associatedMeshStorageManager) {
    EntityFactory::associatedMeshStorageManager = &associatedMeshStorageManager;
}

void EntityFactory::setAssociatedMeshToModelMap(MeshToModelMap &associatedMeshToModelMap) {
    EntityFactory::associatedMeshToModelMap = &associatedMeshToModelMap;
}
