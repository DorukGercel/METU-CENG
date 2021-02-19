#pragma once
#include "vector4f.h"

namespace fst
{
    class Translation
    {
    public:
        float x, y, z;
        math::Vector4f translationMatrix[4];
        
        Translation(float x, float y, float z);
        ~Translation();
    };
} // namespace fst
