#include "./mirrorShading.h"

//Normalization of the vectors is also done
//Given Vector wo must be directed towards the camera
Ray mirrorRayCalculator(Vec3f* pointX ,Vec3f* vectorWo, Vec3f* normalVector){
    
    Vec3f normalizedNormal = vectorNormalizer(*normalVector);
    Vec3f normalizedWo = vectorNormalizer(*vectorWo);

    float cosTheta = cosineCalculator(normalizedWo, normalizedNormal);

    Vec3f vectorWr = addition(
                             scalerProduct(normalizedWo, -1), 
                             scalerProduct(normalizedNormal, 2*cosTheta));

   Ray rayWr;
   rayWr.originPosition = *pointX;
   rayWr.direction = vectorWr;

   return rayWr;
}