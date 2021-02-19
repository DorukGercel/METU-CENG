#pragma once

#include "triangle.h"
#include "vector4f.h"

#include <vector>

namespace fst
{
    class Mesh : public Triangular
    {
    public:
        Mesh(const std::vector<Triangle>& triangles, int material_id, int texture_id ,math::Vector4f* transformationMatrix);
        Mesh(std::vector<Triangle>&& triangles, int material_id, int texture_id ,math::Vector4f* transformationMatrix);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const override;
        bool intersectShadowRay(const Ray& ray, float max_distance) const override;

        std::vector<Triangle> m_triangles;
        math::Vector4f transformationMatrix[4];

        int m_texture_id;
    private:
	    
        int m_material_id;
        
    };
}
