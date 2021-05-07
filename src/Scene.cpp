#include "Scene.h"

Scene::~Scene() {
    delete bvh;
    for (Object* object : objects) {
        delete object;
    }
    objects.clear();
}

Intersection Scene::traverse(const Ray& ray) const{
    Intersection intersection;
    for (Object* object : objects) {
        Intersection tmp = object->rayCast(ray);
        if (tmp.distance < intersection.distance) {
            intersection = tmp;
        }
    }
    return intersection;
}

void Scene::buildBVH() {
    if (bvhEnable)
        this->bvh = new BVH(objects);
}

Intersection Scene::rayCast(const Ray &ray) const {
    if (bvhEnable)
        return this->bvh->rayCast(ray);
    return traverse(ray);
}

Vec3 Scene::rayCastColor(const Ray &ray, int depth) const {
    if (depth > this->maxDepth) {
        return Vec3(0.0, 0.0, 0.0);
    }
    Intersection intersection = rayCast(ray);
    Vec3 color = this->backgroundColor;
    if (intersection.happened) {
        Vec2 st;
        intersection.obj->getSurfaceProperties(intersection.coords, ray.direction, intersection.uv, intersection.normal, st);
        switch (intersection.material->getType()) {
            case TRANSPARENT: {
                Vec3 reflectDir = normalize(reflect(ray.direction, intersection.normal));
                Vec3 refractDir = normalize(refract(ray.direction, intersection.normal, intersection.material->ior));
                Vec3 reflectPos = (dot(reflectDir, intersection.normal) < 0)
                                                 ? intersection.coords - intersection.normal * EPSILON
                                                 : intersection.coords + intersection.normal * EPSILON;
                Vec3 refractPos = (dot(refractDir, intersection.normal) < 0)
                                                 ? intersection.coords - intersection.normal * EPSILON
                                                 : intersection.coords + intersection.normal * EPSILON;
                Vec3 reflectColor =
                    rayCastColor(Ray(reflectPos, reflectDir), depth + 1);
                Vec3 refractColor =
                    rayCastColor(Ray(refractPos, refractDir), depth + 1);
                float kr;
                fresnel(ray.direction, intersection.normal, intersection.material->ior, kr);
                color = reflectColor * kr + refractColor * (1 - kr);
                break;
            }
            case METAL: {
                Vec3 reflectDir = reflect(ray.direction, intersection.normal);
                Vec3 reflectPos = (dot(reflectDir, intersection.normal) < 0)
                                                 ? intersection.coords - intersection.normal * EPSILON
                                                 : intersection.coords + intersection.normal * EPSILON;
                color = intersection.obj->evalDiffuseColor(st) * rayCastColor(Ray(reflectPos, reflectDir), depth + 1) * intersection.material->kr;
                break;
            }
            case LAMBERTIAN: {
                Vec3 diffDir = intersection.coords + intersection.normal + Vec3(rand_n1_1(), rand_n1_1(), rand_n1_1());
                Vec3 diffPos = (dot(diffDir, intersection.normal) < 0)
                                 ? intersection.coords - intersection.normal * EPSILON
                                 : intersection.coords + intersection.normal * EPSILON;
                color = intersection.obj->evalDiffuseColor(st) * rayCastColor(Ray(diffPos, diffDir), depth + 1) * intersection.material->kd;
                break;
            }
            case LIGHT: {
                color = intersection.obj->evalDiffuseColor(st) * intersection.material->kd;
                break;
            }
        }
    }
    return color;
}
