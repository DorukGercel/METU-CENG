#include "matrixProduct.h"

namespace fst{
    
    math::Vector4f* multiplication(math::Vector4f* matrixA, math::Vector4f* matrixB){

        math::Vector4f* product = new math::Vector4f[4]();

        product[0].x = (matrixB[0].x)*(matrixA[0].x)+(matrixB[1].x)*(matrixA[0].y)+(matrixB[2].x)*(matrixA[0].z)+(matrixB[3].x)*(matrixA[0].w);
        product[0].y = (matrixB[0].y)*(matrixA[0].x)+(matrixB[1].y)*(matrixA[0].y)+(matrixB[2].y)*(matrixA[0].z)+(matrixB[3].y)*(matrixA[0].w);
        product[0].z = (matrixB[0].z)*(matrixA[0].x)+(matrixB[1].z)*(matrixA[0].y)+(matrixB[2].z)*(matrixA[0].z)+(matrixB[3].z)*(matrixA[0].w);
        product[0].w = (matrixB[0].w)*(matrixA[0].x)+(matrixB[1].w)*(matrixA[0].y)+(matrixB[2].w)*(matrixA[0].z)+(matrixB[3].w)*(matrixA[0].w);

        product[1].x = (matrixB[0].x)*(matrixA[1].x)+(matrixB[1].x)*(matrixA[1].y)+(matrixB[2].x)*(matrixA[1].z)+(matrixB[3].x)*(matrixA[1].w);
        product[1].y = (matrixB[0].y)*(matrixA[1].x)+(matrixB[1].y)*(matrixA[1].y)+(matrixB[2].y)*(matrixA[1].z)+(matrixB[3].y)*(matrixA[1].w);
        product[1].z = (matrixB[0].z)*(matrixA[1].x)+(matrixB[1].z)*(matrixA[1].y)+(matrixB[2].z)*(matrixA[1].z)+(matrixB[3].z)*(matrixA[1].w);
        product[1].w = (matrixB[0].w)*(matrixA[1].x)+(matrixB[1].w)*(matrixA[1].y)+(matrixB[2].w)*(matrixA[1].z)+(matrixB[3].w)*(matrixA[1].w);

        product[2].x = (matrixB[0].x)*(matrixA[2].x)+(matrixB[1].x)*(matrixA[2].y)+(matrixB[2].x)*(matrixA[2].z)+(matrixB[3].x)*(matrixA[2].w);
        product[2].y = (matrixB[0].y)*(matrixA[2].x)+(matrixB[1].y)*(matrixA[2].y)+(matrixB[2].y)*(matrixA[2].z)+(matrixB[3].y)*(matrixA[2].w);
        product[2].z = (matrixB[0].z)*(matrixA[2].x)+(matrixB[1].z)*(matrixA[2].y)+(matrixB[2].z)*(matrixA[2].z)+(matrixB[3].z)*(matrixA[2].w);
        product[2].w = (matrixB[0].w)*(matrixA[2].x)+(matrixB[1].w)*(matrixA[2].y)+(matrixB[2].w)*(matrixA[2].z)+(matrixB[3].w)*(matrixA[2].w);

        product[3].x = (matrixB[0].x)*(matrixA[3].x)+(matrixB[1].x)*(matrixA[3].y)+(matrixB[2].x)*(matrixA[3].z)+(matrixB[3].x)*(matrixA[3].w);
        product[3].y = (matrixB[0].y)*(matrixA[3].x)+(matrixB[1].y)*(matrixA[3].y)+(matrixB[2].y)*(matrixA[3].z)+(matrixB[3].y)*(matrixA[3].w);
        product[3].z = (matrixB[0].z)*(matrixA[3].x)+(matrixB[1].z)*(matrixA[3].y)+(matrixB[2].z)*(matrixA[3].z)+(matrixB[3].z)*(matrixA[3].w);
        product[3].w = (matrixB[0].w)*(matrixA[3].x)+(matrixB[1].w)*(matrixA[3].y)+(matrixB[2].w)*(matrixA[3].z)+(matrixB[3].w)*(matrixA[3].w);

        return product;

    }

    math::Vector3f transformCoordinate(math::Vector4f* transformationMatrix, math::Vector3f* coordinate){
        math::Vector4f* homogeneousCoordinate = new math::Vector4f();
        math::Vector3f result;

        homogeneousCoordinate->x = coordinate->x;
        homogeneousCoordinate->y = coordinate->y;
        homogeneousCoordinate->z = coordinate->z;
        homogeneousCoordinate->w = 1;

        result.x = homogeneousCoordinate->x * transformationMatrix[0].x + 
                   homogeneousCoordinate->y * transformationMatrix[0].y +
                   homogeneousCoordinate->z * transformationMatrix[0].z +
                   homogeneousCoordinate->w * transformationMatrix[0].w;

        result.y = homogeneousCoordinate->x * transformationMatrix[1].x + 
                   homogeneousCoordinate->y * transformationMatrix[1].y +
                   homogeneousCoordinate->z * transformationMatrix[1].z +
                   homogeneousCoordinate->w * transformationMatrix[1].w;

        result.z = homogeneousCoordinate->x * transformationMatrix[2].x + 
                   homogeneousCoordinate->y * transformationMatrix[2].y +
                   homogeneousCoordinate->z * transformationMatrix[2].z +
                   homogeneousCoordinate->w * transformationMatrix[2].w;

        /*result->w = homogeneousCoordinate->x * transformationMatrix[3].x + 
                    homogeneousCoordinate->y * transformationMatrix[3].y +
                    homogeneousCoordinate->z * transformationMatrix[3].z +
                    homogeneousCoordinate->w * transformationMatrix[3].w;*/
        return result;
    }
}