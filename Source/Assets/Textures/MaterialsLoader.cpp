//
// Created by dominik on 05.05.19.
//

#include "MaterialsLoader.h"

#include <assimp/scene.h>
#include <TinyXML2/tinyxml2.h>

#include "Assets/UsedDirectories.h"
#include "Assets/myXML.h"
#include "Assets/Model.h"
#include "Material.h"
#include "Texture.h"
#include "TextureLoader.h"

#include "Assets/AssimpConversion.h"

#include "Utility.h"
#include "MyExceptions.h"


#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != tinyxml2::XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif

void MaterialsLoader::loadMaterials(tinyxml2::XMLElement *multipleMaterialsXML)
{
    using  namespace tinyxml2;

    XMLElement * materialPathXML = multipleMaterialsXML->FirstChildElement("material");
    while(materialPathXML != nullptr){
        XMLDocument documentXML;
        std::string test = (MATERIAL_DIRECTORY + elementTextToString(materialPathXML));
        XMLError error = documentXML.LoadFile( (MATERIAL_DIRECTORY + elementTextToString(materialPathXML)).c_str());
        XMLElement * materialXML = documentXML.FirstChildElement("material");
        if (error == XML_ERROR_FILE_NOT_FOUND)
        {
            throw FileNotFound("Could not load file: " + (GEOMETRY_DIRECTORY + elementTextToString(materialPathXML)));
        }
        else if (error != XML_SUCCESS)
        {
            throw MaterialLoadingException("Could not parse XML file: " + (GEOMETRY_DIRECTORY + elementTextToString(materialPathXML)));
        }

        std::string type = materialXML->Attribute("type");
        materialPathXML = materialPathXML->NextSiblingElement();
        if(type == "PBR_MR")
        {
            loadMaterialPBR(materialXML);
        }
        else if (type == "PHONG")
        {
            loadMaterialPHONG(materialXML);
        }
        else
        {
            throw MaterialLoadingException("Unsuported material type:\t" + type + "\n");
        }
    }
}


void MaterialsLoader::loadMaterialPBR(tinyxml2::XMLElement *materialXML)
{
    using  namespace tinyxml2;

    XMLElement * albedoXML    = materialXML->FirstChildElement("albedo");
    XMLElement * ambientXML   = materialXML->FirstChildElement("ambient");
    XMLElement * metalnessXML = materialXML->FirstChildElement("metalness");
    XMLElement * roughnessXML = materialXML->FirstChildElement("roughness");
    XMLElement * normalXML    = materialXML->FirstChildElement("normal");

    std::string albedoPath    = TEXTURE_DIRECTORY + elementTextToString(albedoXML   );
    std::string ambientPath   = TEXTURE_DIRECTORY + elementTextToString(ambientXML  );
    std::string metalnessPath = TEXTURE_DIRECTORY + elementTextToString(metalnessXML);
    std::string roughnessPath = TEXTURE_DIRECTORY + elementTextToString(roughnessXML);
    std::string normalPath    = TEXTURE_DIRECTORY + elementTextToString(normalXML   );

    std::shared_ptr<Texture2D> albedoMap    = textureLoader.loadTexture2D(albedoPath   );
    std::shared_ptr<Texture2D> ambientMap   = textureLoader.loadTexture2D(ambientPath  );
    std::shared_ptr<Texture2D> metalnessMap = textureLoader.loadTexture2D(metalnessPath);
    std::shared_ptr<Texture2D> roughnessMap = textureLoader.loadTexture2D(roughnessPath);
    std::shared_ptr<Texture2D> normalMap    = textureLoader.loadTexture2D(normalPath   );

    this->materials.push_back(
            std::shared_ptr<Material>(new MaterialPBR(albedoMap,ambientMap,metalnessMap,roughnessMap,normalMap))
            );

}


void MaterialsLoader::loadMaterialPHONG(tinyxml2::XMLElement *materialXML)
{
    using  namespace tinyxml2;

    XMLElement * colorXML    = materialXML->FirstChildElement("color");
    XMLElement * normalXML   = materialXML->FirstChildElement("normal");

    std::string colorPath    = TEXTURE_DIRECTORY + elementTextToString(colorXML   );
    std::string normalPath   = TEXTURE_DIRECTORY + elementTextToString(normalXML  );

    std::shared_ptr<Texture2D> colorMap    = textureLoader.loadTexture2D(colorPath   );
    std::shared_ptr<Texture2D> normalMap   = textureLoader.loadTexture2D(normalPath  );

    this->materials.push_back(
            std::shared_ptr<Material>(new MaterialPHONG(colorMap,normalMap))
            );
}


void MaterialsLoader::clear()
{
    materials.clear();
}

const std::vector<std::shared_ptr<Material>> &MaterialsLoader::getMaterials() const
{
    return materials;
}
