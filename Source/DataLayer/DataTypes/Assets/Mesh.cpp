//
// Created by dominik on 23.02.19.
//

#include "Mesh.h"

#include "Source/MyExceptions.h"


void Mesh::addBoneData(int vertId, int boneId, float weight) {
    /* Each vertex might be affected by up to 4.
     * */
    for(int i = 0; i < 4; i ++){
        if(boneIds[vertId][i] == 0 ){
            boneIds[vertId][i] = boneId;
            boneWeights[vertId][i] = weight;
            return;
        }
    }
    throw MeshLoadingException("Vertex affected by more than 4 bones.");
}
