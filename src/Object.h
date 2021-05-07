#pragma once

#include "Bounds3.h"
#include "Intersection.h"
#include "Ray.h"
#include "Vector.h"
#include "global.h"

class Object {
public:
    Object(Material* m = NULL) : material(m) {}
    virtual ~Object() {}
    virtual Intersection rayCast(const Ray& ray) = 0;
    virtual void getSurfaceProperties(const Vec3&, const Vec3&,
                                      const Vec2&, Vec3 &, Vec2&) const = 0;
    virtual Vec3 evalDiffuseColor(const Vec2&) const = 0;
    Bounds3 getBounds() { return bounding_box; };
    Material* material;
protected:
    Bounds3 bounding_box;
};
