//
// Created by dominik on 05.05.19.
//

#ifndef GAMEENGINE_MATERIALSLOADER_H
#define GAMEENGINE_MATERIALSLOADER_H

#include "TextureLoader.h"

#include <vector>
#include <memory>

class aiScene;
class aiMaterial;

class Model;
class Texture2D;

namespace tinyxml2{
    class XMLElement;
}

class MaterialsLoader  {
public:
    std::vector< std::shared_ptr<Texture2D> > normalMaps;
    std::vector< std::shared_ptr<Texture2D> > ambientMaps;
    std::vector< std::shared_ptr<Texture2D> > albedoMaps;
    std::vector< std::shared_ptr<Texture2D> > metalnessMaps;
    std::vector< std::shared_ptr<Texture2D> > roughnessMaps;

    void loadMaterials();
    void loadMaterial(const std::string & filePath);

    void setScene(const aiScene *scene);

    void setDirectory(const std::string &directory);

    void clear();

private:
    TextureLoader textureLoader;


    void loadMaterialAtribute(std::vector<std::shared_ptr<Texture2D> > &atributeVector,
                              const std::string &textureFileName);
    void loadMaterialXML(aiMaterial * assimpMaterial);

    const aiScene* scene;
    std::string directory;

    std::string getPathToMaterial(const aiMaterial *assimpMaterial) const;

    std::string getPathToMaterialTexture(const std::string & textureFileName) const;

    std::string elementTextToString(tinyxml2::XMLElement *xmlElement) const;
};


#endif //GAMEENGINE_MATERIALSLOADER_H
