#pragma once

#include "Bounds3.h"
#include "Material.h"
#include "Object.h"
#include "Vector.h"

class Sphere : public Object {
   public:
    Vec3 center;
    float radius, radius2;
    Sphere(const Vec3& c, const float& r, Material* m)
        : center(c), radius(r), radius2(r * r), Object(m) {
        bounding_box = Bounds3(Vec3(center.x - radius, center.y - radius, center.z - radius),
                               Vec3(center.x + radius, center.y + radius, center.z + radius));
    }
    
    Sphere(const Vec3& c, const float& r, const MaterialType& t, const Vec3 &color)
        : Sphere(c, r, new Material(t, color)) {}
    
    Sphere(const Vec3& c, const float& r) : Sphere(c, r, LAMBERTIAN, Vec3(1)) {}
    
    ~Sphere() {
        delete material;
        material = NULL;
    }
    
    Intersection rayCast(const Ray& ray) {
        Intersection result;
        result.happened = false;
        Vec3 L = ray.origin - center;
        float a = dot(ray.direction, ray.direction);
        float b = 2 * dot(ray.direction, L);
        float c = dot(L, L) - radius2;
        float t0, t1;
        if (!solveQuadratic(a, b, c, t0, t1))
            return result;
        if (t0 < 0)
            t0 = t1;
        if (t0 < 0)
            return result;
        result.happened = true;
        result.coords = Vec3(ray.origin + ray.direction * t0);
        result.normal = normalize(Vec3(result.coords - center));
        result.material = this->material;
        result.obj = this;
        result.distance = t0;
        return result;
    }
    
    void getSurfaceProperties(const Vec3 &P, const Vec3 &I,
                              const Vec2 &uv, Vec3 &N, Vec2 &st) const {
        N = normalize(P - center);
    }

    Vec3 evalDiffuseColor(const Vec2 &st) const {
        return material->getColor();
    }
};
