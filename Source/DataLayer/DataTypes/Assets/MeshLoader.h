//
// Created by dominik on 23.04.19.
//

#ifndef GAMEENGINE_MESHLOADER_H
#define GAMEENGINE_MESHLOADER_H

#include <memory>
#include <assimp/scene.h>
#include <map>
#include "Model.h"

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
