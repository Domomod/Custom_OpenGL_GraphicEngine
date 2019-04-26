//
// Created by dominik on 23.04.19.
//

#include "ModelLoader.h"
#include "AssimpConversion.h"
#include <glm/gtc/type_ptr.hpp>


std::string assimpToEngine(const aiString &assimpString) {
    return std::string(assimpString.C_Str());
}

