
#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

enum MaterialType
{
    DIFFUSE_AND_GLOSSY,
    REFLECTION_AND_REFRACTION,
    REFLECTION
};

#include "Color.h"

class Material {
public:
    Material() = default;
    virtual ~Material() = 0;
    MaterialType material_type {};
};

class Lambertian : public Material{
public:
    Lambertian(const Color& color) : diffuse_color(color){
        material_type = DIFFUSE_AND_GLOSSY;
    }

    float Kd = 0.8f, Ks = 0.2f;
    Color diffuse_color;
    float specular_exp = 25;
};

class Reflection : public Material{
public:
    Reflection() { material_type = REFLECTION; }
    float ior = 1.5;   //refractive index
};

class ReflectionRefraction : public Material{
public:
    ReflectionRefraction() { material_type = REFLECTION_AND_REFRACTION; }
    float ior = 1.5;   //refractive index
    float specular_exp = 25;
    float Ks = 0.4f;
};


#endif //RAYTRACER_MATERIAL_H
