#pragma once

#include "triangular.h"
#include "vector3f.h"
#include "vector2f.h"

namespace fst
{
    class Ray;
    struct HitRecord;

    class Triangle : public Triangular
    {
    public:
        Triangle(const math::Vector3f& v0, const math::Vector3f& edge1, const math::Vector3f& edge2, const math::Vector2f& coord1, const math::Vector2f& coord2,const math::Vector2f& coord3);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const override;
        bool intersectShadowRay(const Ray& ray, float max_distance) const override;

        math::Vector3f m_v0, m_edge1, m_edge2;
        math::Vector3f m_normal;

        math::Vector2f m_coord1, m_coord2, m_coord3;
    private:
    };
}
