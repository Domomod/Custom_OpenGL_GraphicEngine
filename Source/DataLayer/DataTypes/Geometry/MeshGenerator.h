//
// Created by dominik on 22.02.19.
//

#ifndef GAMEENGINE_GEOMETRYGENERATOR_H
#define GAMEENGINE_GEOMETRYGENERATOR_H


#include <glm/glm.hpp>
#include <GL/gl3w.h>
#include <memory>

#include "Mesh.h"
#include "Vertex.h"



class MeshGenerator {
public:
    static std::shared_ptr<Mesh> generateSimpleRectangleMesh(float x, float y, float z);

    static std::shared_ptr<Mesh> generateTriangeMesh();

};


#endif //GAMEENGINE_GEOMETRYGENERATOR_H
