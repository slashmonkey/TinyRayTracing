
#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "Object.h"
#include "Intersection.h"

class Sphere : public Object {
public:
    Sphere(float r, const Vec3f& c) : radius(r), center(c){
        radius2 = radius * radius;
    }

    bool intersect(const Vec3f& orig, const Vec3f& dir, float& t_near, uint32_t& index , Vec2f& bary_uv) const override{
        // analytic solution
        Vec3f L = orig - center;
        float a = dir * dir;
        float b = 2 * dir * L;
        float c = L * L - radius2;
        float t0, t1;
        if (!solve_quadratic(a, b, c, t0, t1))
            return false;
        if (t0 < 0)
            t0 = t1;
        if (t0 < 0)
            return false;
        t_near = t0;

        return true;
    }

    void get_surface_data(const Vec3f& hit_pos, Vec3f& hit_normal, Vec2f& tex, const uint32_t& index , const Vec2f& bary_uv, Vec2f& st) const override {
        hit_normal = (hit_pos - center).normalize();
        tex.x = (1 + atan2(hit_normal.z, hit_normal.x) / PI) * 0.5f;
        tex.y = acosf(hit_normal.y) / PI;
    }

    Bound get_bound() const override{
        return Bound(Vec3f (center.x - radius, center.y - radius, center.z - radius),
                       Vec3f (center.x + radius, center.y + radius, center.z + radius));
    }

    Intersection get_intersection(Ray ray) override{
        Intersection intersection;

        Vec3f L = ray.origin() - center;
        float a = ray.direction() * ray.direction();
        float b = 2 * ray.direction() * L;
        float c = L * L - radius2;
        float t0, t1;
        if (!solve_quadratic(a, b, c, t0, t1)) return intersection;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return intersection;

        intersection.hit = true;
        intersection.coords = Vec3f(ray.origin() + ray.direction() * t0);
        intersection.normal = (Vec3f (intersection.coords - center)).normalize();
        intersection.material = this->material;
        intersection.obj = this;
        intersection.distance = t0;
        return intersection;
    }

    ~Sphere() = default;

private:
    Vec3f center;
    float radius;
    float radius2;
};


#endif //RAYTRACER_SPHERE_H
