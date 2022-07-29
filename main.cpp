#include <iostream>
#include "Core/RayTracer.h"
#include "Core/Scene.h"
#include "Core/Sphere.h"
#include "Core/Light.h"
#include "Core/TriangleMesh.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Dependencies/stb_image_write.h"

#define CHANNEL_NUM 3

int main() {
    Scene scene;
    std::unique_ptr<Object> sphere1 = std::make_unique<Sphere>(2.f, Vec3f(-2, -0.5f, -8));
    std::unique_ptr<Object> sphere2 = std::make_unique<Sphere>(2.f, Vec3f(2.5f, -1.0f, -12));
    std::unique_ptr<Object> sphere3 = std::make_unique<Sphere>(1.f, Vec3f(2.f, -2, -6));
    std::unique_ptr<Object> sphere4 = std::make_unique<Sphere>(0.5f, Vec3f(-0.5f, -2.5f, -5.5));
    std::unique_ptr<Object> sphere5 = std::make_unique<Sphere>(1.f, Vec3f(0.2f, 1.3f, -5));

    std::shared_ptr<Material> lambertian1 = std::make_shared<Lambertian>(Red);
    std::shared_ptr<Material> lambertian2 = std::make_shared<Lambertian>(Green);
    std::shared_ptr<Material> lambertian3 = std::make_shared<Lambertian>(Blue);
    std::shared_ptr<Material> lambertian4 = std::make_shared<Lambertian>(White);
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

    Vec3f verts[4] = {{-6,-3,-4}, {6,-3,-4}, {6,-3,-16}, {-6,-3,-16}};
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};
    Vec2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    std::unique_ptr<Object> mesh = std::make_unique<TriangleMesh>(verts, vertIndex, 2, st);
    mesh->set_material(lambertian4);
    scene.add(std::move(mesh));

    std::unique_ptr<Light> light1 = std::make_unique<Light>(Vec3f(-10, 12, -20), 0.5f);
    std::unique_ptr<Light> light2 = std::make_unique<Light>(Vec3f(20, 20, 0), 0.5f);
    scene.add(std::move(light1));
    scene.add(std::move(light2));

    RayTracer tracer(CHANNEL_NUM);
    tracer.render(scene);

    stbi_write_png("./Tracer.png", tracer.get_width(), tracer.get_height(),
                   CHANNEL_NUM, tracer.get_image(), tracer.get_width() * CHANNEL_NUM);
    return 0;
}
