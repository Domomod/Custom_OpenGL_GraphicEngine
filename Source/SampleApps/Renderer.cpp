//
// Created by dominik on 27.02.19.
//

#include "Renderer.h"

Renderer::Renderer() {}

Renderer::~Renderer() {
    shader->deleteProgram();
}