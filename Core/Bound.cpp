
#include "Bound.h"

bool Bound::intersect(const Ray &ray) const {
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
