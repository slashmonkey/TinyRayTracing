#include <iostream>
#include "Core/RayTracer.h"
#include "Core/Scene.h"
#include "Core/Sphere.h"
#include "Core/Light.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Dependencies/stb_image_write.h"

#define CHANNEL_NUM 3

int main() {
    Scene scene;
    std::unique_ptr<Object> sphere1 = std::make_unique<Sphere>(2.f, Vec3f(-2, 1, -8));
    std::unique_ptr<Object> sphere2 = std::make_unique<Sphere>(2.f, Vec3f(1, -2, -8));
    std::unique_ptr<Object> sphere3 = std::make_unique<Sphere>(1.f, Vec3f(2, 1, -6));
    std::unique_ptr<Object> sphere4 = std::make_unique<Sphere>(1.f, Vec3f(2.f, -2, -5));
    std::unique_ptr<Object> sphere5 = std::make_unique<Sphere>(1.f, Vec3f(0.2f, 0.3f, -5));

    std::shared_ptr<Material> lambertian1 = std::make_shared<Lambertian>(Red);
    std::shared_ptr<Material> lambertian2 = std::make_shared<Lambertian>(Green);
    std::shared_ptr<Material> lambertian3 = std::make_shared<Lambertian>(Blue);
    std::shared_ptr<Material> reflection = std::make_shared<ReflectionRefraction>();

    sphere1->set_material(lambertian1);
    sphere2->set_material(lambertian2);
    sphere3->set_material(lambertian3);
    sphere4->set_material(reflection);
    sphere5->set_material(reflection);

    scene.add(std::move(sphere1));
    scene.add(std::move(sphere2));
    scene.add(std::move(sphere3));
    scene.add(std::move(sphere4));
    scene.add(std::move(sphere5));

    std::unique_ptr<Light> light1 = std::make_unique<Light>(Vec3f(-6, 10, -1), 0.5f);
    std::unique_ptr<Light> light2 = std::make_unique<Light>(Vec3f(6, 15, 0), 0.5f);
    scene.add(std::move(light1));
    scene.add(std::move(light2));

    RayTracer tracer(CHANNEL_NUM);
    tracer.render(scene);

    stbi_write_png("./Tracer.png", tracer.get_width(), tracer.get_height(),
                   CHANNEL_NUM, tracer.get_image(), tracer.get_width() * CHANNEL_NUM);
    return 0;
}
