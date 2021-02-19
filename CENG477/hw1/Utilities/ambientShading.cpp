#include "./ambientShading.h"

Vec3f* ambient(Vec3f ambientConstant, Vec3f ambientLight){
    Vec3f* result = new Vec3f;
    result[0] = vector_to_vector_Product(ambientConstant, ambientLight);

    return result;

}