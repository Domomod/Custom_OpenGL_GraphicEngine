//
// Created by dominik on 21.03.19.
//

#include "EntitySystem.h"

void EntitySystem::addEntity(const std::string &entityName, const std::shared_ptr<Entity> &entityPtr)
{
    entityStorageManager.addObject(entityName, entityPtr);
}

void EntitySystem::removeEntity(const std::string &entityName)
{
}

void EntitySystem::addModel(const std::string &modelName, const std::shared_ptr<Model> &modelPtr)
{
    modelStorageManager.addObject(modelName, modelPtr);
}

std::shared_ptr<Entity> EntitySystem::getEntity(const std::string &entityName)
{
    return entityStorageManager.getObject(entityName);
}

std::shared_ptr<Model> EntitySystem::getModel(const std::string &modelName)
{
    return modelStorageManager.getObject(modelName);
}

EntitySystem::EntitySystem()
{
    entityFactory.setAssociatedMeshStorageManager(modelStorageManager);
}

EntitySystem::~EntitySystem()
{

}