#pragma once

#include <atomic>
#include <ctime>
#include <memory>
#include <vector>

#include "Bounds3.h"
#include "Intersection.h"
#include "Object.h"
#include "Ray.h"
#include "Vector.h"

inline int leafNodes, totalLeafNodes, totalPrimitives, interiorNodes;
class BVH {
public:
    class BVHNode {
    public:
        Bounds3 bounds;
        BVHNode* left;
        BVHNode* right;
        Object* object;

        BVHNode() {
            bounds = Bounds3();
            left   = NULL;
            right  = NULL;
            object = NULL;
        }
        
        ~BVHNode() {
            delete left;
            left = NULL;
            delete right;
            right = NULL;
        }
    };
    
    BVH(std::vector<Object*>& objects);
    ~BVH();
    Intersection rayCast(const Ray& ray) const;
private:
    //endIdx is exclusive by convention
    BVHNode* build(std::vector<Object*>& objects, std::vector<Object*>::size_type startIdx, std::vector<Object*>::size_type endIdx);
    Intersection rayCast(BVHNode* node, const Ray& ray) const;
    BVHNode* root;
};
