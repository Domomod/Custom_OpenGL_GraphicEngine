//
// Created by dominik on 23.04.19.
//

#ifndef GAMEENGINE_MESHLOADER_H
#define GAMEENGINE_MESHLOADER_H

#include <memory>
#include <map>
#include <vector>

#include "MeshFactory.h"

class Mesh;

class aiMesh;

class Texture2D;

class MaterialsLoader;

/* Mesh Loader is designed to load one Mesh at a time. It can load
 * multiple meshes one after another. Loaded mesh is owned by the loader
 * until released to the system by make() function. After that MeshLoader
 * goes back to the state after initialisation, therefore it is not possible
 * to return multiple copies of the same mesh by calling make multiple times.
 * This way it's easier to assure that functions are called in right order.
 * */
class MeshLoader
{
public:
    MeshLoader();

    void loadBasicMeshInfo(const aiMesh *aMesh);

    void addBoneInfo(const std::map<std::__cxx11::string, int> &boneNameToIndexMap);

    void addMaterial(const MaterialsLoader &materialLoader, unsigned int matID);

    void addNormalTextures(const std::vector<std::shared_ptr<Texture2D>> &textures);

    void addAOTextures(const std::vector<std::shared_ptr<Texture2D>> &textures);

    void addBaseColorTexture(const std::vector<std::shared_ptr<Texture2D>> &textures);

    void addMetallnessTexture(const std::vector<std::shared_ptr<Texture2D>> &textures);

    void addRoughnessTexture(const std::vector<std::shared_ptr<Texture2D>> &textures);

    std::shared_ptr<Mesh> make();

private:

    MeshFactory meshFactory;
    unsigned int matId;
    const aiMesh *assimpMesh;
};

#endif //GAMEENGINE_MESHLOADER_H
