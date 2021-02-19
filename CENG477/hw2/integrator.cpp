#include "integrator.h"
#include "image.h"
#include "ctpl_stl.h"

namespace fst
{
    Integrator::Integrator(const parser::Scene& parser)
    {
        m_scene.loadFromParser(parser);
    }

    math::Vector3f Integrator::renderPixel(const Ray& ray, int depth) const
    {
        if (depth > m_scene.max_recursion_depth)
        {
            return math::Vector3f(0.0f, 0.0f, 0.0f);
        }

        HitRecord hit_record;
        auto result = m_scene.intersect(ray, hit_record, std::numeric_limits<float>::max());

        if (!result)
        {
            return m_scene.background_color;
        }

        auto& material = m_scene.materials[hit_record.material_id - 1];
        auto color = material.get_ambient() * m_scene.ambient_light;
        auto intersection_point = ray.getPoint(hit_record.distance);

        /*Required for diffuse*/
        int statusCode = 0;
        math::Vector3f textureConstant;

        /*Type of Intersected Object is Sphere*/
        if(hit_record.typeOfIntersectedObject == 0){

            const Sphere* currentSphere = &m_scene.spheres[hit_record.idOfIntersectedSphere];

            /*Enter if sphere has texture*/
            if(currentSphere->m_texture_id >= 0){
                math::Vector3f centerOfSphere = currentSphere->m_center;

                math::Vector3f norm_U_Vector = math::normalize(currentSphere->u - centerOfSphere);
                math::Vector3f norm_V_Vector = math::normalize(currentSphere->v - centerOfSphere);
                math::Vector3f norm_W_Vector = math::normalize(currentSphere->w - centerOfSphere);

                /*Convert the intersection point into a homogenous coordinate*/
                math::Vector3f homogenousIntersectedPoint(intersection_point.x, intersection_point.y, intersection_point.z);
                math::Vector3f translatedPoint;
                math::Vector3f rotatedPoint;

                /*Bring the center of the sphere to the origin*/
                Translation centerTranslation(-1*centerOfSphere.x, -1*centerOfSphere.y, -1*centerOfSphere.z);

                //std::cout << "Center: " << centerOfSphere.x << " " << centerOfSphere.y << " " << centerOfSphere.z << " " << std::endl;
                //centerTranslation.translationMatrix->printVector4x4(centerTranslation.translationMatrix);

                translatedPoint.x = homogenousIntersectedPoint.x - centerOfSphere.x;
                translatedPoint.y = homogenousIntersectedPoint.y - centerOfSphere.y;
                translatedPoint.z = homogenousIntersectedPoint.z - centerOfSphere.z;

                /*Form matrix for transformation uvw to xyz*/
                math::Vector4f uvw_to_xyz[4];
        
                uvw_to_xyz[0].x = norm_U_Vector.x;
                uvw_to_xyz[0].y = norm_U_Vector.y;
                uvw_to_xyz[0].z = norm_U_Vector.z;
                uvw_to_xyz[0].w = 0;

                uvw_to_xyz[1].x = norm_V_Vector.x;
                uvw_to_xyz[1].y = norm_V_Vector.y;
                uvw_to_xyz[1].z = norm_V_Vector.z;
                uvw_to_xyz[1].w = 0;

                uvw_to_xyz[2].x = norm_W_Vector.x;
                uvw_to_xyz[2].y = norm_W_Vector.y;
                uvw_to_xyz[2].z = norm_W_Vector.z;
                uvw_to_xyz[2].w = 0;

                uvw_to_xyz[3].x = 0;
                uvw_to_xyz[3].y = 0;
                uvw_to_xyz[3].z = 0;
                uvw_to_xyz[3].w = 1;

                /*Obtain the required intersection point*/
                rotatedPoint = transformCoordinate(uvw_to_xyz, &translatedPoint);

                int textureID = m_scene.spheres[hit_record.idOfIntersectedSphere].m_texture_id;
            
                int textureWidth = m_scene.textures[textureID-1].m_width;
                int textureHeight = m_scene.textures[textureID-1].m_height;

                float omega = acos((rotatedPoint.y) / (currentSphere->m_radius));
                float alpha = atan2(rotatedPoint.z, rotatedPoint.x);

                float u_Texture = (-alpha + PI)/(2*PI);
                float v_Texture = omega/(PI);
                
                /*Clamp Apperance*/
                if(m_scene.textures[textureID-1].m_appearance.compare("clamp") == 0){
                    if(u_Texture < 0){
                        u_Texture = 0;
                    }
                    if(u_Texture > 1){
                        u_Texture = 1;
                    }
                    if(v_Texture < 0){
                        v_Texture = 0;
                    }
                    if(v_Texture > 1){
                        v_Texture = 1;
                    }
                }
                /*Repeat Apperance*/
                if(m_scene.textures[textureID-1].m_appearance.compare("repeat") == 0){
                    if(u_Texture < 0){
                        u_Texture = 0;
                    }
                    if(v_Texture < 0){
                        v_Texture = 0;
                    }
                    u_Texture = u_Texture - (int)u_Texture;
                    v_Texture = v_Texture - (int)v_Texture;
                }

                float i_Texture = u_Texture * textureWidth;
                float j_Texture = v_Texture * textureHeight;

                /*Nearest Interpolation*/
                if(m_scene.textures[textureID-1].m_interpolation.compare("nearest") == 0){
                    i_Texture = round(i_Texture);
                    j_Texture = round(j_Texture);

                    int i = i_Texture;
                    int j = j_Texture;

                    textureConstant.x = m_scene.textures[textureID-1].m_image[j*textureWidth*3 + i*3];
                    textureConstant.y = m_scene.textures[textureID-1].m_image[j*textureWidth*3 + i*3 + 1];
                    textureConstant.z = m_scene.textures[textureID-1].m_image[j*textureWidth*3 + i*3 + 2];
                }
                /*Bilinear Interpolation*/
                else if(m_scene.textures[textureID-1].m_interpolation.compare("bilinear") == 0){
                    int p = i_Texture;
                    int q = j_Texture;

                    float dx = i_Texture - p;
                    float dy = j_Texture - q;

                    textureConstant.x = m_scene.textures[textureID-1].m_image[q*textureWidth*3 + p*3]*(1-dx)*(1-dy) + 
                                        m_scene.textures[textureID-1].m_image[q*textureWidth*3 + ((p+1)%textureWidth)*3]*dx*(1-dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + p*3]*(1-dx)*(dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + ((p+1)%textureWidth)*3]*(dx)*(dy);
                    
                    textureConstant.y = m_scene.textures[textureID-1].m_image[q*textureWidth*3 + p*3 + 1]*(1-dx)*(1-dy) + 
                                        m_scene.textures[textureID-1].m_image[q*textureWidth*3 + ((p+1)%textureWidth)*3 + 1]*dx*(1-dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + p*3 + 1]*(1-dx)*(dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + ((p+1)%textureWidth)*3 + 1]*(dx)*(dy);

                    textureConstant.z = m_scene.textures[textureID-1].m_image[q*textureWidth*3 + p*3 + 2]*(1-dx)*(1-dy) + 
                                        m_scene.textures[textureID-1].m_image[q*textureWidth*3 + ((p+1)%textureWidth)*3 + 2]*dx*(1-dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + p*3 + 2]*(1-dx)*(dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + ((p+1)%textureWidth)*3 + 2]*(dx)*(dy);
                }
                 
                /*Replace Methods*/
                if(m_scene.textures[textureID-1].m_decalMode.compare("replace_kd") == 0){
                    statusCode = 1;
                }
                else if(m_scene.textures[textureID-1].m_decalMode.compare("blend_kd") == 0){
                    statusCode = 2;
                }
                else if(m_scene.textures[textureID-1].m_decalMode.compare("replace_all") == 0){
                    statusCode = 3;
                }
            }
        }
        /*Type of Intersected Object is Mesh*/
        else if(hit_record.typeOfIntersectedObject == 1){
            const Mesh* currentMesh =  &m_scene.meshes[hit_record.idOfIntersectedMesh];

            /*Enter if mesh has texture*/
            if(currentMesh->m_texture_id >= 0){
                const Triangle * currentTriangle = &currentMesh->m_triangles[hit_record.noOfTriangle];

                math::Vector3f a_Vertex_Triangle, b_Edge_Triangle, c_Edge_Triangle;
                float detA, beta, gama;
                float bx_ax, cx_ax, bybz_ayaz, cycz_ayaz, x_ax, yz_ayaz;
                
                a_Vertex_Triangle = currentTriangle->m_v0;
                b_Edge_Triangle = currentTriangle->m_edge1;
                c_Edge_Triangle = currentTriangle->m_edge2;

                bx_ax = b_Edge_Triangle.x;
                cx_ax = c_Edge_Triangle.x;
                bybz_ayaz = b_Edge_Triangle.y + b_Edge_Triangle.z;
                cycz_ayaz = c_Edge_Triangle.y + b_Edge_Triangle.z;
                x_ax = intersection_point.x - a_Vertex_Triangle.x;
                yz_ayaz = (intersection_point.y + intersection_point.z) - (a_Vertex_Triangle.y + a_Vertex_Triangle.z);
                
                detA = bx_ax * cycz_ayaz - cx_ax*bybz_ayaz; 
                beta = (x_ax * cycz_ayaz - cx_ax * yz_ayaz)/detA;
                gama = (bx_ax * yz_ayaz - x_ax * bybz_ayaz)/detA;

                int textureID = m_scene.meshes[hit_record.idOfIntersectedMesh].m_texture_id;
            
                int textureWidth = m_scene.textures[textureID-1].m_width;
                int textureHeight = m_scene.textures[textureID-1].m_height;

                float u_Texture, v_Texture;

                u_Texture = currentTriangle->m_coord1.x + beta * (currentTriangle->m_coord2.x - currentTriangle->m_coord1.x) + gama * (currentTriangle->m_coord3.x - currentTriangle->m_coord1.x);
                v_Texture = currentTriangle->m_coord1.y + beta * (currentTriangle->m_coord2.y - currentTriangle->m_coord1.y) + gama * (currentTriangle->m_coord3.y - currentTriangle->m_coord1.y);

                /*Clamp Apperance*/
                if(m_scene.textures[textureID-1].m_appearance.compare("clamp") == 0){
                    if(u_Texture < 0){
                        u_Texture = 0;
                    }
                    if(u_Texture > 1){
                        u_Texture = 1;
                    }
                    if(v_Texture < 0){
                        v_Texture = 0;
                    }
                    if(v_Texture > 1){
                        v_Texture = 1;
                    }
                }
                /*Repeat Apperance*/
                if(m_scene.textures[textureID-1].m_appearance.compare("repeat") == 0){
                    if(u_Texture < 0){
                        u_Texture = 0;
                    }
                    if(v_Texture < 0){
                        v_Texture = 0;
                    }
                    u_Texture = u_Texture - (int)u_Texture;
                    v_Texture = v_Texture - (int)v_Texture;
                }

                float i_Texture = u_Texture * textureWidth;
                float j_Texture = v_Texture * textureHeight;

                /*Nearest Interpolation*/
                if(m_scene.textures[textureID-1].m_interpolation.compare("nearest") == 0){
                    i_Texture = round(i_Texture);
                    j_Texture = round(j_Texture);

                    int i = i_Texture;
                    int j = j_Texture;

                    textureConstant.x = m_scene.textures[textureID-1].m_image[j*textureWidth*3 + i*3];
                    textureConstant.y = m_scene.textures[textureID-1].m_image[j*textureWidth*3 + i*3 + 1];
                    textureConstant.z = m_scene.textures[textureID-1].m_image[j*textureWidth*3 + i*3 + 2];
                }
                /*Bilinear Interpolation*/
                else if(m_scene.textures[textureID-1].m_interpolation.compare("bilinear") == 0){
                    int p = i_Texture;
                    int q = j_Texture;

                    float dx = i_Texture - p;
                    float dy = j_Texture - q;

                    textureConstant.x = m_scene.textures[textureID-1].m_image[q*textureWidth*3 + p*3]*(1-dx)*(1-dy) + 
                                        m_scene.textures[textureID-1].m_image[q*textureWidth*3 + ((p+1)%textureWidth)*3]*dx*(1-dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + p*3]*(1-dx)*(dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + ((p+1)%textureWidth)*3]*(dx)*(dy);
                    
                    textureConstant.y = m_scene.textures[textureID-1].m_image[q*textureWidth*3 + p*3 + 1]*(1-dx)*(1-dy) + 
                                        m_scene.textures[textureID-1].m_image[q*textureWidth*3 + ((p+1)%textureWidth)*3 + 1]*dx*(1-dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + p*3 + 1]*(1-dx)*(dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + ((p+1)%textureWidth)*3 + 1]*(dx)*(dy);

                    textureConstant.z = m_scene.textures[textureID-1].m_image[q*textureWidth*3 + p*3 + 2]*(1-dx)*(1-dy) + 
                                        m_scene.textures[textureID-1].m_image[q*textureWidth*3 + ((p+1)%textureWidth)*3 + 2]*dx*(1-dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + p*3 + 2]*(1-dx)*(dy) +
                                        m_scene.textures[textureID-1].m_image[((q+1)%textureHeight)*textureWidth*3 + ((p+1)%textureWidth)*3 + 2]*(dx)*(dy);
                }

                /*Replace Methods*/
                if(m_scene.textures[textureID-1].m_decalMode.compare("replace_kd") == 0){
                    statusCode = 1;
                }
                else if(m_scene.textures[textureID-1].m_decalMode.compare("blend_kd") == 0){
                    statusCode = 2;
                }
                else if(m_scene.textures[textureID-1].m_decalMode.compare("replace_all") == 0){
                    statusCode = 3;
                }


            }
        }

        for (auto& light : m_scene.point_lights)
        {
            auto to_light = light.get_position() - intersection_point;
            auto light_pos_distance = math::length(to_light);
            to_light = to_light / light_pos_distance;

            Ray shadow_ray(intersection_point + m_scene.shadow_ray_epsilon * to_light, to_light);

            if (!m_scene.intersectShadowRay(shadow_ray, light_pos_distance))
            {
                color = color + light.computeRadiance(light_pos_distance) * material.computeBrdf(to_light, -ray.get_direction(), hit_record.normal, textureConstant, statusCode);
                
                if(statusCode == 3){
                    color = color + textureConstant;
                }
            }
        }

        auto& mirror = material.get_mirror();
        if (mirror.x + mirror.y + mirror.z > 0.0f)
        {
            auto new_direction = math::reflect(ray.get_direction(), hit_record.normal);
            Ray secondary_ray(intersection_point + m_scene.shadow_ray_epsilon * new_direction, new_direction);

            return color + mirror * renderPixel(secondary_ray, depth + 1);
        }
        else
        {
            return color;
        }
    }

    void Integrator::integrate() const
    {
        for (auto& camera : m_scene.cameras)
        {
            auto& resolution = camera.get_screen_resolution();
            Image image(resolution.x, resolution.y);

            ctpl::thread_pool pool(128);
            for (int i = 0; i < resolution.x; ++i)
            {
                pool.push([i, &resolution, &camera, &image, this](int id) {
                    for (int j = 0; j < resolution.y; ++j)
                    {
                        auto ray = camera.castPrimayRay(static_cast<float>(i), static_cast<float>(j));
                        auto color = renderPixel(ray, 0);
                        image.setPixel(i, j, math::clamp(color, 0.0f, 255.0f));
                    }
                });
            }
            pool.stop(true);

            image.save(camera.get_image_name());
        }
    }

    void Integrator::modelTransformObjects(){
        modeling(&m_scene);
    }
}
