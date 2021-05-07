#pragma once

#include <vector>

#include "BVH.h"
#include "Object.h"
#include "Ray.h"
#include "Vector.h"
#include "global.h"
#include "Camera.h"

class Scene {
   public:
    int width = 1280;
    int height = 960;
    Vec3 backgroundColor = Vec3(1.0);
    int maxDepth = 10;
    bool bvhEnable;
    Camera camera;

    Scene() {}
    Scene(int w, int h, const Camera& cam, bool b) : width(w), height(h), camera(cam), bvhEnable(b) {}
    ~Scene();

    void Add(Object* object) {
        objects.push_back(object);
    }

    const std::vector<Object *> &get_objects() const {
        return objects;
    }
    
    void buildBVH();
    Vec3 rayCastColor(const Ray &ray, int depth) const;
    
private:
    std::vector<Object*> objects;
    BVH *bvh;
    
    Intersection rayCast(const Ray &ray) const;
    Intersection traverse(const Ray& ray) const;

    Vec3 reflect(const Vec3 &I, const Vec3 &N) const {
        return I - 2 * dot(I, N) * N;
    }

    Vec3 refract(const Vec3 &I, const Vec3 &N, const float &ior) const {
        float cosi = clamp(-1, 1, dot(I, N));
        float etai = 1, etat = ior;
        Vec3 n = N;
        if (cosi < 0) {
            cosi = -cosi;
        } else {
            std::swap(etai, etat);
            n = -N;
        }
        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);
        return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
    }

    void fresnel(const Vec3 &I, const Vec3 &N, const float &ior, float &kr) const {
        float cosi = clamp(-1, 1, dot(I, N));
        float etai = 1, etat = ior;
        if (cosi > 0) {
            std::swap(etai, etat);
        }
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        if (sint >= 1) {
            kr = 1;
        } else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
    }
};
