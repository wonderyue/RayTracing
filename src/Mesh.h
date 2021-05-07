#pragma once

#include "Vector.h"

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
};

struct Mesh {
    Mesh() {}
    Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices) {
        vertices     = _Vertices;
        indices      = _Indices;
        material = std::nullopt;
    }
    std::string MeshName;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::optional<Material> material;
};
