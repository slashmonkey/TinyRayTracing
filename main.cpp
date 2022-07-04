#include <iostream>
#include "Core/RayTracer.h"
#include "Core/Scene.h"
#include "Core/Sphere.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Dependencies/stb_image_write.h"

#define CHANNEL_NUM 3

int main() {
    Scene scene;
    std::unique_ptr<Object> sphere = std::make_unique<Sphere>(4.f, Vec3f(-1, 0, -12));
    std::unique_ptr<Object> sphere_ground = std::make_unique<Sphere>(10.f, Vec3f(0, -10 , -5));
    scene.add(std::move(sphere));
    scene.add(std::move(sphere_ground));

    RayTracer tracer(CHANNEL_NUM);
    tracer.render(scene);

    stbi_write_png("./Tracer.png", tracer.get_width(), tracer.get_height(),
                   CHANNEL_NUM, tracer.get_image(), tracer.get_width() * CHANNEL_NUM);
    return 0;
}
