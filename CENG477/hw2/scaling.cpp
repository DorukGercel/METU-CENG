#include "scaling.h"
#include <iostream>

namespace fst
{
    Scaling::Scaling(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {
        scalingMatrix[0].x = x;
        scalingMatrix[0].y = 0;
        scalingMatrix[0].z = 0;
        scalingMatrix[0].w = 0;

        scalingMatrix[1].x = 0;
        scalingMatrix[1].y = y;
        scalingMatrix[1].z = 0;
        scalingMatrix[1].w = 0;

        scalingMatrix[2].x = 0;
        scalingMatrix[2].y = 0;
        scalingMatrix[2].z = z;
        scalingMatrix[2].w = 0;

        scalingMatrix[3].x = 0;
        scalingMatrix[3].y = 0;
        scalingMatrix[3].z = 0;
        scalingMatrix[3].w = 1;

        /*std::cout << "Scaling" << std::endl;
        std::cout << scalingMatrix[0].x << " " << scalingMatrix[0].y << " " << scalingMatrix[0].z << " " << scalingMatrix[0].w << " " << std::endl;
        std::cout << scalingMatrix[1].x << " " << scalingMatrix[1].y << " " << scalingMatrix[1].z << " " << scalingMatrix[1].w << " " << std::endl;
        std::cout << scalingMatrix[2].x << " " << scalingMatrix[2].y << " " << scalingMatrix[2].z << " " << scalingMatrix[2].w << " " << std::endl;
        std::cout << scalingMatrix[3].x << " " << scalingMatrix[3].y << " " << scalingMatrix[3].z << " " << scalingMatrix[3].w << " " << std::endl;
        std::cout << std::endl;*/
    }

    Scaling::~Scaling(){

    }
}
