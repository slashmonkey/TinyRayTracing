
#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include <memory>
#include "Math.h"
#include "Color.h"
#include "Object.h"
#include "Light.h"
#include "BVH.h"

class Scene {
public:
    int max_depth = 10;
    Color background_color = Color(0.235294 * 255, 0.67451 * 255, 0.843137 * 255);
    void add(Object* object){ objects.push_back(std::move(object)); }
    void add(std::unique_ptr<Light> light){ lights.push_back(std::move(light)); }

    void buildBVH() {
        if (objects.empty()) return;
        bvh = std::make_unique<BVH>(objects);
    }

    Intersection intersect(const Ray& ray) const{
        return bvh->intersect(ray);
    }

    const std::vector<Object*>& get_objects() const { return objects; }
    const std::vector<std::unique_ptr<Light>>& get_lights() const { return lights; }

private:
    std::unique_ptr<BVH> bvh;
    std::vector<Object*> objects;
    std::vector<std::unique_ptr<Light>> lights;
};


#endif //RAYTRACER_SCENE_H
