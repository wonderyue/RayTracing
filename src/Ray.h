#pragma once

#include "Vector.h"
struct Ray {
    Vec3 origin, direction, direction_inv;
    float t;

    Ray(const Vec3& ori, const Vec3& dir, const float _t = 0.0)
        : origin(ori), direction(dir), t(_t) {
        direction_inv = Vec3(1. / direction.x, 1. / direction.y, 1. / direction.z);
    }

    Vec3 get(float t) const {
        return origin + t * direction;
    }
};
