
#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H

#include "Math.h"
#include "Material.h"

class Object;

struct Intersection {
    Intersection(){
        hit = false;
        coords = Vec3f ();
        normal = Vec3f ();
        distance = std::numeric_limits<double>::max();
        obj = nullptr;
        material = nullptr;
    }

    bool hit;
    Vec3f coords;
    Vec3f normal;
    double distance;
    Object* obj;
    std::shared_ptr<Material> material;
};

#endif //RAYTRACER_INTERSECTION_H
