//
// Created by dominik on 21.03.19.
//

#ifndef GAMEENGINE_ENTITYSYSTEM_H
#define GAMEENGINE_ENTITYSYSTEM_H

#include <string>
#include <Source/DataLayer/DataTypes/GameLogic/EntityFactory.h>

#include "EntityStorageManager.h"
#include "ModelStorageManager.h"
#include "ModelToModelSpaceMatricesMap.h"
#include "Source/DataLayer/DataTypes/Assets/MeshGenerator.h"

/*  Initial purpose of entity system was to behave like a smart database of loaded game objects, meshes etc.
 *  Also to store them in such a manner that it's fast to draw them instanced or not, and to modify them
 *  without the need to additionaly bake data. Atm this part is fully implemented.
 *
 *  It consists of samller classes that do the actual storing, sorting, acquiring.
 *
 *  TODO: fix those:
 *  TODO: acquiring an entity or model is not ideal yet as it's an O(log n) op.
 *  TODO: not baking data requires the user to refer to pointer to a pointer to modify model data.
 * */

class EntitySystem {
public:
    EntitySystem();

    virtual ~EntitySystem();

    void addEntity(const std::string& entityName, const std::shared_ptr<Entity> & entityPtr);

    void removeEntity(const std::string& entityName);

    void addModel(const std::string &modelName, const std::shared_ptr<Model> &modelPtr);

    std::shared_ptr<Model> getModel(const std::string &modelName);

    const std::vector<glm::mat4> * getAllFromModelSpaceMatricesForModel(const std::string &meshName) const;

    EntityFactory entityFactory;
private:
    EntityStorageManager entityStorageManager;
    ModelStorageManager modelStorageManager;
    ModelToModelSpaceMatricesMap modelToModelSpaceMatriceslMap;
};


#endif //GAMEENGINE_ENTITYSYSTEMMEDIATOR_H
