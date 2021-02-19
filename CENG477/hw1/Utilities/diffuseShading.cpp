#include "./diffuseShading.h"

Vec3f* diffuseForTriangle(Vec3f diffuseConstant, float distanceRSquare, Vec3f pointLightIntensity, Vec3f vectorWi, Vec3f* vertices, Vec3f*& normalVector, std::mutex* normalVectorMutex){    
    
    normalVectorMutex->lock();
    
    if(normalVector == NULL){
        //Normal vector of triangle
        Vec3f calculatedNormalVector = triangleNormalVectorCalculator(vertices);
        
        normalVector = new Vec3f;
        normalVector->x = calculatedNormalVector.x;
        normalVector->y = calculatedNormalVector.y;
        normalVector->z = calculatedNormalVector.z;

    }

    normalVectorMutex->unlock();

    //Cos angle between plane normal and wi
    float cosTheta = cosineCalculator(*normalVector, vectorWi);
    
    if(cosTheta <= 0){
        cosTheta = 0;
    }

    Vec3f diffuseResult = vector_to_vector_Product(scalerProduct(diffuseConstant, cosTheta), 
                                                       scalerProduct(pointLightIntensity, 1/distanceRSquare));

    Vec3f* result = new Vec3f;
    result[0] = diffuseResult;

    return result;


    /*if(normalIsCalculated){
        //Cos angle between plane normal and wi
        float cosTheta = cosineCalculator(*normalVector, vectorWi);

        if(cosTheta <= 0){
            cosTheta = 0;
        }

        Vec3f diffuseResult = vector_to_vector_Product(scalerProduct(diffuseConstant, cosTheta), 
                                                       scalerProduct(pointLightIntensity, 1/distanceRSquare));

        Vec3f* result = new Vec3f;
        result[0] = diffuseResult;

        return result;
    }
    else{
        //Normal vector of triangle
        Vec3f calculatedNormalVector = triangleNormalVectorCalculator(vertices);
        //Cos angle between plane normal and wi
        float cosTheta = cosineCalculator(calculatedNormalVector, vectorWi);

        if(cosTheta <= 0){
            cosTheta = 0;
        }

        Vec3f diffuseResult = vector_to_vector_Product(scalerProduct(diffuseConstant, cosTheta), 
                                                       scalerProduct(pointLightIntensity, 1/distanceRSquare));

        Vec3f* result = new Vec3f[2];
        result[0] = diffuseResult;
        result[1] = calculatedNormalVector;

        return result;
    }*/
    
}

Vec3f* diffuseForSphere(Vec3f diffuseConstant, float distanceRSquare, Vec3f pointX, Vec3f pointLightIntensity, Vec3f vectorWi,Vec3f* center, float radius){
    //Normal vector of sphere, point x specific
    Vec3f calculatedNormalVector = sphereNormalVectorCalculator(center, radius, &pointX);
    //Cos angle between plane normal and wi
    float cosTheta = cosineCalculator(calculatedNormalVector, vectorWi);

    if(cosTheta <= 0){
            cosTheta = 0;
    }

    Vec3f diffuseResult = vector_to_vector_Product(scalerProduct(diffuseConstant, cosTheta), 
                                                   scalerProduct(pointLightIntensity, 1/distanceRSquare));

    Vec3f* result = new Vec3f;
    result[0] = diffuseResult;

    return result;

}