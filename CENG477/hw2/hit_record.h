#pragma once

#include "vector3f.h"

namespace fst
{
    struct HitRecord
    {
        math::Vector3f normal;
        float distance;
        int material_id;

        /*If 0 sphere; If 1 Mesh*/
        int typeOfIntersectedObject;
        
        /*Shows the id of intersected object*/
        int idOfIntersectedSphere;
        int idOfIntersectedMesh;
        /*No of the triangle intersected in the mesh*/
        int noOfTriangle;
    };
}