
#ifndef RAYTRACER_TRIANGLEMESH_H
#define RAYTRACER_TRIANGLEMESH_H

#include "Object.h"
#include "Intersection.h"
#include "Bound.h"
#include "Material.h"

class Triangle : public Object{
public:
    Vec3f v0, v1, v2;
    Vec3f e1, e2;
    Vec3f normal;

    Triangle(Vec3f _v0, Vec3f _v1, Vec3f _v2){
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = e1.cross(e2).normalize();
    }

    bool intersect(const Vec3f& orig, const Vec3f& dir, float& t_near, uint32_t& index, Vec2f& bary_uv) const override {
        return false;
    }

    void get_surface_data(const Vec3f& hit_pos, Vec3f& hit_normal, Vec2f& tex, const uint32_t& index, const Vec2f& bary_uv,
                     Vec2f& st) const override { }

    Intersection get_intersection(Ray ray) override {
        return Intersection();
    }

    Bound get_bound() const override { return union_bounds(Bound(v0, v1), v2); }

    ~Triangle() override = default;
};

class TriangleMesh : public Object{
public:
    uint32_t num_triangles;
    std::unique_ptr<Vec3f[]> vertices;
    std::unique_ptr<uint32_t[]> vertexIndex;
    std::unique_ptr<Vec2f[]> stCoordinates;

    TriangleMesh(const Vec3f* verts, const uint32_t* vertsIndex, const uint32_t& numTris, const Vec2f* st){
        uint32_t maxIndex = 0;
        for (uint32_t i = 0; i < numTris * 3; ++i)
            if (vertsIndex[i] > maxIndex)
                maxIndex = vertsIndex[i];
        maxIndex += 1;
        vertices = std::unique_ptr<Vec3f[]>(new Vec3f[maxIndex]);
        memcpy(vertices.get(), verts, sizeof(Vec3f) * maxIndex);
        vertexIndex = std::unique_ptr<uint32_t[]>(new uint32_t[numTris * 3]);
        memcpy(vertexIndex.get(), vertsIndex, sizeof(uint32_t) * numTris * 3);
        num_triangles = numTris;
        stCoordinates = std::unique_ptr<Vec2f[]>(new Vec2f[maxIndex]);
        memcpy(stCoordinates.get(), st, sizeof(Vec2f) * maxIndex);
    }

    void get_surface_data(const Vec3f& hit_pos, Vec3f& hit_normal, Vec2f& tex, const uint32_t& index, const Vec2f& bary_uv, Vec2f& st) const override {
        const Vec3f& v0 = vertices[vertexIndex[index * 3]];
        const Vec3f& v1 = vertices[vertexIndex[index * 3 + 1]];
        const Vec3f& v2 = vertices[vertexIndex[index * 3 + 2]];
        Vec3f e0 = (v1 - v0).normalize();
        Vec3f e1 = (v2 - v1).normalize();
        hit_normal = e0.cross(e1).normalize();
        const Vec2f& st0 = stCoordinates[vertexIndex[index * 3]];
        const Vec2f& st1 = stCoordinates[vertexIndex[index * 3 + 1]];
        const Vec2f& st2 = stCoordinates[vertexIndex[index * 3 + 2]];
        st = st0 * (1 - tex.x - tex.y) + st1 * tex.x + st2 * tex.y;
    }

    //MOLLER TRUMBORE
    bool ray_triangle_intersect(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, const Vec3f& orig,
                              const Vec3f& dir, float& tnear, float& u, float& v) const
    {
        Vec3f e1 = v1 - v0;
        Vec3f e2 = v2 - v0;
        Vec3f s = orig - v0;
        Vec3f s1 = dir.cross(e2);
        Vec3f s2 = s.cross(e1);

        float t = 0.f;
        float b1 = 0.f;
        float b2 = 0.f;
        float s1e1 = s1.dot(e1);

        t = s2.dot(e2) / s1e1;
        b1 = s1.dot(s) / s1e1;
        b2 = s2.dot(dir) / s1e1;
        if(t > 0 && b1 >=0 && b2 >= 0 && (1 - b1 - b2) >= 0 && t >= 0)
        {
            u = b1;
            v = b2;
            Vec3f intersect_point = (1 - b1 - b2) * v0 + b1 * v1 + b2 * v2;
            Vec3f ray = intersect_point - orig;
            tnear = t;
            return true;
        }

        return false;
    }

    bool intersect(const Vec3f& orig, const Vec3f& dir, float& t_near, uint32_t& index , Vec2f& bary_uv) const override{
        bool intersect = false;
        for (uint32_t k = 0; k < num_triangles; ++k)
        {
            const Vec3f& v0 = vertices[vertexIndex[k * 3]];
            const Vec3f& v1 = vertices[vertexIndex[k * 3 + 1]];
            const Vec3f& v2 = vertices[vertexIndex[k * 3 + 2]];
            float t, u, v;
            if (ray_triangle_intersect(v0, v1, v2, orig, dir, t, u, v) && t < t_near)
            {
                t_near = t;
                bary_uv.x = u;
                bary_uv.y = v;
                index = k;
                intersect |= true;
            }
        }

        return intersect;
    }

    Bound get_bound() const override { return bound; };

    Intersection get_intersection(Ray ray) override{
        Intersection intersection;
        return intersection;
    }

    ~TriangleMesh() = default;

private:
    Bound bound;
};


#endif //RAYTRACER_TRIANGLEMESH_H
