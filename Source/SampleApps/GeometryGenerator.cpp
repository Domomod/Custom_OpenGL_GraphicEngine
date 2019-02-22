//
// Created by dominik on 22.02.19.
//

#include "GeometryGenerator.h"

void
GeometryGenerator::generateMoreComplexRectangleMesh(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies) {
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(-50, -0.7, -50)
    );
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(0, -0.7, -50)
    );
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(50, -0.7, -50)
    );
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(-50, -0.7, 0)
    );
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(0, -0.7, 0)
    );
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(50, -0.7, 0)
    );
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(-50, -0.7, 50)
    );
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(0, -0.7, 50)
    );
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(50, -0.7, 50)
    );


    indicies.push_back(0);
    indicies.push_back(3);
    indicies.push_back(1);

    indicies.push_back(1);
    indicies.push_back(3);
    indicies.push_back(4);

    indicies.push_back(1);
    indicies.push_back(4);
    indicies.push_back(2);

    indicies.push_back(2);
    indicies.push_back(4);
    indicies.push_back(5);

    indicies.push_back(3);
    indicies.push_back(6);
    indicies.push_back(4);

    indicies.push_back(4);
    indicies.push_back(6);
    indicies.push_back(7);

    indicies.push_back(4);
    indicies.push_back(7);
    indicies.push_back(5);

    indicies.push_back(5);
    indicies.push_back(7);
    indicies.push_back(8);
}

void GeometryGenerator::generateSimpleRectangleMesh(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies) {
    verticies.emplace_back(
            glm::vec3(0, 1, 1), glm::vec3(-50, -0.7, -50)
    );
    verticies.emplace_back(
            glm::vec3(1, 0, 1), glm::vec3(50, -0.7, -50)
    );
    verticies.emplace_back(
            glm::vec3(1, 1, 0), glm::vec3(-50, -0.7, 50)
    );
    verticies.emplace_back(
            glm::vec3(1, 1, 1), glm::vec3(50, -0.7, 50)
    );

    indicies.push_back(0);
    indicies.push_back(2);
    indicies.push_back(1);

    indicies.push_back(1);
    indicies.push_back(2);
    indicies.push_back(3);
}

void GeometryGenerator::generateTriangeMesh(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies) {
    verticies.emplace_back(
            glm::vec3(1, 1, 1), glm::vec3(-1, -0.2, 0)
    );
    verticies.emplace_back(
            glm::vec3(1, 1, 1), glm::vec3(0, -0.2, -20)
    );
    verticies.emplace_back(
            glm::vec3(1, 1, 1), glm::vec3(1, -0.2, 0)
    );

    indicies.push_back(0);
    indicies.push_back(1);
    indicies.push_back(2);
}

void
GeometryGenerator::generateAsymetricalRectanuglarMesh(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies,
                                                      GLushort numVerticiesAlongXaxis, GLushort numVerticiesAlongZaxis,
                                                      GLfloat sizeX, GLfloat sizeZ, GLfloat centerX, GLfloat centerY,
                                                      GLfloat centerZ) {
    genereateRectangularGeometry(verticies, indicies, numVerticiesAlongXaxis,
                                 numVerticiesAlongZaxis, sizeX, sizeZ, centerX, centerY,
                                 centerZ);

    GLushort numEdgesAlongXaxis = numVerticiesAlongXaxis - short(1);
    GLushort numEdgesAlongZaxis = numVerticiesAlongZaxis - short(1);

    for (int j = 0; j < numEdgesAlongZaxis; j++) {
        for (int i = 0; i < numEdgesAlongXaxis; i++) {
            int column = i;
            int row = j * numVerticiesAlongXaxis;
            int nexColumn = column + 1;
            int nextRow = row + numVerticiesAlongXaxis;

            int topLeft = row + column;
            int topRight = row + nexColumn;
            int bottomLeft = nextRow + column;
            int bottomRight = nextRow + nexColumn;

            indicies.push_back(topLeft);
            indicies.push_back(bottomLeft);
            indicies.push_back(topRight);

            indicies.push_back(topRight);
            indicies.push_back(bottomLeft);
            indicies.push_back(bottomRight);
        }
    }
}

void
GeometryGenerator::generateSymetricalRectanuglarMesh(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies,
                                                     GLushort numVerticiesAlongXaxis, GLushort numVerticiesAlongZaxis,
                                                     GLfloat sizeX, GLfloat sizeZ, GLfloat centerX, GLfloat centerY,
                                                     GLfloat centerZ) {
    genereateRectangularGeometry(verticies, indicies, numVerticiesAlongXaxis,
                                 numVerticiesAlongZaxis, sizeX, sizeZ, centerX, centerY,
                                 centerZ);

    GLushort numEdgesAlongXaxis = numVerticiesAlongXaxis - short(1);
    GLushort numEdgesAlongZaxis = numVerticiesAlongZaxis - short(1);

    for (int j = 0; j < numEdgesAlongZaxis; j++) {
        for (int i = 0; i < numEdgesAlongXaxis; i++) {
            int column = i;
            int row = j * numVerticiesAlongXaxis;
            int nexColumn = column + 1;
            int nextRow = row + numVerticiesAlongXaxis;

            int topLeft = row + column;
            int topRight = row + nexColumn;
            int bottomLeft = nextRow + column;
            int bottomRight = nextRow + nexColumn;

            if ((j+i)%2) {
                indicies.push_back(topLeft);
                indicies.push_back(bottomLeft);
                indicies.push_back(topRight);

                indicies.push_back(topRight);
                indicies.push_back(bottomLeft);
                indicies.push_back(bottomRight);
            } else {
                indicies.push_back(topLeft);
                indicies.push_back(bottomLeft);
                indicies.push_back(bottomRight);

                indicies.push_back(topLeft);
                indicies.push_back(bottomRight);
                indicies.push_back(topRight);
            }
        }
    }
}

void GeometryGenerator::genereateRectangularGeometry(std::vector<Vertex> &verticies, std::vector<GLushort> &indicies,
                                                     GLushort numVerticiesAlongXaxis, GLushort numVerticiesAlongZaxis,
                                                     GLfloat sizeX, GLfloat sizeZ, GLfloat centerX, GLfloat centerY,
                                                     GLfloat centerZ) {
    float halfSizeX = sizeX/2;
    float halfSizeZ = sizeZ/2;
    GLushort numEdgesAlongXaxis = numVerticiesAlongXaxis - short(1);
    GLushort numEdgesAlongZaxis = numVerticiesAlongZaxis - short(1);
    float stepX = sizeX / numEdgesAlongXaxis;
    float stepZ = sizeZ/ numEdgesAlongZaxis;

    for(int j=0;j<numVerticiesAlongZaxis;j++) {
        for(int i=0;i<numVerticiesAlongXaxis;i++) {
            float x = -halfSizeX + centerX + i*stepX;
            float y = centerY;
            float z = -halfSizeZ + centerZ + j*stepZ;

            glm::vec3 color(1);
            glm::vec3 position(x, y, z);

            verticies.emplace_back(color,position);
        }
    }
}
