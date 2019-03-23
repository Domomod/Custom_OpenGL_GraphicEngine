//
// Created by dominik on 22.03.19.
//

#ifndef GAMEENGINE_ENTITYFACTORY_H
#define GAMEENGINE_ENTITYFACTORY_H

#include <memory>
#include <string>
#include "Entity.h"
#include "Source/DataLayer/StorageManagment/MeshStorageManager.h"
#include "Source/DataLayer/StorageManagment/MeshToModelMap.h"

class EntityFactory {
    friend class EntitySystem;
public:
    std::shared_ptr<Entity> make(const std::string &meshName, const glm::vec3 &position) const;

private:
    void setAssociatedMeshStorageManager(MeshStorageManager &associatedMeshStorageManager);

public:
    void setAssociatedMeshToModelMap(MeshToModelMap &associatedMeshToModelMap);

private:

    MeshStorageManager* associatedMeshStorageManager;
    MeshToModelMap* associatedMeshToModelMap;

};


#endif //GAMEENGINE_ENTITYFACTORY_H
