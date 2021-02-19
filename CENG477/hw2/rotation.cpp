#include "rotation.h"
#include "matrixProduct.h"
#include <iostream>

namespace fst
{
    Rotation::Rotation(float angle, float x, float y, float z)
        :angle(angle)
        ,x(x)
        ,y(y)
        ,z(z)
    {
        //std::cout << "Rotation" << std::endl;
        createRotationMatrix(angle,x,y,z);

        //rotationMatrix->printVector4x4(rotationMatrix);
    }

    Rotation::~Rotation(){

    }

    void Rotation::createRotationMatrix(float angle, float x, float y, float z){
        
        float u[3];
        float v[3];
        float w[3];
        int minIndex;

        u[0] = x;
        u[1] = y;
        u[2] = z;

        minIndex = abs(u[0]) < abs(u[1]) ? 0 : 1;
        minIndex = abs(u[minIndex]) < abs(u[2]) ? minIndex : 2;

        if(minIndex == 0){
            v[0] = 0;
            v[1] = -u[2];
            v[2] = u[1];
        }
        if(minIndex == 1){
            v[1] = 0;
            v[0] = -u[2];
            v[2] = u[0];
        }
        if(minIndex == 2){
            v[2] = 0;
            v[0] = -u[1];
            v[1] = u[0];
        }

        w[0] = u[1] * v[2] - u[2] * v[1]; 
        w[1] = u[2] * v[0] - u[0] * v[2];
        w[2] = u[0] * v[1] - u[1] * v[0];

        /*std::cout << u[0] << " " << u[1] << " " << u[2] << " " <<  std::endl;
        std::cout << v[0] << " " << v[1] << " " << v[2] << " " <<  std::endl;
        std::cout << w[0] << " " << w[1] << " " << w[2] << " " <<  std::endl;
        std::cout << std::endl;*/

        /*Form matrix for transformation uvw to xyz*/
        math::Vector4f uvw_to_xyz[4];
        
        uvw_to_xyz[0].x = u[0];
        uvw_to_xyz[0].y = u[1];
        uvw_to_xyz[0].z = u[2];
        uvw_to_xyz[0].w = 0;

        uvw_to_xyz[1].x = v[0];
        uvw_to_xyz[1].y = v[1];
        uvw_to_xyz[1].z = v[2];
        uvw_to_xyz[1].w = 0;

        uvw_to_xyz[2].x = w[0];
        uvw_to_xyz[2].y = w[1];
        uvw_to_xyz[2].z = w[2];
        uvw_to_xyz[2].w = 0;

        uvw_to_xyz[3].x = 0;
        uvw_to_xyz[3].y = 0;
        uvw_to_xyz[3].z = 0;
        uvw_to_xyz[3].w = 1;

        /*Form rotation matrix around x axis*/
        float angleInRadians = angle * (PI/180.0);
        
        /*std::cout << angle << std::endl;
        std::cout << angleInRadians << std::endl;
        std::cout << std::endl;*/

        float cosOfRotation = cos(angleInRadians);
        float sinOfRotation = sin(angleInRadians);

        math::Vector4f rotate[4];

        rotate[0].x = 1;
        rotate[0].y = 0;
        rotate[0].z = 0;
        rotate[0].w = 0;

        rotate[1].x = 0;
        rotate[1].y = cosOfRotation;
        rotate[1].z = -1*sinOfRotation;
        rotate[1].w = 0;

        rotate[2].x = 0;
        rotate[2].y = sinOfRotation;
        rotate[2].z = cosOfRotation;
        rotate[2].w = 0;

        rotate[3].x = 0;
        rotate[3].y = 0;
        rotate[3].z = 0;
        rotate[3].w = 1;

        /*Form matrix for transformation xyz to uvw*/
        math::Vector4f xyz_to_uvw[4];
        
        xyz_to_uvw[0].x = u[0];
        xyz_to_uvw[0].y = v[0];
        xyz_to_uvw[0].z = w[0];
        xyz_to_uvw[0].w = 0;

        xyz_to_uvw[1].x = u[1];
        xyz_to_uvw[1].y = v[1];
        xyz_to_uvw[1].z = w[1];
        xyz_to_uvw[1].w = 0;

        xyz_to_uvw[2].x = u[2];
        xyz_to_uvw[2].y = v[2];
        xyz_to_uvw[2].z = w[2];
        xyz_to_uvw[2].w = 0;

        xyz_to_uvw[3].x = 0;
        xyz_to_uvw[3].y = 0;
        xyz_to_uvw[3].z = 0;
        xyz_to_uvw[3].w = 1;

        /*Find the product of rotate matrix and uvw_to_xyz matrix to obtain product matrix*/
        math::Vector4f product[4];

        product[0].x = (uvw_to_xyz[0].x)*(rotate[0].x)+(uvw_to_xyz[1].x)*(rotate[0].y)+(uvw_to_xyz[2].x)*(rotate[0].z)+(uvw_to_xyz[3].x)*(rotate[0].w);
        product[0].y = (uvw_to_xyz[0].y)*(rotate[0].x)+(uvw_to_xyz[1].y)*(rotate[0].y)+(uvw_to_xyz[2].y)*(rotate[0].z)+(uvw_to_xyz[3].y)*(rotate[0].w);
        product[0].z = (uvw_to_xyz[0].z)*(rotate[0].x)+(uvw_to_xyz[1].z)*(rotate[0].y)+(uvw_to_xyz[2].z)*(rotate[0].z)+(uvw_to_xyz[3].z)*(rotate[0].w);
        product[0].w = (uvw_to_xyz[0].w)*(rotate[0].x)+(uvw_to_xyz[1].w)*(rotate[0].y)+(uvw_to_xyz[2].w)*(rotate[0].z)+(uvw_to_xyz[3].w)*(rotate[0].w);

        product[1].x = (uvw_to_xyz[0].x)*(rotate[1].x)+(uvw_to_xyz[1].x)*(rotate[1].y)+(uvw_to_xyz[2].x)*(rotate[1].z)+(uvw_to_xyz[3].x)*(rotate[1].w);
        product[1].y = (uvw_to_xyz[0].y)*(rotate[1].x)+(uvw_to_xyz[1].y)*(rotate[1].y)+(uvw_to_xyz[2].y)*(rotate[1].z)+(uvw_to_xyz[3].y)*(rotate[1].w);
        product[1].z = (uvw_to_xyz[0].z)*(rotate[1].x)+(uvw_to_xyz[1].z)*(rotate[1].y)+(uvw_to_xyz[2].z)*(rotate[1].z)+(uvw_to_xyz[3].z)*(rotate[1].w);
        product[1].w = (uvw_to_xyz[0].w)*(rotate[1].x)+(uvw_to_xyz[1].w)*(rotate[1].y)+(uvw_to_xyz[2].w)*(rotate[1].z)+(uvw_to_xyz[3].w)*(rotate[1].w);

        product[2].x = (uvw_to_xyz[0].x)*(rotate[2].x)+(uvw_to_xyz[1].x)*(rotate[2].y)+(uvw_to_xyz[2].x)*(rotate[2].z)+(uvw_to_xyz[3].x)*(rotate[2].w);
        product[2].y = (uvw_to_xyz[0].y)*(rotate[2].x)+(uvw_to_xyz[1].y)*(rotate[2].y)+(uvw_to_xyz[2].y)*(rotate[2].z)+(uvw_to_xyz[3].y)*(rotate[2].w);
        product[2].z = (uvw_to_xyz[0].z)*(rotate[2].x)+(uvw_to_xyz[1].z)*(rotate[2].y)+(uvw_to_xyz[2].z)*(rotate[2].z)+(uvw_to_xyz[3].z)*(rotate[2].w);
        product[2].w = (uvw_to_xyz[0].w)*(rotate[2].x)+(uvw_to_xyz[1].w)*(rotate[2].y)+(uvw_to_xyz[2].w)*(rotate[2].z)+(uvw_to_xyz[3].w)*(rotate[2].w);

        product[3].x = (uvw_to_xyz[0].x)*(rotate[3].x)+(uvw_to_xyz[1].x)*(rotate[3].y)+(uvw_to_xyz[2].x)*(rotate[3].z)+(uvw_to_xyz[3].x)*(rotate[3].w);
        product[3].y = (uvw_to_xyz[0].y)*(rotate[3].x)+(uvw_to_xyz[1].y)*(rotate[3].y)+(uvw_to_xyz[2].y)*(rotate[3].z)+(uvw_to_xyz[3].y)*(rotate[3].w);
        product[3].z = (uvw_to_xyz[0].z)*(rotate[3].x)+(uvw_to_xyz[1].z)*(rotate[3].y)+(uvw_to_xyz[2].z)*(rotate[3].z)+(uvw_to_xyz[3].z)*(rotate[3].w);
        product[3].w = (uvw_to_xyz[0].w)*(rotate[3].x)+(uvw_to_xyz[1].w)*(rotate[3].y)+(uvw_to_xyz[2].w)*(rotate[3].z)+(uvw_to_xyz[3].w)*(rotate[3].w);

        /*Find the product of xyz_to_uvw matrix and product matrix to obtain rotation matrix*/
        this->rotationMatrix[0].x = (product[0].x)*(xyz_to_uvw[0].x)+(product[1].x)*(xyz_to_uvw[0].y)+(product[2].x)*(xyz_to_uvw[0].z)+(product[3].x)*(xyz_to_uvw[0].w);
        this->rotationMatrix[0].y = (product[0].y)*(xyz_to_uvw[0].x)+(product[1].y)*(xyz_to_uvw[0].y)+(product[2].y)*(xyz_to_uvw[0].z)+(product[3].y)*(xyz_to_uvw[0].w);
        this->rotationMatrix[0].z = (product[0].z)*(xyz_to_uvw[0].x)+(product[1].z)*(xyz_to_uvw[0].y)+(product[2].z)*(xyz_to_uvw[0].z)+(product[3].z)*(xyz_to_uvw[0].w);
        this->rotationMatrix[0].w = (product[0].w)*(xyz_to_uvw[0].x)+(product[1].w)*(xyz_to_uvw[0].y)+(product[2].w)*(xyz_to_uvw[0].z)+(product[3].w)*(xyz_to_uvw[0].w);

        this->rotationMatrix[1].x = (product[0].x)*(xyz_to_uvw[1].x)+(product[1].x)*(xyz_to_uvw[1].y)+(product[2].x)*(xyz_to_uvw[1].z)+(product[3].x)*(xyz_to_uvw[1].w);
        this->rotationMatrix[1].y = (product[0].y)*(xyz_to_uvw[1].x)+(product[1].y)*(xyz_to_uvw[1].y)+(product[2].y)*(xyz_to_uvw[1].z)+(product[3].y)*(xyz_to_uvw[1].w);
        this->rotationMatrix[1].z = (product[0].z)*(xyz_to_uvw[1].x)+(product[1].z)*(xyz_to_uvw[1].y)+(product[2].z)*(xyz_to_uvw[1].z)+(product[3].z)*(xyz_to_uvw[1].w);
        this->rotationMatrix[1].w = (product[0].w)*(xyz_to_uvw[1].x)+(product[1].w)*(xyz_to_uvw[1].y)+(product[2].w)*(xyz_to_uvw[1].z)+(product[3].w)*(xyz_to_uvw[1].w);

        this->rotationMatrix[2].x = (product[0].x)*(xyz_to_uvw[2].x)+(product[1].x)*(xyz_to_uvw[2].y)+(product[2].x)*(xyz_to_uvw[2].z)+(product[3].x)*(xyz_to_uvw[2].w);
        this->rotationMatrix[2].y = (product[0].y)*(xyz_to_uvw[2].x)+(product[1].y)*(xyz_to_uvw[2].y)+(product[2].y)*(xyz_to_uvw[2].z)+(product[3].y)*(xyz_to_uvw[2].w);
        this->rotationMatrix[2].z = (product[0].z)*(xyz_to_uvw[2].x)+(product[1].z)*(xyz_to_uvw[2].y)+(product[2].z)*(xyz_to_uvw[2].z)+(product[3].z)*(xyz_to_uvw[2].w);
        this->rotationMatrix[2].w = (product[0].w)*(xyz_to_uvw[2].x)+(product[1].w)*(xyz_to_uvw[2].y)+(product[2].w)*(xyz_to_uvw[2].z)+(product[3].w)*(xyz_to_uvw[2].w);

        this->rotationMatrix[3].x = (product[0].x)*(xyz_to_uvw[3].x)+(product[1].x)*(xyz_to_uvw[3].y)+(product[2].x)*(xyz_to_uvw[3].z)+(product[3].x)*(xyz_to_uvw[3].w);
        this->rotationMatrix[3].y = (product[0].y)*(xyz_to_uvw[3].x)+(product[1].y)*(xyz_to_uvw[3].y)+(product[2].y)*(xyz_to_uvw[3].z)+(product[3].y)*(xyz_to_uvw[3].w);
        this->rotationMatrix[3].z = (product[0].z)*(xyz_to_uvw[3].x)+(product[1].z)*(xyz_to_uvw[3].y)+(product[2].z)*(xyz_to_uvw[3].z)+(product[3].z)*(xyz_to_uvw[3].w);
        this->rotationMatrix[3].w = (product[0].w)*(xyz_to_uvw[3].x)+(product[1].w)*(xyz_to_uvw[3].y)+(product[2].w)*(xyz_to_uvw[3].z)+(product[3].w)*(xyz_to_uvw[3].w);

        /*uvw_to_xyz->printVector4x4(uvw_to_xyz);
        rotate->printVector4x4(rotate);
        xyz_to_uvw->printVector4x4(xyz_to_uvw);
        product->printVector4x4(product);
        rotationMatrix->printVector4x4(rotationMatrix);*/
        
    }
}
