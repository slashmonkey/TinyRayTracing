
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
    MaterialType material_type;
};

//DIFFUSE_AND_GLOSSY
class Lambertian : public Material{
public:
    Lambertian(const Color& color) : diffuse_color(color){
        material_type = DIFFUSE_AND_GLOSSY;
    }
    ~Lambertian() override = default;
    float Kd = 0.8f, Ks = 0.2f;
    Color diffuse_color;
    float specular_exp = 25;
};


#endif //RAYTRACER_MATERIAL_H
