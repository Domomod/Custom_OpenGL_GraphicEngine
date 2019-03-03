//
// Created by dominik on 27.02.19.
//

#include "AbstractRenderer.h"

void AbstractRenderer::init() {
    initialiseAttributeSender();
    initialiseUniformSender();
}

void AbstractRenderer::fullRender(Mesh &mesh, Window &window) {
    sendMeshDataToGPU(mesh);
    render(mesh);
}

OnChangeListener<std::pair<int, int>> &AbstractRenderer::getOnWindowResizeProjectionUpdater() {
    return onWindowResizeProjectionUpdater;
}

void AbstractRenderer::setFillType(GLenum fillType) {
    AbstractRenderer::fillType = fillType;
}

void AbstractRenderer::setGeometryType(GLenum geometryType) {
    AbstractRenderer::geometryType = geometryType;
}
