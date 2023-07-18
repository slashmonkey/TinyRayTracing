
#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Math.h"

class Camera {
public:
    Camera() = default;
    ~Camera() = default;
    Camera(const Vec3f& origin, const Vec3f& target, float fov, int w, int h) : orig(origin), target(target), fov(fov), width(w), height(h){
        update();
    }

    Vec3f ray_dir(const float& x, const float& y) const;
    Vec3f origin() { return orig; }

    void update();

private:
    float fov;      //in degree
    float ratio;    // width/height
    float width;
    float height;
    Vec3f orig;
    Vec3f target;
    float tan_fov;
};


#endif //RAYTRACER_CAMERA_H
