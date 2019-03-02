//
// Created by dominik on 27.02.19.
//

#include "ForwardTesselationRenderer.h"

ForwardTesselationRenderer::ForwardTesselationRenderer() {}

ForwardTesselationRenderer::~ForwardTesselationRenderer() {
    shader->deleteProgram();
}