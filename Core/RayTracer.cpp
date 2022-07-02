
#include "RayTracer.h"
#include "Color.h"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

void RayTracer::draw_pixel(unsigned int x, unsigned int y, const Color& color) {
    if (x < 0 || x >= window_width || y < 0 || y > window_width) return;
    unsigned int ind = (window_width * y + x) * channel;
    image[ind + 0] = static_cast<unsigned char>(color.R());
    image[ind + 1] = static_cast<unsigned char>(color.G());
    image[ind + 2] = static_cast<unsigned char>(color.B());
}

void RayTracer::render(const Scene& scene) {
    Vec3f origin = Vec3f(0.f, 0.f, 0.f);
    Vec3f target = Vec3f(0.f, 0.f, -1.f);
    float fov = 90;
    camera = std::make_unique<Camera>(origin, target, fov, window_width, window_height);

    for (int i = 0; i < window_height; ++i) {
        for (int j = 0; j < window_width; ++j) {
            Color color;
            for (int k = 0; k < samples_per_pixel; ++k) {
                Ray ray(camera->origin(), camera->ray_dir(j + random_in_range(0.f, 1.f), i + random_in_range(0.f, 1.f)));
                color = color + cast_ray(ray.origin(), ray.direction(), scene, 0);
            }
            float scale = 1.f / samples_per_pixel;
            color = color * scale;
            color.clamp();
            draw_pixel(j, i, color);
        }
    }
}

Color RayTracer::cast_ray(const Vec3f& orig, const Vec3f& dir, const Scene& scene, int depth) {
    if (depth > scene.max_depth) {
        return Black;
    }

    Color hit_color = scene.background_color;
    HitRecord hitRecord;
    if (trace(orig, dir, scene, hitRecord)){
        float scale = 4;
        float pattern = (fmodf(hitRecord.tex_coord.x * scale, 1) > 0.5) ^ (fmodf(hitRecord.tex_coord.y * scale, 1) > 0.5);
        hit_color = Blue.lerp(White * 0.8f, pattern) * std::max(0.f, hitRecord.normal*(-1 * dir));
    }
    return hit_color;
}

bool RayTracer::trace(const Vec3f& orig, const Vec3f& dir, const Scene& scene, HitRecord& hit_record) {
    float t_near = infinity;
    for (const auto& obj: scene.get_objects()) {
        if (obj->intersect(orig, dir, t_near)){
            Vec3f pos = orig + dir * t_near;
            Vec3f normal;
            Vec2f tex;
            obj->get_surface_data(pos, normal, tex);
            hit_record.pos = pos;
            hit_record.normal = normal;
            hit_record.tex_coord = tex;
            hit_record.t_near = t_near;
            hit_record.material = obj->material;
            return true;
        }
    }
    return false;
}
