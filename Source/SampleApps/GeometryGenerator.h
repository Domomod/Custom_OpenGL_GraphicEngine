//
// Created by dominik on 22.02.19.
//

#ifndef GAMEENGINE_GEOMETRYGENERATOR_H
#define GAMEENGINE_GEOMETRYGENERATOR_H


#include <glm/glm.hpp>
#include <GL/gl3w.h>
#include "../View/Window.h"
#include "../View/Shader.h"
#include "Application.h"

class GeometryGenerator {
public:
    static void generateMoreComplexRectangleMesh(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies);

    static void generateSimpleRectangleMesh(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies);

    static void generateTriangeMesh(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies);

    static void generateAsymetricalRectanuglarMesh(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies,
                                            GLushort numVerticiesAlongXaxis = 2, GLushort numVerticiesAlongZaxis = 2,
                                            GLfloat sizeX = 10.0, GLfloat sizeZ = 10.0, GLfloat centerX = 0.0f,
                                            GLfloat centerY = 0.0f, GLfloat centerZ = 0.0f);

    static void generateSymetricalRectanuglarMesh(std::vector<Vertex>& verticies, std::vector<GLushort>& indicies,
                                                  GLushort numVerticiesAlongXaxis = 2, GLushort numVerticiesAlongZaxis = 2,
                                                  GLfloat sizeX = 100.0, GLfloat sizeZ = 100.0, GLfloat centerX = 0.0f,
                                                  GLfloat centerY = 0.0f, GLfloat centerZ = 0.0f);

private:
    static void genereateRectangularGeometry(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies,
                                             GLushort numVerticiesAlongXaxis, GLushort numVerticiesAlongZaxis, GLfloat sizeX,
                                             GLfloat sizeZ, GLfloat centerX, GLfloat centerY, GLfloat centerZ);

};


#endif //GAMEENGINE_GEOMETRYGENERATOR_H
