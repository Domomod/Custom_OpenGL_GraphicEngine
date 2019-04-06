//
// Created by dominik on 20.03.19.
//

#ifndef GAMEENGINE_MESHSTORAGEMANAGER_H
#define GAMEENGINE_MESHSTORAGEMANAGER_H

#include "StorageManagerTemplate.h"
#include "Source/DataLayer/DataTypes/Assets/Mesh.h"

/*This class stores Meshes which can be later accesed via a shared pointers, thus preventing storing the same Mesh in two
 * places in memory.
 * */

class MeshStorageManager : public StorageManagerTemplate<Mesh>{
};


#endif //GAMEENGINE_MESHSTORAGEMANAGER_H
