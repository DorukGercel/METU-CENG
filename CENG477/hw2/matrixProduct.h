#ifndef __matrixProduct_h__
#define __matrixProduct_h__
#include "vector4f.h"
#include "vector3f.h"
#include "utility.h"

namespace fst{
    
    math::Vector4f* multiplication(math::Vector4f* matrixA, math::Vector4f* matrixB);

    math::Vector3f transformCoordinate(math::Vector4f* transformationMatrix, math::Vector3f* coordinate);
}

#endif //__matrixProduct_h__
