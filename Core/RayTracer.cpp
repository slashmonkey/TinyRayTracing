
#include "RayTracer.h"
#include "Color.h"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include "Math.h"

void RayTracer::draw_pixel(unsigned int x, unsigned int y, const Color &color) {
    if (x < 0 || x >= window_width || y < 0 || y > window_width) return;
    unsigned int ind = (window_width * y + x) * channel;
    image[ind + 0] = static_cast<unsigned char>(color.R());
    image[ind + 1] = static_cast<unsigned char>(color.G());
    image[ind + 2] = static_cast<unsigned char>(color.B());
}

void RayTracer::render(const Scene &scene) {
    Vec3f origin = Vec3f(0.f, 0.f, 0.f);
    Vec3f target = Vec3f(0.f, 0.f, -1.f);
    float fov = 90;
    camera = std::make_unique<Camera>(origin, target, fov, window_width, window_height);

    for (int i = 0; i < window_height; ++i) {
        for (int j = 0; j < window_width; ++j) {
            Color color;
            for (int k = 0; k < samples_per_pixel; ++k) {
                Ray ray(camera->origin(),
                        camera->ray_dir(j + random_in_range(0.f, 1.f), i + random_in_range(0.f, 1.f)));
                color = color + cast_ray(ray.origin(), ray.direction(), scene, 0);
            }
            float scale = 1.f / samples_per_pixel;
            color = color * scale;
            color.clamp();
            draw_pixel(j, i, color);
        }
    }
}

Color RayTracer::cast_ray(const Vec3f &orig, const Vec3f &dir, const Scene &scene, int depth) {
    if (depth > scene.max_depth) return scene.background_color;

    Color hit_color = scene.background_color;
    HitRecord hitRecord;
    if (trace(orig, dir, scene, hitRecord)) {
        switch (hitRecord.material->material_type) {
            case DIFFUSE_AND_GLOSSY: {
                std::shared_ptr<Lambertian> lambertian = std::dynamic_pointer_cast<Lambertian>(hitRecord.material);

                float diffuse_intensity = 0.f;
                float specular_intensity = 0.f;

                Vec3f hit_point = hitRecord.pos;
                Vec3f shadow_point_orig = dir.dot(hitRecord.normal) < 0.f ?
                                          hit_point + hitRecord.normal * EPSILON :
                                          hit_point - hitRecord.normal * EPSILON;

                for (auto &light: scene.get_lights()) {
                    Vec3f dir_hit_light = (light->position - hitRecord.pos);
                    float r2 = dir_hit_light.dot(dir_hit_light);
                    dir_hit_light.normalize();
                    float light_dot_normal = std::max(0.f, dir_hit_light.dot(hitRecord.normal));

                    Vec3f h = (dir_hit_light - dir).normalize();
                    specular_intensity += light->intensity *
                                          std::powf(std::max(0.f, h.dot(hitRecord.normal)), lambertian->specular_exp);

                    HitRecord shadow_ray_hitRecord;
                    if (trace(shadow_point_orig, dir_hit_light, scene, shadow_ray_hitRecord)) {
                        if ((shadow_point_orig - shadow_ray_hitRecord.pos).norm() < dir_hit_light.norm()) {
                            continue;
                        }
                    }

                    diffuse_intensity += light->intensity * std::max(0.f, light_dot_normal);
                }

                Color diffuse_color = lambertian->diffuse_color * diffuse_intensity * lambertian->Kd;
                Color specular_color = White * specular_intensity * lambertian->Ks;

                hit_color = diffuse_color + specular_color;
                break;
            }

            case REFLECTION: {
                std::shared_ptr<Reflection> reflection = std::dynamic_pointer_cast<Reflection>(hitRecord.material);

                float kr;
                fresnel(dir, hitRecord.normal, reflection->ior, kr);
                Vec3f hit_point = hitRecord.pos;
                Vec3f reflect_dir = reflect(dir, hitRecord.normal);
                Vec3f reflect_orig =
                        reflect_dir.dot(hitRecord.normal) < 0 ? hit_point + hitRecord.normal * EPSILON : hit_point -
                                                                                                         hitRecord.normal *
                                                                                                         EPSILON;

                hit_color = cast_ray(reflect_orig, reflect_dir, scene, depth + 1) * kr;
                break;
            }

            case REFLECTION_AND_REFRACTION: {
                std::shared_ptr<ReflectionRefraction> refraction = std::dynamic_pointer_cast<ReflectionRefraction>(
                        hitRecord.material);

                Vec3f hit_point = hitRecord.pos;
                Vec3f reflectionDirection = reflect(dir, hitRecord.normal).normalize();
                Vec3f refractionDirection = refract(dir, hitRecord.normal, refraction->ior).normalize();
                Vec3f reflectionRayOrig = (reflectionDirection.dot(hitRecord.normal)) < 0.f ?
                                          hit_point - hitRecord.normal * EPSILON :
                                          hit_point + hitRecord.normal * EPSILON;
                Vec3f refractionRayOrig = (refractionDirection.dot(hitRecord.normal) < 0) ?
                                          hit_point - hitRecord.normal * EPSILON :
                                          hit_point + hitRecord.normal * EPSILON;
                Color reflectionColor = cast_ray(reflectionRayOrig, reflectionDirection, scene, depth + 1);
                Color refractionColor = cast_ray(refractionRayOrig, refractionDirection, scene, depth + 1);
                float kr;
                fresnel(dir, hitRecord.normal, refraction->ior, kr);

                float specular_intensity = 0.f;
                for (auto &light: scene.get_lights()) {
                    Vec3f dir_hit_light = (light->position - hitRecord.pos);
                    float r2 = dir_hit_light.dot(dir_hit_light);
                    dir_hit_light.normalize();
                    float light_dot_normal = std::max(0.f, dir_hit_light.dot(hitRecord.normal));

                    Vec3f h = (dir_hit_light - dir).normalize();
                    specular_intensity += light->intensity *
                                          std::powf(std::max(0.f, h.dot(hitRecord.normal)), refraction->specular_exp);
                }
                Color specular_color = White * specular_intensity * refraction->Ks;

                hit_color = specular_color + reflectionColor * kr + refractionColor * (1 - kr);

                break;
            }
        }
    }
    return hit_color;
}

bool RayTracer::trace(const Vec3f &orig, const Vec3f &dir, const Scene &scene, HitRecord &hit_record) {
    float t_near = infinity;
    bool hit = false;
    Ray ray(orig, dir);
    Intersection intersection = scene.intersect(ray);
    hit = intersection.hit;
    if (hit) {
        hit_record.pos = intersection.coords;
        hit_record.normal = intersection.normal;
        hit_record.material = intersection.material;
        hit_record.object = intersection.obj;
    }

    //blow is used without bvh
    /*for (const auto& obj: scene.get_objects()) {
        Vec2f bary_uv;
        uint32_t index;
        if (obj->intersect(orig, dir, t_near, index, bary_uv)){
            Vec3f pos = orig + dir * t_near;
            Vec3f normal;
            Vec2f tex;
            Vec2f st;
            obj->get_surface_data(pos, normal, tex, index, bary_uv, st);
            hit_record.pos = pos;
            hit_record.normal = normal;
            hit_record.tex_coord = tex;
            hit_record.t_near = t_near;
            hit_record.bary_uv = bary_uv;
            hit_record.bary_st = st;
            hit_record.material = obj->material;
            hit = true;
        }
    }*/
    return hit;
}
