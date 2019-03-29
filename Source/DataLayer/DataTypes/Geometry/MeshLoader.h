//
// Created by dominik on 28.03.19.
//

#ifndef GAMEENGINE_MESHLOADER_H
#define GAMEENGINE_MESHLOADER_H

#include <memory>

#include "Mesh.h"

class MeshLoader {
public:
    static std::shared_ptr<Mesh> loadMesh(const std::string &path);
private:
};


#endif //GAMEENGINE_MESHLOADER_H
