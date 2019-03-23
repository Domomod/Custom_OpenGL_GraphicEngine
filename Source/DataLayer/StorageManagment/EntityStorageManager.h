//
// Created by dominik on 21.03.19.
//

#ifndef GAMEENGINE_ENTITYSTORAGEMANAGER_H
#define GAMEENGINE_ENTITYSTORAGEMANAGER_H

#include <map>
#include <memory>

#include "StorageManagerTemplate.h"
#include "Source/DataLayer/DataTypes/GameLogic/Entity.h"

class EntityStorageManager : public StorageManagerTemplate<Entity>{
};


#endif //GAMEENGINE_ENTITYSTORAGEMANAGER_H
