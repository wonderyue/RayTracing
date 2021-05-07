#pragma once

#include "Ray.h"
#include "Vector.h"

class Camera {
public:
    Camera() {}
    Camera(const Vec3& p, const Vec3& f, const Vec3& u, float fov, float aspect)
    :pos(p), front(f), up(u) {
        float half_height = tan(deg2rad(fov) * 0.5);
        float half_width = aspect * half_height;
        right = normalize(cross(front, up));
        up = normalize(up);
        front = normalize(front);
        bottomLeft = pos - half_width * right - half_height * up + front;
        horizontal = half_width * right * 2;
        vertical = half_height * up * 2;
    }

    Ray getRay(float x, float y) const {
        return Ray(pos, normalize(bottomLeft + x * horizontal + y * vertical - pos));
    }

private:
    Vec3 bottomLeft;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 pos;
    Vec3 up;
    Vec3 front;
    Vec3 right;
};
