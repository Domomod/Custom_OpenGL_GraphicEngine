//
// Created by dominik on 22.03.19.
//

#include "EntityFactory.h"

#include "Entity.h"
#include "ModelStorageManager.h"

std::shared_ptr<Entity>
EntityFactory::make(const std::string &modelName, const glm::vec3 &position, const glm::vec3 &rotation,
                    const glm::vec3 &scaling) const
{
    std::shared_ptr<Model> model = associatedMeshStorageManager->getObject(modelName);
    return std::make_shared<Entity>(
            model,
            position,
            rotation,
            scaling
    );
}

std::shared_ptr<Entity>
EntityFactory::make(const std::string &modelName, const glm::vec3 &position, const float scaling,
                    const glm::vec3 &rotation) const
{
    std::shared_ptr<Model> model = associatedMeshStorageManager->getObject(modelName);
    return std::make_shared<Entity>(
            model,
            position,
            rotation,
            glm::vec3(scaling)
    );
}

void EntityFactory::setAssociatedMeshStorageManager(ModelStorageManager &associatedMeshStorageManager)
{
    EntityFactory::associatedMeshStorageManager = &associatedMeshStorageManager;
}



