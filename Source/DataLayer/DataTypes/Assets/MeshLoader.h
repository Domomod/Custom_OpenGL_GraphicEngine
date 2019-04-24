//
// Created by dominik on 23.04.19.
//

#ifndef GAMEENGINE_MESHLOADER_H
#define GAMEENGINE_MESHLOADER_H

#include <memory>
#include <assimp/scene.h>
#include <map>
#include "Model.h"

/* Mesh Loader is designed to load one Mesh at a time. It can load
 * multiple meshes one after another. Loaded mesh is owned by the loader
 * until released to the system by make() function. After that MeshLoader
 * goes back to the state after initialisation, therefore it is not possible
 * to return multiple copies of the same mesh by calling make multiple times.
 * This way it's easier to assure that functions are called in right order.
 * */
class MeshLoader{
public:
    explicit MeshLoader(const aiScene *scene);

    void loadBasicMeshInfo(const aiMesh *aMesh);
    void addBoneInfo(const std::map<std::__cxx11::string, int> &boneNameToIndexMap);
    std::shared_ptr<Mesh> make();
private:
    std::shared_ptr<Mesh> constructedMesh;
    bool isMeshInitialised(){ return constructedMesh != nullptr; }

    const aiScene* scene;
    const aiMesh* assimpMesh;
};

#endif //GAMEENGINE_MESHLOADER_H
