
#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include "Math.h"

class Light {
public:
    Light(const Vec3f& pos, const float & i) : position(pos), intensity(i){}
    ~Light() = default;

    Vec3f position;
    float intensity;

};


#endif //RAYTRACER_LIGHT_H
