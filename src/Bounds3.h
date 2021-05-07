#pragma once

#include <array>
#include <limits>

#include "Ray.h"
#include "Vector.h"

class Bounds3 {
public:
    Vec3 pMin, pMax, centroid;
    
    Bounds3(const Vec3& p1, const Vec3& p2) {
        pMin = Vec3(fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z));
        pMax = Vec3(fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z));
        centroid = 0.5 * pMin + 0.5 * pMax;
    }
    
    Bounds3(const Vec3& p) : Bounds3(p, p) {}
    
    Bounds3() {
        float minNum = std::numeric_limits<float>::lowest();
        float maxNum = std::numeric_limits<float>::max();
        Bounds3(Vec3(minNum, minNum, minNum), Vec3(maxNum, maxNum, maxNum));
    }

    int longestAxis() const {
        Vec3 d = pMax - pMin;
        if (d.x > d.y && d.x > d.z)
            return 0;
        return d.y > d.z ? 1 : 2;
    }
    
    inline bool rayCast(const Ray& ray) const;
};

inline bool Bounds3::rayCast(const Ray& ray) const {
    float tEnter = std::numeric_limits<float>::lowest();
    float tExit = std::numeric_limits<float>::max();
    for (int i = 0; i < 3; i++) {
        float t0 = (pMin[i] - ray.origin[i]) * ray.direction_inv[i];
        float t1 = (pMax[i] - ray.origin[i]) * ray.direction_inv[i];
        if (t0 > t1)
            std::swap(t0, t1);
        tEnter = fmax(tEnter, t0);
        tExit = fmin(tExit, t1);
        if (tExit < .0f || tEnter > tExit)
            return false;
    }
    return true;
}

inline Bounds3 merge(const Bounds3& b1, const Bounds3& b2) {
    Bounds3 ret;
    ret.pMin = Vec3::min(b1.pMin, b2.pMin);
    ret.pMax = Vec3::max(b1.pMax, b2.pMax);
    return ret;
}

inline Bounds3 merge(const Bounds3& b, const Vec3& p) {
    Bounds3 ret;
    ret.pMin = Vec3::min(b.pMin, p);
    ret.pMax = Vec3::max(b.pMax, p);
    return ret;
}
