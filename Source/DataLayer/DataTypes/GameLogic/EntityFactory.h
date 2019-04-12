//
// Created by dominik on 22.03.19.
//

#ifndef GAMEENGINE_ENTITYFACTORY_H
#define GAMEENGINE_ENTITYFACTORY_H

#include <memory>
#include <string>
#include "Entity.h"
#include "Source/DataLayer/StorageManagment/ModelStorageManager.h"
#include "Source/DataLayer/StorageManagment/ModelToModelSpaceMatricesMap.h"

class EntityFactory {
    friend class EntitySystem;
public:
    std::shared_ptr<Entity> make(const std::string &modelName, const glm::vec3 &position) const;

private:
    void setAssociatedMeshStorageManager(ModelStorageManager &associatedMeshStorageManager);

public:
    void setAssociatedMeshToModelMap(ModelToModelSpaceMatricesMap &associatedMeshToModelMap);

private:

    ModelStorageManager* associatedMeshStorageManager;
    ModelToModelSpaceMatricesMap* associatedMeshToModelMap;

};


#endif //GAMEENGINE_ENTITYFACTORY_H
