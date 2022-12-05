
#ifndef RAYTRACER_BVH_H
#define RAYTRACER_BVH_H

#include <utility>

#include "Bound.h"
#include "Object.h"
#include "Intersection.h"

struct BVHNode{
    Bound bounds;
    BVHNode* left;
    BVHNode* right;
    Object* object;

    BVHNode(){
        bounds = Bound();
        left = right = nullptr;
        object = nullptr;
    }
};

class BVH {
public:
    BVH(std::vector<Object*> objs):objects(std::move(objs)){
        if (objs.empty()) return;
        root = recursive_build(objects);
    }

    BVHNode* root;
    BVHNode* recursive_build(std::vector<Object*> objects);
    Intersection intersect(const Ray &ray) const;
    Intersection get_intersection(BVHNode* node, const Ray& ray)const;
    std::vector<Object*> objects;
};


#endif //RAYTRACER_BVH_H
