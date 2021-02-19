#include "utility.h"

Vec3f normalVectorCalculator(Vec3f vertex0, Vec3f vertex1, Vec3f vertex2){
    Vec3f edge1 = vertex1 - vertex0;
    Vec3f edge2 = vertex2 - vertex0;

    Vec3f normalVector = cross(edge1, edge2);
    
    return normalize(normalVector);
}