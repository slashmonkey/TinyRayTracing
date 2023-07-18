
#ifndef RAYTRACER_BVH_H
#define RAYTRACER_BVH_H

#include <utility>

#include "Bound.h"
#include "Object.h"
#include "Intersection.h"

struct BVHNode{
    Bound bounds {};
    BVHNode* left = nullptr;
    BVHNode* right = nullptr;
    Object* object = nullptr;
};

class BVH {
public:
    explicit BVH(std::vector<Object*> objects): objectsInScene(std::move(objects)){
        root = recursive_build(objectsInScene);
    }

    BVHNode* root;
    BVHNode* recursive_build(std::vector<Object*> objects);
    Intersection intersect(const Ray &ray) const;
    Intersection get_intersection(BVHNode* node, const Ray& ray)const;

private:
    std::vector<Object*> objectsInScene;
};


#endif //RAYTRACER_BVH_H
