//
// Created by dominik on 22.03.19.
//

#include "EntityFactory.h"

std::shared_ptr<Entity> EntityFactory::make(const std::string &modelName, const glm::vec3 &position) const {
    std::shared_ptr<Model> model = associatedMeshStorageManager->getObject(modelName);
    return std::make_shared<Entity>(
            model,
            associatedMeshToModelMap->allocateModel(modelName),
            position
            );
}

void EntityFactory::setAssociatedMeshStorageManager(ModelStorageManager &associatedMeshStorageManager) {
    EntityFactory::associatedMeshStorageManager = &associatedMeshStorageManager;
}

void EntityFactory::setAssociatedMeshToModelMap(ModelToModelSpaceMatricesMap &associatedMeshToModelMap) {
    EntityFactory::associatedMeshToModelMap = &associatedMeshToModelMap;
}
