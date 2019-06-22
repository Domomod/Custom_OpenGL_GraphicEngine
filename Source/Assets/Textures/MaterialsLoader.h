//
// Created by dominik on 05.05.19.
//

#ifndef GAMEENGINE_MATERIALSLOADER_H
#define GAMEENGINE_MATERIALSLOADER_H

#include "TextureLoader.h"

#include <vector>
#include <memory>
#include <TinyXML2/tinyxml2.h>

class aiScene;
class aiMaterial;

class Material;
class Model;
class Texture2D;

namespace tinyxml2{
    class XMLElement;
}

class MaterialsLoader  {
public:

    const std::vector<std::shared_ptr<Material>> &getMaterials() const;

    void loadMaterials(tinyxml2::XMLElement * multipleMaterialsXML);

    void clear();

private:
    TextureLoader textureLoader;

    void loadMaterialPBR(tinyxml2::XMLElement *materialXML);
    void loadMaterialPHONG(tinyxml2::XMLElement *materialXML);

    std::vector< std::shared_ptr<Material> > materials;
};


#endif //GAMEENGINE_MATERIALSLOADER_H
