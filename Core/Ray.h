
#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "Math.h"
class Ray {

public:
    Ray() : orig(), dir(0, 0, -1){};
    ~Ray() = default;
    Ray(const Vec3f& origin, const Vec3f& direction) : orig(origin), dir(direction){
        dir.normalize();
    }

    Vec3f origin() const { return orig; }
    Vec3f direction() const { return dir; }
    Vec3f at(const float& t) const { return orig + dir * t; }
private:
    Vec3f orig;
    Vec3f dir;
};


#endif //RAYTRACER_RAY_H
