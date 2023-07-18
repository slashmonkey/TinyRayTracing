
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

    bool intersect(const Ray& ray) const;

    Vec3f diagonal() const { return max - min; }
    int max_extent() const{
        Vec3f d = diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if(d.y > d.z){
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
