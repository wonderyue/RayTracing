#pragma once

#include "Material.h"
#include "Vector.h"
class Object;
class Sphere;

struct Intersection {
    Intersection() {
        happened = false;
        coords   = Vec3();
        normal   = Vec3();
        distance = std::numeric_limits<double>::max();
        obj      = NULL;
        material = NULL;
    }
    bool happened;
    Vec3 coords;
    Vec3 normal;
    Vec2 uv;
    double distance;
    Object* obj;
    Material* material;
};
