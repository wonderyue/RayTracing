#pragma once

#include "Vector.h"

enum MaterialType { LAMBERTIAN, TRANSPARENT, METAL, LIGHT };

class Material {
public:
    float ior = 1.5f;
    float specularExp = 16.0f;
    float kr = 0.9f;
    Vec3 kd = Vec3(0.6f);

    inline Material(MaterialType t = LAMBERTIAN, Vec3 c = Vec3(1, 1, 1));
    inline MaterialType getType();
    inline Vec3 getColor();
private:
     MaterialType type;
     Vec3 color;
};

Material::Material(MaterialType t, Vec3 c) {
    type  = t;
    color = c;
}

MaterialType Material::getType() {
    return type;
}
Vec3 Material::getColor() {
    return color;
}
