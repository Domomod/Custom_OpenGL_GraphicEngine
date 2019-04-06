//
// Created by dominik on 22.02.19.
//

#include "MeshGenerator.h"

std::shared_ptr<Mesh> MeshGenerator::generateSimpleRectangleMesh(float x, float y, float z) {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    mesh->positions.emplace_back(glm::vec4(-x, y, -z, 1));
    mesh->positions.emplace_back(glm::vec4( x, y, -z, 1));
    mesh->positions.emplace_back(glm::vec4(-x, y,  z, 1));
    mesh->positions.emplace_back(glm::vec4( x, y,  z, 1));

    mesh->colors.emplace_back(glm::vec4(1,0,0,1));
    mesh->colors.emplace_back(glm::vec4(1,1,0,1));
    mesh->colors.emplace_back(glm::vec4(0,1,1,1));
    mesh->colors.emplace_back(glm::vec4(1,0,1,1));

    mesh->uv.emplace_back(glm::vec2(0, 0));
    mesh->uv.emplace_back(glm::vec2(1, 0));
    mesh->uv.emplace_back(glm::vec2(1, 1));
    mesh->uv.emplace_back(glm::vec2(0, 1));

    mesh->normals.emplace_back(glm::vec4(0, 1, 0, 0));
    mesh->normals.emplace_back(glm::vec4(0, 1, 0, 0));
    mesh->normals.emplace_back(glm::vec4(0, 1, 0, 0));
    mesh->normals.emplace_back(glm::vec4(0, 1, 0, 0));

    mesh->indicies.push_back(0);
    mesh->indicies.push_back(2);
    mesh->indicies.push_back(1);

    mesh->indicies.push_back(1);
    mesh->indicies.push_back(2);
    mesh->indicies.push_back(3);

    return mesh;
}

std::shared_ptr<Mesh> MeshGenerator::generateTriangeMesh() {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    mesh->positions.emplace_back(glm::vec4(-1, -1, 0, 1));
    mesh->positions.emplace_back(glm::vec4( 0,  1, 0, 1));
    mesh->positions.emplace_back(glm::vec4( 1, -1, 0, 1));

    mesh->colors.emplace_back(glm::vec4(1,0,0,1));
    mesh->colors.emplace_back(glm::vec4(0,1,0,1));
    mesh->colors.emplace_back(glm::vec4(0,0,1,1));

    mesh->uv.emplace_back(glm::vec2(0, 0));
    mesh->uv.emplace_back(glm::vec2(1, 0.5));
    mesh->uv.emplace_back(glm::vec2(0, 1));

    mesh->normals.emplace_back(glm::vec4(0, 0, 1, 0));
    mesh->normals.emplace_back(glm::vec4(0, 0, 1, 0));
    mesh->normals.emplace_back(glm::vec4(0, 0, 1, 0));


    mesh->indicies.push_back(0);
    mesh->indicies.push_back(2);
    mesh->indicies.push_back(1);

    return mesh;
}