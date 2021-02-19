#pragma once

#include "scene.h"
#include "modeling.h"
#include "vector3f.h"
#include <math.h>

namespace fst
{
    class Integrator
    {
    public:
        Integrator(const parser::Scene& parser);

        math::Vector3f renderPixel(const Ray& ray, int depth) const;
        void integrate() const;

        /*New Added*/
        void modelTransformObjects();

    private:
        Scene m_scene;
    };
}