#include "mathHelper.h"


float determinantCalculator(Vec3f* matrix, Vec3f* altMatrix){
    float detA, beta, gama, t;

    float row1 = (matrix[1].y * matrix[2].z - matrix[2].y * matrix[1].z);
    float row2 = (matrix[2].x * matrix[1].z - matrix[1].x * matrix[2].z);
    float row3 = (matrix[1].x * matrix[2].y - matrix[2].x * matrix[1].y);
    
    //Determinant for A

    detA = matrix[0].x * row1 +
          matrix[0].y * row2 +
          matrix[0].z * row3;
    
    if(detA == 0){
          return -1;
    }

    //Determinant for beta
    beta = (altMatrix[0].x * row1 +
          altMatrix[0].y * row2 +
          altMatrix[0].z * row3) / detA;

    if(beta < 0){
          return -1;
    }

    //Determinant for gama

    gama = (altMatrix[0].x * (matrix[2].y * matrix[0].z - matrix[0].y * matrix[2].z) +
          altMatrix[0].y * (matrix[0].x * matrix[2].z - matrix[2].x * matrix[0].z) +
          altMatrix[0].z * (matrix[2].x * matrix[0].y - matrix[0].x * matrix[2].y))/detA;

    if(gama < 0){
          return -1;
    }

    if(beta + gama > 1){
          return -1;
    }
    
    //Determinant for t

    t = (altMatrix[0].x * (matrix[0].y * matrix[1].z - matrix[1].y * matrix[0].z) +
          altMatrix[0].y * (matrix[1].x * matrix[0].z - matrix[0].x * matrix[1].z) +
          altMatrix[0].z * (matrix[0].x * matrix[1].y - matrix[0].y * matrix[1].x))/detA;

    if(t < T_MIN || t > T_MAX){
          return -1;
    }

    return t;
}