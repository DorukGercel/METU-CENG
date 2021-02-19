#include "./specularShading.h"

Vec3f* specularForTriangle(Vec3f specularConstant, float phongExponent, float distanceRSquare , Vec3f vectorWi, Vec3f vectorWo , Vec3f pointLightIntensity ,Vec3f* normalVector){
    
    //Half vector 
    Vec3f halfVector = halfVectorCalculator(vectorWi, vectorWo);

    //Cos angle between plane normal and half vector
    float cosAlpha = cosineCalculator(*normalVector, halfVector);
    if(cosAlpha <= 0){
        cosAlpha = 0;
    }

    //Cos^Phong Exponent
    float cosAlphaPhongExp = pow(cosAlpha, phongExponent);

    Vec3f specularResult = vector_to_vector_Product(scalerProduct(specularConstant, cosAlphaPhongExp),
                                                   scalerProduct(pointLightIntensity, 1/distanceRSquare));
    
    Vec3f* result = new Vec3f;
    result[0] = specularResult;

    return result;
    
}

Vec3f* specularForSphere(Vec3f specularConstant, float phongExponent, float distanceRSquare ,Vec3f pointX, Vec3f vectorWi, Vec3f vectorWo , Vec3f pointLightIntensity, Vec3f* center, float radius){
    //Normal vector of sphere, point x specific
    Vec3f calculatedNormalVector = sphereNormalVectorCalculator(center, radius, &pointX);
    //Half vector 
    Vec3f halfVector = halfVectorCalculator(vectorWi, vectorWo);
    //Cos angle between plane normal and half vector
    float cosAlpha = cosineCalculator(calculatedNormalVector, halfVector);

    if(cosAlpha <= 0){
            cosAlpha = 0;
    }

    //Cos^Phong Exponent
    float cosAlphaPhongExp = pow(cosAlpha, phongExponent);

    Vec3f specularResult = vector_to_vector_Product(scalerProduct(specularConstant, cosAlphaPhongExp), 
                                                   scalerProduct(pointLightIntensity, 1/distanceRSquare));

    Vec3f* result = new Vec3f;
    result[0] = specularResult;

    return result;

}