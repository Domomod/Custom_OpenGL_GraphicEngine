//
// Created by dominik on 23.02.19.
//

#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> &verticies, const std::vector<GLushort> &indicies) : verticies(verticies),
                                                                                          indicies(indicies) {}

const std::vector<Vertex> &Mesh::getVerticies() const {
    return verticies;
}

const std::vector<GLushort> &Mesh::getIndicies() const {
    return indicies;
}
