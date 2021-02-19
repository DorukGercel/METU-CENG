#pragma once
#include "vector4f.h"

namespace fst
{
    class Scaling
    {
    public:
        float x, y, z;
        math::Vector4f scalingMatrix[4];
        Scaling(float x, float y, float z);
        ~Scaling();
    };
}
