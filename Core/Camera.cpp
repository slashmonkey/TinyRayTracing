
#include "Camera.h"

Vec3f Camera::ray_dir(const float& x, const float& y) {
    float ndc_x = (x + 0.5f) / width;
    float ndc_y = (y + 0.5f) / height;

    float screen_x = 2 * ndc_x - 1;
    float screen_y = 1 - 2 * ndc_y;

    float camera_x = screen_x * ratio * tan_fov;
    float camera_y = screen_y * tan_fov;

    return Vec3f(camera_x, camera_y, -1.f);
}

void Camera::update() {
    ratio = width / height;
    tan_fov = std::tan(TO_RADIANS(fov * 0.5f));
}
