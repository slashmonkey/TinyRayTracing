
#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include "Math.h"

struct Color{
public:
    Color() : r(0), g(0), b(0) {};

    Color(float _r, float _g, float _b) {
        r = _r;
        g = _g;
        b = _b;
    };

    Color(const Color& c){
        r = c.r;
        g = c.g;
        b = c.b;
    };

    float R() const { return r; }
    float G() const { return g; }
    float B() const { return b; }

    template<class T>
    Color operator* (const T t) const { return Color(this->R() * t, this->G() * t, this->B() * t); }
    Color operator* (Color color) const { return Color(this->R() * color.R(), this->G() * color.G(), this->B() * color.B()); }
    Color operator+ (const Color& color) const {
        return Color(this->R() + color.R(), this->G() + color.G(), this->B() + color.B());
    }

    void clamp(){
        if (r > 255.f) r = 255.f;
        if (g > 255.f) g = 255.f;
        if (b > 255.f) b = 255.f;
    }

    Color lerp(const Color& c, float weight) const{
        Color color;
        color.r = (*this).R() * (1 - weight) + c.R() * weight;
        color.g = (*this).G() * (1 - weight) + c.G() * weight;
        color.b = (*this).B() * (1 - weight) + c.B() * weight;
        return color;
    }

private:
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
};

const Color Black = Color(0.f, 0.f, 0.f);
const Color White = Color(255.f, 255.f, 255.f);
const Color Red   = Color(255.f, 0.f, 0.f);
const Color Green = Color(0.f, 255.f, 0.f);
const Color Blue  = Color(0.f, 0.f, 255.f);
#endif //RAYTRACER_COLOR_H
