#include "positionRayTracer.h"

float pixelLengthCalculator(float firstCoordinate, float secondCoordinate, int noPixels){
    if(firstCoordinate > secondCoordinate){
        return (firstCoordinate - secondCoordinate)/noPixels;
    }
    return (secondCoordinate - firstCoordinate)/noPixels;
}

void positionPrint(parser::Vec3f position){
    
    std::cout << "Position: " << position.x << " " << position.y << " " << position.z << std::endl;
}