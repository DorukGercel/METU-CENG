#include "sphereRayTracer.h"

float t_FinderForSphere(Vec3f center, float radius, Ray ray){
    //Names come from discriminant equation
    float A, B, C, twoA, delta;

    A = ray.direction.x * ray.direction.x + 
        ray.direction.y * ray.direction.y + 
        ray.direction.z * ray.direction.z;
    
    B = 2 * ray.direction.x * (ray.originPosition.x - center.x) + 
        2 * ray.direction.y * (ray.originPosition.y - center.y) + 
        2 * ray.direction.z * (ray.originPosition.z - center.z);
    
    C = (ray.originPosition.x - center.x) * (ray.originPosition.x - center.x) + 
        (ray.originPosition.y - center.y) * (ray.originPosition.y - center.y) +
        (ray.originPosition.z - center.z) * (ray.originPosition.z - center.z) - (radius * radius);

    delta = B * B - 4 * A * C;

    twoA = 2*A;

    if(delta < 0.0){
        return -1;
    }
    else if(delta == 0){
        float t;
        t = -B/twoA;

        if(t >= T_MIN){
            return t;
        }
    }
    else{
        float t1, t2, sqrtDelta;

        sqrtDelta = sqrt(delta);

        t1 = (-B - sqrtDelta)/twoA;
        t2 = (-B + sqrtDelta)/twoA;

        if(t1 <= t2 && t1 >= T_MIN){
            return t1;
        }
        else if(t1 > t2 && t2 >= T_MIN){
            return t2;
        }
        else{
            return -1;
        }
    }
}


Vec3f sphereNormalVectorCalculator(Vec3f* center, float radius, Vec3f* intersectPoint){
    
    Vec3f result = subtraction(*intersectPoint, *center);

    result.x = result.x/radius;
    result.y = result.y/radius;
    result.z = result.z/radius;

    return result;
}
