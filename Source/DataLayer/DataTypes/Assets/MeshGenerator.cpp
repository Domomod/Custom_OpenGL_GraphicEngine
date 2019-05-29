//
// Created by dominik on 22.02.19.
//

#include "MeshGenerator.h"

#include "MeshFactory.h"

std::shared_ptr<Mesh> MeshGenerator::generateSimpleRectangleMesh(float x, float y, float z) {
    return MeshFactory()
                 .addPosition(glm::vec3(-x, y, -z))
                 .addPosition(glm::vec3( x, y, -z))
                 .addPosition(glm::vec3(-x, y,  z))
                 .addPosition(glm::vec3( x, y,  z))

                 .addColor(glm::vec4(1,0,0,1))
                 .addColor(glm::vec4(1,1,0,1))
                 .addColor(glm::vec4(0,1,1,1))
                 .addColor(glm::vec4(1,0,1,1))

                 .addUv(glm::vec2(0, 0))
                 .addUv(glm::vec2(1, 0))
                 .addUv(glm::vec2(1, 1))
                 .addUv(glm::vec2(0, 1))

                 .addNormal(glm::vec3(0, 1, 0))
                 .addNormal(glm::vec3(0, 1, 0))
                 .addNormal(glm::vec3(0, 1, 0))
                 .addNormal(glm::vec3(0, 1, 0))

                 .addFace(0, 2, 1)
                 .addFace(1, 2, 3)
                 .make("Rectangle");
}


std::shared_ptr<Mesh> MeshGenerator::generateTriangeMesh() {
    return MeshFactory()
            .addPosition(glm::vec3(-1, -1, 0))
            .addPosition(glm::vec3( 0,  1, 0))
            .addPosition(glm::vec3( 1, -1, 0))

            .addColor(glm::vec4(1,0,0,1))
            .addColor(glm::vec4(0,1,0,1))
            .addColor(glm::vec4(0,0,1,1))

            .addUv(glm::vec2(0, 0  ))
            .addUv(glm::vec2(1, 0.5))
            .addUv(glm::vec2(0, 1  ))

            .addNormal(glm::vec3(0, 1, 0))
            .addNormal(glm::vec3(0, 1, 0))
            .addNormal(glm::vec3(0, 1, 0))

            .addFace(0, 2, 1)
            .make("Triangle");
}


std::shared_ptr<Mesh> MeshGenerator::generateSkyBox() {
    return MeshFactory()
            .addPosition(glm::vec3( 1.f, 1.f, 1.f))
            .addPosition(glm::vec3( 1.f, 1.f,-1.f))
            .addPosition(glm::vec3( 1.f,-1.f, 1.f))
            .addPosition(glm::vec3( 1.f,-1.f,-1.f))
            .addPosition(glm::vec3(-1.f, 1.f, 1.f))
            .addPosition(glm::vec3(-1.f, 1.f,-1.f))
            .addPosition(glm::vec3(-1.f,-1.f, 1.f))
            .addPosition(glm::vec3(-1.f,-1.f,-1.f))
            .addFace(5,7,3)
            .addFace(3,1,5)
            .addFace(6,7,5)
            .addFace(5,4,6)
            .addFace(3,2,0)
            .addFace(0,1,3)
            .addFace(6,4,0)
            .addFace(0,2,6)
            .addFace(5,1,0)
            .addFace(0,4,5)
            .addFace(7,6,3)
            .addFace(3,6,2)
            .make("SkyBox");
}