
#ifndef RAYTRACER_BOUND_H
#define RAYTRACER_BOUND_H

#include "Math.h"
#include "Ray.h"

class Bound {
public:
    Vec3f min;
    Vec3f max;
    Bound() = default;
    Bound(const Vec3f& p): min(p), max(p) {}
    Bound(const Vec3f& p_min, const Vec3f& p_max) {
        min = Vec3f(fmin(p_min.x, p_max.x), fmin(p_min.y, p_max.y), fmin(p_min.z, p_max.z));
        max = Vec3f(fmax(p_min.x, p_max.x), fmax(p_min.y, p_max.y), fmax(p_min.z, p_max.z));
    }

    Vec3f get_centroid() const { return 0.5 * min + 0.5 * max; }

    bool intersect(const Ray& ray) const{
        float t_enter_x = -infinity, t_exist_x = infinity;
        float t_enter_y = -infinity, t_exist_y = infinity;
        float t_enter_z = -infinity, t_exist_z = infinity;

        float inv_dir = 1.0f / ray.direction().x;
        float axis_min = min.x, axis_max = max.x;
        if (inv_dir < 0) std::swap(axis_min, axis_max);
        t_enter_x = (axis_min - ray.origin().x) * inv_dir;
        t_exist_x = (axis_max - ray.origin().x) * inv_dir;

        inv_dir = 1.0f / ray.direction().y;
        axis_min = min.y, axis_max = max.y;
        if (inv_dir < 0) std::swap(axis_min, axis_max);
        t_enter_y = (axis_min - ray.origin().y) * inv_dir;
        t_exist_y = (axis_max - ray.origin().y) * inv_dir;

        inv_dir = 1.0f / ray.direction().z;
        axis_min = min.z, axis_max = max.z;
        if (inv_dir < 0) std::swap(axis_min, axis_max);
        t_enter_z = (axis_min - ray.origin().z) * inv_dir;
        t_exist_z = (axis_max - ray.origin().z) * inv_dir;

        //max t_enter
        float t_enter = std::max(std::max(t_enter_x, t_enter_y), t_enter_z);
        //min t_exist
        float t_exist = std::min(std::min(t_exist_x, t_exist_y), t_exist_z);

        return (t_enter < t_exist) && (t_exist >= 0);
    }

    Vec3f diagonal() const { return max - min; }
    int max_extent() const{
        Vec3f d = diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if(d.y - d.z){
            return 1;
        }else{
            return 2;
        }
    }
};

inline Bound union_bounds(const Bound& bound1, const Bound& bound2){
    Bound bound;
    bound.min = Vec3f::Min(bound1.min, bound2.min);
    bound.max = Vec3f::Max(bound1.max, bound2.max);
    return bound;
}

inline Bound union_bounds(const Bound& bound, const Vec3f& vertex)
{
    Bound ret;
    ret.min = Vec3f::Min(bound.min, vertex);
    ret.max = Vec3f::Max(bound.max, vertex);
    return ret;
}


#endif //RAYTRACER_BOUND_H
