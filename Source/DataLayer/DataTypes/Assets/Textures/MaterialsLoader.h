//
// Created by dominik on 05.05.19.
//

#ifndef GAMEENGINE_MATERIALSLOADER_H
#define GAMEENGINE_MATERIALSLOADER_H

#include <vector>
#include <memory>

class aiScene;
class aiMaterial;

class Model;
class Texture;

namespace tinyxml2{
    class XMLElement;
}

class MaterialsLoader  {
public:
    std::vector< std::shared_ptr<Texture> > normalMaps;
    std::vector< std::shared_ptr<Texture> > ambientMaps;
    std::vector< std::shared_ptr<Texture> > albedoMaps;
    std::vector< std::shared_ptr<Texture> > metalnessMaps;
    std::vector< std::shared_ptr<Texture> > roughnessMaps;

    void loadMaterials();
    void loadMaterial(const std::string & filePath);

    void setScene(const aiScene *scene);

    void setDirectory(const std::string &directory);

private:

    void loadMaterialAtribute(std::vector<std::shared_ptr<Texture> > &atributeVector,
                              const std::string &textureFileName);
    void loadMaterialXML(aiMaterial * assimpMaterial);

    const aiScene* scene;
    std::string directory;

    std::string getPathToMaterial(const aiMaterial *assimpMaterial) const;

    std::string getPathToMaterialTexture(const std::string & textureFileName) const;

    std::string elementTextToString(tinyxml2::XMLElement *xmlElement) const;
};


#endif //GAMEENGINE_MATERIALSLOADER_H
