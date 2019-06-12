//
// Created by dominik on 05.05.19.
//

#include "MaterialsLoader.h"

#include <assimp/scene.h>
#include <TinyXML2/tinyxml2.h>

#include "Source/DataLayer/DataTypes/Assets/Model.h"
#include "Texture.h"
#include "TextureLoader.h"

#include "Source/DataLayer/DataTypes/Assets/AssimpConversion.h"

#include "Utility.h"


#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != tinyxml2::XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif


void MaterialsLoader::loadMaterials() {
    for (int idx = 0; idx < scene->mNumMaterials; idx++) {
        aiMaterial *material = scene->mMaterials[idx];
        loadMaterialXML(material);
    }
}


void MaterialsLoader::setScene(const aiScene *scene) {
    MaterialsLoader::scene = scene;
}


void MaterialsLoader::setDirectory(const std::string &directory) {
    MaterialsLoader::directory = directory;
}


    void MaterialsLoader::loadMaterialAtribute(std::vector<std::shared_ptr<Texture2D> > &atributeVector,
                                               const std::string &textureFileName) {

        if(textureFileName != ""){
            std::string fullPath = getPathToMaterialTexture(textureFileName);
            atributeVector.emplace_back(TextureLoader::loadTexture2D(fullPath));
        }
        else {
            atributeVector.push_back(TextureLoader::getDefaultTexture());
        }
    }


void MaterialsLoader::loadMaterialXML(aiMaterial * assimpMaterial) {
    std::string filePath = getPathToMaterial(assimpMaterial);
    loadMaterial(filePath);
}


std::string MaterialsLoader::elementTextToString(tinyxml2::XMLElement *xmlElement) const {
    using namespace tinyxml2;
    const char* elementText = xmlElement->GetText();
    if(elementText == nullptr){
        return "";
    }
    return elementText;
}


std::string MaterialsLoader::getPathToMaterial(const aiMaterial *assimpMaterial) const {
    aiString materialName;
    assimpMaterial->Get(AI_MATKEY_NAME, materialName);
#ifdef __linux__
    std::__cxx11::string filePath = directory + "/Materials/" + assimpToEngine(materialName) + ".Material";
#elif _WIN32
    std::string filePath = directory + "\\Materials\\" + assimpToEngine(materialName) + ".Material";
#else
#error "OS not supported"
#endif

    return filePath;
}

std::string MaterialsLoader::getPathToMaterialTexture(const std::string &textureFileName) const {

#ifdef __linux__
    std::__cxx11::string filePath = directory + "/Textures/" + textureFileName;
#elif _WIN32
    std::__cxx11::string filePath = directory + "\\Textures\\" + textureFileName;
#else
#error "OS not supported"
#endif
    return filePath;
}

void MaterialsLoader::loadMaterial(const std::string &  filePath) {
    using namespace tinyxml2;

    XMLDocument xmlDocument;
    XMLError xmlResult = xmlDocument.LoadFile(filePath.c_str());
    XMLCheckResult(xmlResult);

    XMLNode * xmlMaterial = xmlDocument.FirstChildElement("material");

    XMLElement * xmlAlbedo    = xmlMaterial->FirstChildElement("albedo");
    XMLElement * xmlAmbient   = xmlMaterial->FirstChildElement("ambient");
    XMLElement * xmlMetalness = xmlMaterial->FirstChildElement("metalness");
    XMLElement * xmlRoughness = xmlMaterial->FirstChildElement("roughness");
    XMLElement * xmlNormal    = xmlMaterial->FirstChildElement("normal");

    std::string albedoPath    = elementTextToString(xmlAlbedo);
    std::string ambientPath   = elementTextToString(xmlAmbient);
    std::string metalnessPath = elementTextToString(xmlMetalness);
    std::string roughnessPath = elementTextToString(xmlRoughness);
    std::string normalPath    = elementTextToString(xmlNormal);

    loadMaterialAtribute(albedoMaps, albedoPath);
    loadMaterialAtribute(ambientMaps, ambientPath);
    loadMaterialAtribute(metalnessMaps, metalnessPath);
    loadMaterialAtribute(roughnessMaps, roughnessPath);
    loadMaterialAtribute(normalMaps, normalPath);
}

void MaterialsLoader::clear() {
    normalMaps.clear();
    ambientMaps.clear();
    albedoMaps.clear();
    metalnessMaps.clear();
    roughnessMaps.clear();
}


