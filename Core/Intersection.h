
#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H

#include "Math.h"
#include "Material.h"

class Object;

struct Intersection {
    bool hit = false;
    double distance = std::numeric_limits<double>::max();
    Vec3f coords{};
    Vec3f normal{};
    Object* obj{};
    std::shared_ptr<Material> material{};
};

#endif //RAYTRACER_INTERSECTION_H
