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
    static std::shared_ptr<Mesh> generateMoreComplexRectangleMesh();

    static std::shared_ptr<Mesh> generateSimpleRectangleMesh(float x, float y, float z);

    static std::shared_ptr<Mesh> generateTriangeMesh();

    static std::shared_ptr<Mesh> generateAsymetricalRectanuglarMesh(
            GLushort numVerticiesAlongXaxis = 2,
            GLushort numVerticiesAlongZaxis = 2,
            GLfloat sizeX = 10.0,
            GLfloat sizeZ = 10.0,
            GLfloat centerX = 0.0f,
            GLfloat centerY = 0.0f,
            GLfloat centerZ = 0.0f);

    static std::shared_ptr<Mesh> generateSymetricalRectanuglarMesh(
            GLushort numVerticiesAlongXaxis = 2,
            GLushort numVerticiesAlongZaxis = 2,
            GLfloat sizeX = 100.0,
            GLfloat sizeZ = 100.0,
            GLfloat centerX = 0.0f,
            GLfloat centerY = 0.0f,
            GLfloat centerZ = 0.0f);


private:
    static std::vector<Vertex> genereateRectangularGeometry(
            GLushort numVerticiesAlongXaxis,
            GLushort numVerticiesAlongZaxis,
            GLfloat sizeX,
            GLfloat sizeZ,
            GLfloat centerX,
            GLfloat centerY,
            GLfloat centerZ);

};


#endif //GAMEENGINE_GEOMETRYGENERATOR_H
