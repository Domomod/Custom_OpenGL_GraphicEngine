//
// Created by dominik on 22.02.19.
//

#include "MeshGenerator.h"

#include "Mesh.h"


std::shared_ptr<Mesh> MeshGenerator::generateSimpleRectangleMesh(float x, float y, float z) {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    mesh->positions.emplace_back(glm::vec3(-x, y, -z));
    mesh->positions.emplace_back(glm::vec3( x, y, -z));
    mesh->positions.emplace_back(glm::vec3(-x, y,  z));
    mesh->positions.emplace_back(glm::vec3( x, y,  z));

    mesh->colors.emplace_back(glm::vec4(1,0,0,1));
    mesh->colors.emplace_back(glm::vec4(1,1,0,1));
    mesh->colors.emplace_back(glm::vec4(0,1,1,1));
    mesh->colors.emplace_back(glm::vec4(1,0,1,1));

    mesh->uv.emplace_back(glm::vec2(0, 0));
    mesh->uv.emplace_back(glm::vec2(1, 0));
    mesh->uv.emplace_back(glm::vec2(1, 1));
    mesh->uv.emplace_back(glm::vec2(0, 1));

    mesh->normals.emplace_back(glm::vec3(0, 1, 0));
    mesh->normals.emplace_back(glm::vec3(0, 1, 0));
    mesh->normals.emplace_back(glm::vec3(0, 1, 0));
    mesh->normals.emplace_back(glm::vec3(0, 1, 0));

    mesh->indicies.push_back(0);
    mesh->indicies.push_back(2);
    mesh->indicies.push_back(1);

    mesh->indicies.push_back(1);
    mesh->indicies.push_back(2);
    mesh->indicies.push_back(3);

    mesh->hasBones   = false;
    mesh->hasColors  = true;
    mesh->hasNormals = true;
    mesh->hasUvs     = true;

    return mesh;
}


std::shared_ptr<Mesh> MeshGenerator::generateTriangeMesh() {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    mesh->positions.emplace_back(glm::vec3(-1, -1, 0));
    mesh->positions.emplace_back(glm::vec3( 0,  1, 0));
    mesh->positions.emplace_back(glm::vec3( 1, -1, 0));

    mesh->colors.emplace_back(glm::vec4(1,0,0,1));
    mesh->colors.emplace_back(glm::vec4(0,1,0,1));
    mesh->colors.emplace_back(glm::vec4(0,0,1,1));

    mesh->uv.emplace_back(glm::vec2(0, 0));
    mesh->uv.emplace_back(glm::vec2(1, 0.5));
    mesh->uv.emplace_back(glm::vec2(0, 1));

    mesh->normals.emplace_back(glm::vec3(0, 0, 1));
    mesh->normals.emplace_back(glm::vec3(0, 0, 1));
    mesh->normals.emplace_back(glm::vec3(0, 0, 1));


    mesh->indicies.push_back(0);
    mesh->indicies.push_back(2);
    mesh->indicies.push_back(1);

    mesh->hasBones   = false;
    mesh->hasColors  = true;
    mesh->hasNormals = true;
    mesh->hasUvs     = true;

    return mesh;
}


std::shared_ptr<Mesh> MeshGenerator::generateSkyBox() {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    mesh->positions.emplace_back(glm::vec3( 1.f, 1.f, 1.f));
    mesh->positions.emplace_back(glm::vec3( 1.f, 1.f,-1.f));
    mesh->positions.emplace_back(glm::vec3( 1.f,-1.f, 1.f));
    mesh->positions.emplace_back(glm::vec3( 1.f,-1.f,-1.f));
    mesh->positions.emplace_back(glm::vec3(-1.f, 1.f, 1.f));
    mesh->positions.emplace_back(glm::vec3(-1.f, 1.f,-1.f));
    mesh->positions.emplace_back(glm::vec3(-1.f,-1.f, 1.f));
    mesh->positions.emplace_back(glm::vec3(-1.f,-1.f,-1.f));

    mesh->indicies.push_back(5);
    mesh->indicies.push_back(7);
    mesh->indicies.push_back(3);
    mesh->indicies.push_back(3);
    mesh->indicies.push_back(1);
    mesh->indicies.push_back(5);

    mesh->indicies.push_back(6);
    mesh->indicies.push_back(7);
    mesh->indicies.push_back(5);
    mesh->indicies.push_back(5);
    mesh->indicies.push_back(4);
    mesh->indicies.push_back(6);

    mesh->indicies.push_back(3);
    mesh->indicies.push_back(2);
    mesh->indicies.push_back(0);
    mesh->indicies.push_back(0);
    mesh->indicies.push_back(1);
    mesh->indicies.push_back(3);

    mesh->indicies.push_back(6);
    mesh->indicies.push_back(4);
    mesh->indicies.push_back(0);
    mesh->indicies.push_back(0);
    mesh->indicies.push_back(2);
    mesh->indicies.push_back(6);

    mesh->indicies.push_back(5);
    mesh->indicies.push_back(1);
    mesh->indicies.push_back(0);
    mesh->indicies.push_back(0);
    mesh->indicies.push_back(4);
    mesh->indicies.push_back(5);

    mesh->indicies.push_back(7);
    mesh->indicies.push_back(6);
    mesh->indicies.push_back(3);
    mesh->indicies.push_back(3);
    mesh->indicies.push_back(6);
    mesh->indicies.push_back(2);

    mesh->hasBones   = false;
    mesh->hasColors  = false;
    mesh->hasNormals = false;
    mesh->hasUvs     = false;

    return mesh;
}