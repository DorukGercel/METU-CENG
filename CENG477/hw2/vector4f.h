#pragma once
#include <iostream>

namespace fst
{
    namespace math
    {
        struct Vector4f
        {
            float x, y, z, w;

            Vector4f()=default;

            explicit Vector4f(float a)
                : x(a)
                , y(a)
                , z(a)
                , w(a)
            {}
            Vector4f(float a, float b, float c, float d)
                : x(a)
                , y(b)
                , z(c)
                , w(d)
            {}

            void printVector4x4(Vector4f* vector){
                std::cout << vector[0].x << " " << vector[0].y << " " << vector[0].z << " " << vector[0].w << std::endl;
                std::cout << vector[1].x << " " << vector[1].y << " " << vector[1].z << " " << vector[1].w << std::endl;
                std::cout << vector[2].x << " " << vector[2].y << " " << vector[2].z << " " << vector[2].w << std::endl;
                std::cout << vector[3].x << " " << vector[3].y << " " << vector[3].z << " " << vector[3].w << std::endl;
                std::cout << std::endl;
            }
        };
    }
}