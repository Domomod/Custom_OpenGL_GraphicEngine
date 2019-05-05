//
// Created by dominik on 05.05.19.
//

#include "MaterialsLoader.h"

#include "AssimpConversion.h"

void MaterialsLoader::loadMaterials() {
    normalMaps.clear();
    AOMaps.clear();
    albedoMaps.clear();
    metallnessMaps.clear();
    roughnessMaps.clear();

    loadMaterial(aiTextureType_NORMALS , normalMaps);
    loadMaterial(aiTextureType_AMBIENT , AOMaps);
    loadMaterial(aiTextureType_DIFFUSE , albedoMaps);
    loadMaterial(aiTextureType_SPECULAR, metallnessMaps);
    loadMaterial(aiTextureType_EMISSIVE, roughnessMaps);
}

void MaterialsLoader::setScene(const aiScene *scene) {
    MaterialsLoader::scene = scene;
}

void MaterialsLoader::setDirectory(const std::string &directory) {
    MaterialsLoader::directory = directory;
}

void MaterialsLoader::loadMaterial(aiTextureType textureType, std::vector< std::shared_ptr<Texture> > &destination) {
    for(int idx = 0; idx < scene->mNumMaterials; idx++){
        aiMaterial* material = scene->mMaterials[idx];
        if(material->GetTextureCount(textureType)){
            aiString path;
            if(AI_SUCCESS == material->GetTexture(textureType, 0,
                                                  &path, nullptr, nullptr, nullptr, nullptr, nullptr)){
                std::__cxx11::string stdPath = assimpToEngine(path);
                std::__cxx11::string fullPath = directory;
#ifdef __linux__
                fullPath += "/";
                replace(stdPath.begin(), stdPath.end(), '\\', '/');
#elif _WIN32
                fullPath += "\\";
                std::replace(stdPath.begin(), stdPath.end(), '/', '\\');
#else
#error "OS not supported"
#endif
                fullPath += stdPath;

                destination.push_back(TextureLoader::loadTexture(fullPath));
            } else {
                /* If model does not have this texture load default texture
                * */
                destination.push_back(TextureLoader::getDefaultTexture());
            }
        } else {
            /* If model does not have this texture load default texture
            * */
            destination.push_back(TextureLoader::getDefaultTexture());
        }
    }
}
