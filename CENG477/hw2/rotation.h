#pragma once
#include "vector4f.h"
#include "utility.h"
#include <math.h>
#define PI 22.0/7.0

namespace fst
{
    class Rotation
    {
    public:
        float angle, x, y, z;
        math::Vector4f rotationMatrix[4];

        Rotation(float angle, float x, float y, float z);
        ~Rotation();
        void createRotationMatrix(float angle, float x, float y, float z);
    };
} // namespace fst
