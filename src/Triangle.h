#pragma once

#include <array>
#include <cassert>

#include "BVH.h"
#include "Intersection.h"
#include "Material.h"
#include "OBJ_Loader.h"
#include "Object.h"
#include "Triangle.h"

// Möller Trumbore intersection algorithm
bool rayTriangleIntersect(const Vec3& v0, const Vec3& e1, const Vec3& e2, const Vec3& orig,
                          const Vec3& dir, float& tnear, float& u, float& v) {
    Vec3 s0 = orig - v0;
    Vec3 s1 = cross(dir, e2);
    Vec3 s2 = cross(s0, e1);
    // res: (tnear, u, v)
    Vec3 res = (1 / dot(s1, e1)) * Vec3(dot(s2, e2), dot(s1, s0), dot(s2, dir));
    tnear = res.x;
    u = res.y;
    v = res.z;
    return res.x >= .0f && res.y >= .0f && res.z >= .0f && res.y + res.z <= 1.0f;
}

class Triangle : public Object {
   public:
    Vec3 v0, v1, v2;
    Vec3 e1, e2;
    Vec3 t0, t1, t2;
    Vec3 normal;
    Vec2 st0, st1, st2;

    Triangle(const Vertex& _v0, const Vertex& _v1, const Vertex& _v2, Material* m)
        : v0(_v0.position), v1(_v1.position), v2(_v2.position),
          st0(_v0.texCoord), st1(_v1.texCoord), st2(_v2.texCoord), Object(m) {
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = normalize(cross(e1, e2));
        bounding_box = merge(Bounds3(v0, v1), v2);
    }

    Intersection rayCast(const Ray& ray) override {
        Intersection inter;
        if (dot(ray.direction, normal) > 0)
            return inter;
        float u, v, t = 0;
        inter.happened = rayTriangleIntersect(v0, e1, e2, ray.origin, ray.direction, t, u, v);
        if (inter.happened) {
            inter.coords = ray.origin + t * ray.direction;
            inter.normal = normal;
            inter.uv = Vec2(u, v);
            inter.obj = this;
            inter.material = material;
            inter.distance = t;
        }
        return inter;
    }

    void getSurfaceProperties(const Vec3& P, const Vec3& I, const Vec2& uv,
                              Vec3& N, Vec2& st) const override {
        st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;
    };

    Vec3 evalDiffuseColor(const Vec2& st) const override {
        return material->getColor();
//        float scale = 10;
//        bool v = (fmodf(st.x * scale, 1) > 0.5) ^ (fmodf(st.y * scale, 1) > 0.5);
//        return v ? Vec3(1, 1, 0) : Vec3(1);
    }
};

class MeshTriangle : public Object {
   public:
    MeshTriangle(const std::string& filename, Material* m, bool bvhEnable = true) : Object(m) {
        objl::Loader loader;
        loader.LoadFile(filename);

        assert(loader.LoadedMeshes.size() == 1);
        mesh = loader.LoadedMeshes[0];

        Vec3 min_vert =
            Vec3{std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(),
                 std::numeric_limits<float>::infinity()};
        Vec3 max_vert =
            Vec3{-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
                 -std::numeric_limits<float>::infinity()};
        
        material->specularExp = 8;
        for (int i = 0; i < mesh.vertices.size(); i += 3) {
            for (int j = 0; j < 3; j++) {
                auto vert = Vec3(mesh.vertices[i + j].position.x, mesh.vertices[i + j].position.y,
                                mesh.vertices[i + j].position.z);
                min_vert = Vec3(std::min(min_vert.x, mesh.vertices[i + j].position.x), std::min(min_vert.y, mesh.vertices[i + j].position.y),
                                std::min(min_vert.z, mesh.vertices[i + j].position.z));
                max_vert = Vec3(std::max(max_vert.x, mesh.vertices[i + j].position.x), std::max(max_vert.y, mesh.vertices[i + j].position.y),
                                std::max(max_vert.z, mesh.vertices[i + j].position.z));
            }
            triangles.push_back(new Triangle(mesh.vertices[i], mesh.vertices[i+1], mesh.vertices[i+2], material));
        }
        bounding_box = Bounds3(min_vert, max_vert);
        if (bvhEnable)
            bvh = new BVH(triangles);
    }
    
    ~MeshTriangle() {
        for (Object* t : triangles) {
            delete t;
        }
        triangles.clear();
    }

    void getSurfaceProperties(const Vec3& P, const Vec3& I, const Vec2& uv,
                              Vec3& N, Vec2& st) const {
    }

    Vec3 evalDiffuseColor(const Vec2& st) const {
        return Vec3(0.5, 0.5, 0.5);
    }

    Intersection rayCast(const Ray& ray) {
        Intersection intersection;
        if (bvh != NULL) {
            intersection = bvh->rayCast(ray);
        } else {
            for (Object* object : triangles) {
                Intersection tmp = object->rayCast(ray);
                if (tmp.distance < intersection.distance) {
                    intersection = tmp;
                }
            }
        }
        return intersection;
    }
private:
    std::unique_ptr<Vec3[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;
    std::unique_ptr<Vec2[]> stCoordinates;
    std::vector<Object*> triangles;
    BVH* bvh = NULL;
    Mesh mesh;
};
