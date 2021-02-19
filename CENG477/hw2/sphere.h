#pragma once

#include "vector3f.h"
#include "vector4f.h"

namespace fst
{
    class Ray;
    struct HitRecord;

    class Sphere
    {
    public:
        Sphere(const math::Vector3f& center, float radius, int material_id, int texture_id ,math::Vector4f* transformationMatrix);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const;
        bool intersectShadowRay(const Ray& ray, float max_distance) const;

        math::Vector4f transformationMatrix[4];
        math::Vector3f m_center;
        float m_radius;

        math::Vector3f u;
        math::Vector3f v;
        math::Vector3f w;

        int m_texture_id;

    private:
        int m_material_id;
    };
}