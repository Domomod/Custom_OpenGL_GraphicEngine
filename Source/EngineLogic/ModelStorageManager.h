//
// Created by dominik on 20.03.19.
//

#ifndef GAMEENGINE_MESHSTORAGEMANAGER_H
#define GAMEENGINE_MESHSTORAGEMANAGER_H

#include "StorageManagerTemplate.h"
#include "Assets/Model.h"

/*This class stores Meshes which can be later accesed via a shared pointers, thus preventing storing the same Mesh in two
 * places in memory.
 * */

class ModelStorageManager : public StorageManagerTemplate<Model>
{
};


#endif //GAMEENGINE_MESHSTORAGEMANAGER_H
