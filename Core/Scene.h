
#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include "Math.h"
#include "Color.h"
#include "Object.h"

class Scene {
public:
    int max_depth = 10;
    Color background_color = Color(0.235294 * 255, 0.67451 * 255, 0.843137 * 255);
    void add(std::unique_ptr<Object> object){ objects.push_back(std::move(object)); }

    const std::vector<std::unique_ptr<Object>>& get_objects() const { return objects; }

private:
    std::vector<std::unique_ptr<Object>> objects;
};


#endif //RAYTRACER_SCENE_H
