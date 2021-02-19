#include "triangleRayTracer.h"

//If t < 0 then there is no intersection
float isPointInTriangle(Vec3f *vertices, Ray ray, Vec3f* &triangularMatrix, Vec3f* &cameraSolution,std::mutex* triangleMatrixMutex, bool isFromCamera){
    
    /*float** A = new float*[3];
    A[0] = new float[3];
    A[1] = new float[3];
    A[2] = new float[3];*/
    

    triangleMatrixMutex->lock();

    if(triangularMatrix == NULL){
        triangularMatrix = new Vec3f[3];

        triangularMatrix[0].x = vertices[0].x - vertices[1].x;
        triangularMatrix[0].y = vertices[0].y - vertices[1].y;
        triangularMatrix[0].z = vertices[0].z - vertices[1].z;

        triangularMatrix[1].x = vertices[0].x - vertices[2].x;
        triangularMatrix[1].y = vertices[0].y - vertices[2].y;
        triangularMatrix[1].z = vertices[0].z - vertices[2].z;

    }
    triangularMatrix[2].x = ray.direction.x;
    triangularMatrix[2].y = ray.direction.y;
    triangularMatrix[2].z = ray.direction.z;

    /*solution[0] = vertices[0].x - ray.originPosition.x;
    solution[1] = vertices[0].y - ray.originPosition.y;
    solution[2] = vertices[0].z - ray.originPosition.z;*/

    if(isFromCamera){
        if(cameraSolution == NULL){

            cameraSolution = new Vec3f;

            cameraSolution[0].x = vertices[0].x - ray.originPosition.x;
            cameraSolution[0].y = vertices[0].y - ray.originPosition.y;
            cameraSolution[0].z = vertices[0].z - ray.originPosition.z;

            float t = determinantCalculator(triangularMatrix, cameraSolution);

            triangleMatrixMutex->unlock();

            return t;
        }
        else{
            float t = determinantCalculator(triangularMatrix, cameraSolution);

            triangleMatrixMutex->unlock();

            return t;
        }
    }
    else{

        Vec3f* solution = new Vec3f;

        solution[0].x = vertices[0].x - ray.originPosition.x;
        solution[0].y = vertices[0].y - ray.originPosition.y;
        solution[0].z = vertices[0].z - ray.originPosition.z;

        float t = determinantCalculator(triangularMatrix, solution);

        triangleMatrixMutex->unlock();

        delete[] solution;

        return t;

    }


    /*A[0][0] = vertices[0].x - vertices[1].x;
    A[0][1] = vertices[0].x - vertices[2].x;*/
    /*A[0][0] = triangularMatrix[0].x;
    A[0][1] = triangularMatrix[1].x;*
    A[0][2] = ray.direction.x;*/


    /*A[1][0] = vertices[0].y - vertices[1].y;
    A[1][1] = vertices[0].y - vertices[2].y;*/
    /*A[1][0] = triangularMatrix[0].y;
    A[1][1] = triangularMatrix[1].y;
    A[1][2] = ray.direction.y;*/

    

    /*A[2][0] = vertices[0].z - vertices[1].z;
    A[2][1] = vertices[0].z - vertices[2].z*/
    /*A[2][0] = triangularMatrix[0].z;
    A[2][1] = triangularMatrix[1].z;
    A[2][2] = ray.direction.z;*/


    /*delete A[0];
    delete A[1];
    delete A[2];
    delete A;*/

    //return t;

}

//If t < 0 then there is no intersection
/*float t_FinderForTriangle(float** A, float* solution){
    
    float t = determinantCalculator(A, solution);

    return t;
}*/

Vec3f triangleNormalVectorCalculator(Vec3f *vertices){

    Vec3f side1, side2;

    side1 = subtraction(vertices[2], vertices[1]);
    side2 = subtraction(vertices[0], vertices[1]);

    return vectorNormalizer(crossProduct(side1, side2));
}