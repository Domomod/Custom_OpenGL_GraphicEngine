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

/*  Initial purpose of entity system was to behave like a smart database of loaded game objects, meshes etc.
 *  Also to store them in such a manner that it's fast to draw them instanced or not, and to modify them
 *  without the need to additionaly bake data. Atm this part is fully implemented.
 *
 *  It consists of samller classes that do the actual storing, sorting, acquiring.
 *
 *  TODO: fix those:
 *  TODO: acquiring an entity or mesh is not ideal yet as it's an O(log n) op.
 *  TODO: not baking data requires the user to refer to pointer to a pointer to modify model data.
 * */

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
