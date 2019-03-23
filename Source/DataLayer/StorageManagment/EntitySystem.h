//
// Created by dominik on 21.03.19.
//

#ifndef GAMEENGINE_ENTITYSYSTEM_H
#define GAMEENGINE_ENTITYSYSTEM_H

#include <string>
#include <Source/DataLayer/DataTypes/GameLogic/EntityFactory.h>

#include "EntityStorageManager.h"
#include "MeshStorageManager.h"
#include "MeshToModelMap.h"
#include "Source/DataLayer/DataTypes/Geometry/MeshGenerator.h"

class EntitySystem {
public:
    EntitySystem();

    virtual ~EntitySystem();

    void addEntity(const std::string& entityName, const std::shared_ptr<Entity> & entityPtr);

    void removeEntity(const std::string& entityName);

    void addMesh(const std::string &meshName, const std::shared_ptr<Mesh> & meshPtr);

    std::shared_ptr<Mesh> getMesh(const std::string &meshName);

    const std::vector<glm::mat4> * getAllModelsForMesh(const std::string& meshName) const;

    MeshGenerator meshGenerator;
    EntityFactory entityFactory;
private:
    EntityStorageManager entityStorageManager;
    MeshStorageManager meshStorageManager;
    MeshToModelMap meshToModelMap;
};


#endif //GAMEENGINE_ENTITYSYSTEMMEDIATOR_H
