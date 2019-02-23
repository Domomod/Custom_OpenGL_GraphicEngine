//
// Created by dominik on 23.02.19.
//

#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> &verticies, const std::vector<GLushort> &indicies) : verticies(verticies),
                                                                                          indicies(indicies) {}
