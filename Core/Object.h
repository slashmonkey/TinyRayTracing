
#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "Math.h"
#include "Material.h"

class Object {
public:
    Object() = default;
    virtual ~Object() = default;
    virtual bool intersect(const Vec3f& orig, const Vec3f& dir, float& t_near) const = 0;
    virtual void get_surface_data(const Vec3f& hit_pos, Vec3f& hit_normal, Vec2f& tex) const = 0;

    std::shared_ptr<Material> material;

};


#endif //RAYTRACER_OBJECT_H
