#include "translation.h"
#include <iostream>

namespace fst
{
    Translation::Translation(float x, float y, float z)
        : x(x), y(y), z(z)
    {
        translationMatrix[0].x = 1;
        translationMatrix[0].y = 0;
        translationMatrix[0].z = 0;
        translationMatrix[0].w = x;

        translationMatrix[1].x = 0;
        translationMatrix[1].y = 1;
        translationMatrix[1].z = 0;
        translationMatrix[1].w = y;

        translationMatrix[2].x = 0;
        translationMatrix[2].y = 0;
        translationMatrix[2].z = 1;
        translationMatrix[2].w = z;

        translationMatrix[3].x = 0;
        translationMatrix[3].y = 0;
        translationMatrix[3].z = 0;
        translationMatrix[3].w = 1;

        /*std::cout << "Translation" << std::endl;
        std::cout << translationMatrix[0].x << " " << translationMatrix[0].y << " " << translationMatrix[0].z << " " << translationMatrix[0].w << " " << std::endl;
        std::cout << translationMatrix[1].x << " " << translationMatrix[1].y << " " << translationMatrix[1].z << " " << translationMatrix[1].w << " " << std::endl;
        std::cout << translationMatrix[2].x << " " << translationMatrix[2].y << " " << translationMatrix[2].z << " " << translationMatrix[2].w << " " << std::endl;
        std::cout << translationMatrix[3].x << " " << translationMatrix[3].y << " " << translationMatrix[3].z << " " << translationMatrix[3].w << " " << std::endl;
        std::cout << std::endl;*/
    }

    Translation::~Translation(){
        
    }
} // namespace fst
