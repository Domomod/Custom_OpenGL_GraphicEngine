//
// Created by dominik on 05.05.19.
//

#ifndef GAMEENGINE_MATERIALSLOADER_H
#define GAMEENGINE_MATERIALSLOADER_H

#include <vector>
#include <memory>
#include <assimp/scene.h>

#include "Model.h"

#include "Textures/Texture.h"
#include "Textures/TextureLoader.h"

class MaterialsLoader  {
public:
    std::vector< std::shared_ptr<Texture> > normalMaps;
    std::vector< std::shared_ptr<Texture> > AOMaps;
    std::vector< std::shared_ptr<Texture> > albedoMaps;
    std::vector< std::shared_ptr<Texture> > metallnessMaps;
    std::vector< std::shared_ptr<Texture> > roughnessMaps;

    void loadMaterials();

    void setScene(const aiScene *scene);

    void setDirectory(const std::string &directory);

private:

    void loadMaterial(aiTextureType textureType, std::vector<std::shared_ptr<Texture>> &destination);

    const aiScene* scene;
    std::string directory;
};


#endif //GAMEENGINE_MATERIALSLOADER_H
