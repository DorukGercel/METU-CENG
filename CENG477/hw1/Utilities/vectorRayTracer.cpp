#include "vectorRayTracer.h"

using namespace parser;

Ray rayCalculator(Vec3f originPosition, Vec3f otherPosition){
    
    Vec3f direction = subtraction(otherPosition, originPosition);
    Ray resultRay;

    resultRay.originPosition = originPosition;
    resultRay.direction = direction;

    return resultRay;
}

Vec3f addition(Vec3f v1, Vec3f v2){
    
    Vec3f result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
}

Vec3f subtraction(Vec3f v1, Vec3f v2){

    Vec3f result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

Vec3f scalerProduct(Vec3f v, float c){

    Vec3f result;

    result.x = v.x * c;
    result.y = v.y * c;
    result.z = v.z * c;

    return result;
}

Vec3f vector_to_vector_Product(Vec3f v1, Vec3f v2){
    Vec3f result;

    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;

    return result;
}

float dotProduct(Vec3f v1, Vec3f v2){

    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

}

Vec3f crossProduct(Vec3f v1, Vec3f v2){

    Vec3f result;

    result.x = v1.y * v2.z - v2.y * v1.z;
    result.y = v1.z * v2.x - v2.z * v1.x;
    result.z = v1.x * v2.y - v2.x * v1.y;

    return result;
}

float cosineCalculator(Vec3f v1, Vec3f v2){

    float cosineResult = dotProduct(v1, v2)/(vectorLengthCalculator(v1) * vectorLengthCalculator(v2));

    return cosineResult;

}

float vectorLengthCalculator(Vec3f v){

    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

}

Vec3f vectorNormalizer(Vec3f v){

    Vec3f result;
    float vectorLength = vectorLengthCalculator(v);

    result.x = v.x / vectorLength;
    result.y = v.y / vectorLength;
    result.z = v.z / vectorLength;

    return result;
}

//The normalization operation is also done inside of the function for v1 and v2
Vec3f halfVectorCalculator(Vec3f v1, Vec3f v2){

    Vec3f w1 = vectorNormalizer(v1);
    Vec3f w2 = vectorNormalizer(v2);
    
    Vec3f result = vectorNormalizer(addition(w1, w2));

    return result;
}


Ray camera_to_plane_VectorCalculator(Vec3f cameraPosition, Vec3f pixelPosition){

    return rayCalculator(cameraPosition, pixelPosition);
}

Ray x_to_camera_VectorCalculator(Vec3f xPosition, Vec3f cameraPosition){

    return rayCalculator(xPosition, cameraPosition);
}

Ray x_to_lightSource_VectorCalculator(Vec3f xPosition, Vec3f lightSourcePosition){

    return rayCalculator(xPosition, lightSourcePosition);
}

void vectorPrint(Vec3f v){

    std::cout << "Direction: " << v.x << " " << v.y << " " << v.z << std::endl;

}
