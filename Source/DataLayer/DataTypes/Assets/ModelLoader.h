//
// Created by dominik on 28.03.19.
//

#ifndef GAMEENGINE_MODELLOADER_H
#define GAMEENGINE_MODELLOADER_H

#include <memory>
#include <assimp/scene.h>
#include <map>

#include "Model.h"

/* Assimp is able to load whole scenes containing hierarchy of meshes etc ... (formats 3ds, collada)
 * I decided the Game Engine will only load single Models, and leave the scene making
 * to the engine. Therefore my engine is assuming loaded model is a single model.
 *
 * ModelLoader uses additional Loader Classes to do the actual loading.
 * */
class ModelLoader {
public:
    static std::shared_ptr<Model> loadModel(const std::string &path);
private:
    /*Loader State*/
    inline static const aiScene* scene;
};



#endif //GAMEENGINE_MODELLOADER_H
