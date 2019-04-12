//
// Created by dominik on 20.03.19.
//

#ifndef GAMEENGINE_MESHTOMODELMAP_H
#define GAMEENGINE_MESHTOMODELMAP_H

#include <map>
#include <memory>
#include <vector>

#include "Source/MyExceptions.h"
#include "Source/DataLayer/DataTypes/Assets/Mesh.h"
#include "ModelMatricesBuffer.h"

/* This class stores pointers to Meshes and maps them to vectors of Models, it's use case is when user want's to render,
 * an array of indexed objects.
 * Disclaimer:
 * It' purpouse is to map meshes to models, not to allocate or store meshes.
 * */
class ModelToModelSpaceMatricesMap {
public:
    void addModel(const std::string &modelName){
        modelToModelSpaceMatricessMap.emplace(modelName, ModelMatricesBuffer());
    }

    glm::mat4** allocateModel(const std::string& modelName){
        auto iter = modelToModelSpaceMatricessMap.find(modelName);
        if (iter != modelToModelSpaceMatricessMap.end()){
            auto& modelBuffer = (*iter).second;
            auto allocatedModel = modelBuffer.addModelMatrix(glm::mat4(1));
            return allocatedModel;
        } else {
            throw MeshNotFound("Mesh to model map knows nothing of such a model, it might have been loaded but wasn't added to the mapper.");
        }
    }

    const std::vector<glm::mat4>& getModelsByReference(const std::string& modelName) const{
        auto iter = modelToModelSpaceMatricessMap.find(modelName);
        if (iter != modelToModelSpaceMatricessMap.end()){
            return iter->second.getMatrices();
        } else {
            throw MeshNotFound(
                    "Mesh to model map knows nothing of such a model, it might have been loaded but wasn't added to the mapper.");
        }
    }

private:
    std::map<std::string, ModelMatricesBuffer> modelToModelSpaceMatricessMap;
};


#endif //GAMEENGINE_MESHTOMODELMAP_H
