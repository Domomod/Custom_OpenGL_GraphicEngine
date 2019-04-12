//
// Created by dominik on 28.03.19.
//

#ifndef GAMEENGINE_MESHLOADER_H
#define GAMEENGINE_MESHLOADER_H

#include <memory>
#include <assimp/scene.h>

#include "Model.h"

class ModelLoader {
public:
    static std::shared_ptr<Model> loadModel(const std::string &path);
private:
    static void loadMesh(const std::shared_ptr<Model> &returnModel, const aiScene *scene);
};


#endif //GAMEENGINE_MESHLOADER_H
