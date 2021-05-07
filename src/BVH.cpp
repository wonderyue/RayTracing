#include <algorithm>
#include <cassert>

#include "BVH.h"

BVH::BVH(std::vector<Object*>& objects) {
    if (objects.empty())
        return;
    root = build(objects, 0, objects.size());
}

BVH::~BVH() {
    delete root;
    root = NULL;
}

BVH::BVHNode* BVH::build(std::vector<Object*>& objects, std::vector<Object*>::size_type startIdx, std::vector<Object*>::size_type endIdx) {
    BVHNode* node = new BVHNode();
    if (endIdx - startIdx == 1) {
        node->bounds = objects[startIdx]->getBounds();
        node->object = objects[startIdx];
        node->left   = NULL;
        node->right  = NULL;
        return node;
    } else if (endIdx - startIdx == 2) {
        node->left   = build(objects, startIdx, startIdx+1);
        node->right  = build(objects, startIdx+1, endIdx);
        node->bounds = merge(node->left->bounds, node->right->bounds);
        return node;
    } else {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i)
            centroidBounds = merge(centroidBounds, objects[i]->getBounds().centroid);
        int dim = centroidBounds.longestAxis();
        switch (dim) {
            case 0:
                std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                    return f1->getBounds().centroid.x < f2->getBounds().centroid.x;
                });
                break;
            case 1:
                std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                    return f1->getBounds().centroid.y < f2->getBounds().centroid.y;
                });
                break;
            case 2:
                std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                    return f1->getBounds().centroid.z < f2->getBounds().centroid.z;
                });
                break;
        }
        std::vector<Object*>::size_type midIdx = (endIdx - startIdx) / 2 + startIdx;
        node->left  = build(objects, startIdx, midIdx);
        node->right = build(objects, midIdx, endIdx);
        node->bounds = merge(node->left->bounds, node->right->bounds);
    }
    return node;
}

Intersection BVH::rayCast(const Ray& ray) const {
    Intersection res;
    if (root == NULL)
        return res;
    res = rayCast(root, ray);
    return res;
}

Intersection BVH::rayCast(BVHNode* node, const Ray& ray) const {
    Intersection res;
    if (node == NULL)
        return res;
    if (!node->bounds.rayCast(ray))
        return res;
    if (node->left == NULL && node->right == NULL)
        return node->object->rayCast(ray);
    res = rayCast(node->left, ray);
    if (!res.happened)
        return rayCast(node->right, ray);
    Intersection right = rayCast(node->right, ray);
    if (right.happened && right.distance < res.distance)
        return right;
    return res;
}
