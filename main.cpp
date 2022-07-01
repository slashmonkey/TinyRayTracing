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
    scene.add(std::move(sphere));

    RayTracer tracer(CHANNEL_NUM);
    tracer.render(scene);

    stbi_flip_vertically_on_write(1);
    stbi_write_png("./Tracer.png", tracer.get_width(), tracer.get_height(),
                   CHANNEL_NUM, tracer.get_image(), tracer.get_width() * CHANNEL_NUM);
    return 0;
}
