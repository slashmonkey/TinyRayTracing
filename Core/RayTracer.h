
#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H

#include <memory>
#include "Camera.h"
#include "Math.h"

class Color;
class Ray;
class Scene;
class Object;
class Material;

struct HitRecord{
    float t_near;
    Vec2f tex_coord;
    Vec3f pos;
    Vec3f normal;
    std::shared_ptr<Material> material;
};

class RayTracer {
public:
    RayTracer(unsigned int channel) : image(nullptr), channel(channel){
        image = new unsigned char[window_width * window_height * channel];
    };

    ~RayTracer() { delete image; };

    void render(const Scene& scene);
    Color cast_ray(const Vec3f& orig, const Vec3f& dir, const Scene& scene, int depth);
    bool trace(const Vec3f& orig, const Vec3f& dir, const Scene& scene, HitRecord& hit_record);

    unsigned char* get_image() { return image; }

    int get_width() const { return window_width; }
    int get_height() const { return window_height; }

private:
    const int window_width = 1280;
    const int window_height = 960;
    const unsigned int channel;
    unsigned char* image;
    std::unique_ptr<Camera> camera;
    const int samples_per_pixel = 16;

    void draw_pixel(unsigned int x, unsigned int y, const Color& color);
};


#endif //RAYTRACER_RAYTRACER_H
