
#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "Math.h"
#include "Material.h"
#include "Bound.h"

class Intersection;

class Object {
public:
    Object() = default;
    virtual ~Object() = default;
    virtual bool intersect(const Vec3f& orig, const Vec3f& dir, float& t_near, uint32_t& index, Vec2f& bary_uv) const = 0;
    virtual void get_surface_data(const Vec3f& hit_pos, Vec3f& hit_normal, Vec2f& tex, const uint32_t& index, const Vec2f& bary_uv, Vec2f& st) const = 0;
    virtual Intersection get_intersection(Ray ray) = 0;
    virtual Bound get_bound() const = 0;
    void set_material(std::shared_ptr<Material> mat){ material = mat; }

    std::shared_ptr<Material> material;
};


#endif //RAYTRACER_OBJECT_H
