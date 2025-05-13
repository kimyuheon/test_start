// VulkanRenderer.cpp
#include "VulkanRenderer.h"
#include <stdexcept>
#include <fstream>
#include <cmath>

void VulkanRenderer::OnMouseDown(int x, int y) {
    if (drawState == DrawState::Idle) {
        rectStart = { x, y };
        rectEnd = rectStart;
        drawState = DrawState::DrawingRect;
    }
    else if (drawState == DrawState::Extruding) {
        drawState = DrawState::Idle;
    }
}

void VulkanRenderer::OnMouseMove(int x, int y) {
    if (drawState == DrawState::DrawingRect) {
        rectEnd = { x, y };
        BuildCubeGeometry();
        UpdateVertices();
    }
    else if (drawState == DrawState::Extruding) {
        extrudeDepth = std::max(0.1f, (float)(y - rectStart.y) * 0.01f);
        BuildCubeGeometry();
        UpdateVertices();
    }
}

void VulkanRenderer::OnMouseUp() {
    if (drawState == DrawState::DrawingRect) {
        drawState = DrawState::Extruding;
    }
    else if (drawState == DrawState::Extruding) {
        drawState = DrawState::Idle;
    }
}

void VulkanRenderer::BuildCubeGeometry() {
    vertices.clear();
    indices.clear();

    float x1 = (float)rectStart.x * 0.01f;
    float y1 = (float)rectStart.y * 0.01f;
    float x2 = (float)rectEnd.x * 0.01f;
    float y2 = (float)rectEnd.y * 0.01f;
    float z = extrudeDepth;

    float l = std::min(x1, x2);
    float r = std::max(x1, x2);
    float b = std::min(y1, y2);
    float t = std::max(y1, y2);

    glm::vec3 v[] = {
        { l, b, 0 }, { r, b, 0 }, { r, t, 0 }, { l, t, 0 },
        { l, b, z }, { r, b, z }, { r, t, z }, { l, t, z }
    };

    for (int i = 0; i < 8; ++i) vertices.push_back({ v[i] });

    uint32_t inds[] = {
        0,1,2, 2,3,0, 4,5,6, 6,7,4,
        0,1,5, 5,4,0, 2,3,7, 7,6,2,
        1,2,6, 6,5,1, 3,0,4, 4,7,3
    };
    indices.assign(inds, inds + 36);
}

void VulkanRenderer::UpdateVertices() {
    void* data;
    vkMapMemory(device, vertexMemory, 0, VK_WHOLE_SIZE, 0, &data);
    memcpy(data, vertices.data(), vertices.size() * sizeof(Vertex));
    vkUnmapMemory(device, vertexMemory);

    vkMapMemory(device, indexMemory, 0, VK_WHOLE_SIZE, 0, &data);
    memcpy(data, indices.data(), indices.size() * sizeof(uint32_t));
    vkUnmapMemory(device, indexMemory);
}
