#include "sphere.h"
#include "hit_record.h"
#include "ray.h"

namespace fst
{
    Sphere::Sphere(const math::Vector3f& center, float radius, int material_id, int texture_id ,math::Vector4f* transformationMatrix)
        : m_center(center)
        , m_radius(radius)
        , m_material_id(material_id)
        , m_texture_id(texture_id)
    {
        for(int i = 0; i < 4; i++){
            this->transformationMatrix[i] = transformationMatrix[i];
        }
        this->u = math::Vector3f(radius, 0, 0) + m_center;
        this->v = math::Vector3f(0, radius, 0) + m_center;
        this->w = math::Vector3f(0, 0, radius) + m_center;
    }

    bool Sphere::intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const
    {
        //geometrical approach
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto distance = a - sqrtf(x);
        if (distance > 0.0f && distance < max_distance)
        {
            //Fill the intersection record.
            hit_record.distance = distance;
            hit_record.normal = math::normalize(ray.getPoint(hit_record.distance) - m_center);
            hit_record.material_id = m_material_id;

            return true;
        }
        return false;
    }

    bool Sphere::intersectShadowRay(const Ray& ray, float max_distance) const
    {
        //geometrical approach
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto distance = a - sqrtf(x);
        return distance > 0.0f && distance < max_distance;
    }
}