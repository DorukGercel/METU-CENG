#include "modeling.h"

namespace fst{

    void modeling(Scene* scene){
        int i = 0;
        for (auto &mesh : scene->meshes){
            for(auto &triangle : mesh.m_triangles){
                
                //std::cout << "Old Normal: " << triangle.m_normal.x << " " << triangle.m_normal.y << " "<< triangle.m_normal.z << " " << std::endl;

                math::Vector3f vertex0 = triangle.m_v0;
                math::Vector3f vertex1 = triangle.m_edge1 + triangle.m_v0;
                math::Vector3f vertex2 = triangle.m_edge2 + triangle.m_v0;

                math::Vector3f newVertex0 = transformCoordinate(mesh.transformationMatrix ,&vertex0);
                math::Vector3f newVertex1 = transformCoordinate(mesh.transformationMatrix ,&vertex1);
                math::Vector3f newVertex2 = transformCoordinate(mesh.transformationMatrix ,&vertex2);
                
                math::Vector3f newEdge1 = newVertex1 - newVertex0;
                math::Vector3f newEdge2 = newVertex2 - newVertex0;
                triangle = Triangle(newVertex0, newEdge1, newEdge2, triangle.m_coord1, triangle.m_coord2, triangle.m_coord3);
                
                //std::cout << "New Normal: " << triangle.m_normal.x << " " << triangle.m_normal.y << " "<< triangle.m_normal.z << " " << std::endl;
            }
        }   
        for(auto &sphere : scene->spheres){
            
            //std::cout << i++ << std::endl;

            math::Vector3f center = sphere.m_center;
            math::Vector3f U_Point = sphere.u;
            math::Vector3f V_Point = sphere.v;
            math::Vector3f W_Point = sphere.w;
            
            math::Vector3f newCenter  = transformCoordinate(sphere.transformationMatrix, &center);
            math::Vector3f newU_Point = transformCoordinate(sphere.transformationMatrix, &U_Point);
            math::Vector3f newV_Point = transformCoordinate(sphere.transformationMatrix, &V_Point);
            math::Vector3f newW_Point = transformCoordinate(sphere.transformationMatrix, &W_Point);

            /*std::cout << "Old UVW" << std::endl;
            std::cout << sphere.u.x - center.x << " " << sphere.u.y  - center.y<< " " << sphere.u.z  - center.z << std::endl;
            std::cout << sphere.v.x  - center.x << " " << sphere.v.y  - center.y<< " " << sphere.v.z  - center.z<< std::endl;
            std::cout << sphere.w.x  - center.x<< " " << sphere.w.y  - center.y<< " " << sphere.w.z  - center.z<< std::endl;
            std::cout << std::endl;*/
            
            sphere.m_center = newCenter;
            sphere.u = newU_Point;
            sphere.v = newV_Point;
            sphere.w = newW_Point;

            float newRadius = math::length(sphere.u - sphere.m_center);
            sphere.m_radius = newRadius;

            /*std::cout << "New UVW" << std::endl;
            std::cout << sphere.u.x  - newCenter.x<< " " << sphere.u.y - newCenter.y << " " << sphere.u.z - newCenter.z << std::endl;
            std::cout << sphere.v.x - newCenter.x<< " " << sphere.v.y - newCenter.y << " " << sphere.v.z - newCenter.z << std::endl;
            std::cout << sphere.w.x - newCenter.x<< " " << sphere.w.y - newCenter.y<< " " << sphere.w.z - newCenter.z << std::endl;
            std::cout << std::endl;*/

            //std::cout << center.x <<  " " << center.y <<  " " << center.z <<  " " << std::endl;
            //std::cout << sphere.m_center.x <<  " " << sphere.m_center.y <<  " " << sphere.m_center.z <<  " " << std::endl;
        } 
    }

}