#include "scene.h"

namespace fst
{
    void Scene::loadFromParser(const parser::Scene& parser)
    {
        for (auto& camera : parser.cameras)
        {
            cameras.push_back(Camera(
                math::Vector3f(camera.position.x, camera.position.y, camera.position.z),
                math::Vector3f(camera.gaze.x, camera.gaze.y, camera.gaze.z),
                math::Vector3f(camera.up.x, camera.up.y, camera.up.z),
                math::Vector4f(camera.near_plane.x, camera.near_plane.y, camera.near_plane.z, camera.near_plane.w),
                math::Vector2f(camera.image_width, camera.image_height),
                camera.image_name,
                camera.near_distance));
        }

        for (auto& pointlight : parser.point_lights)
        {
            point_lights.push_back(PointLight(
                math::Vector3f(pointlight.position.x, pointlight.position.y, pointlight.position.z),
                math::Vector3f(pointlight.intensity.x, pointlight.intensity.y, pointlight.intensity.z)));
        }

        for (auto& material : parser.materials)
        {
            materials.push_back(Material(
                math::Vector3f(material.ambient.x, material.ambient.y, material.ambient.z),
                math::Vector3f(material.diffuse.x, material.diffuse.y, material.diffuse.z),
                math::Vector3f(material.specular.x, material.specular.y, material.specular.z),
                math::Vector3f(material.mirror.x, material.mirror.y, material.mirror.z),
                material.phong_exponent));
        }

        for (auto &translation : parser.translations)
        {
            translations.push_back(
                Translation(translation.x, translation.y, translation.z));
        }

        for (auto &scaling : parser.scalings)
        {
            scalings.push_back(
                Scaling(scaling.x, scaling.y, scaling.z));
        }

        for (auto &rotation : parser.rotations)
        {
            rotations.push_back(
                Rotation(rotation.angle, rotation.x, rotation.y, rotation.z));
        }
        /*****************************************************************************/
        //std::cout << "Translation" << std::endl;
        //for(auto &translation : translations){
        //    translation.translationMatrix->printVector4x4(translation.translationMatrix);
        //}
        //std::cout << "Scaling" << std::endl;
        //for(auto &scaling : scalings){
        //    scaling.scalingMatrix->printVector4x4(scaling.scalingMatrix);
        //}
        //std::cout << "Rotation" << std::endl;
        //for (auto &rotation: rotations){
        //    rotation.rotationMatrix->printVector4x4(rotation.rotationMatrix);
        //}
        /*****************************************************************************/
        for (auto& vertex : parser.vertex_data)
        {
            vertex_data.push_back(math::Vector3f(vertex.x, vertex.y, vertex.z));
        }

        for (auto& tex_coord : parser.tex_coord_data)
        {
            tex_coord_data.push_back(math::Vector2f(tex_coord.x, tex_coord.y));
        }

        for (auto& mesh : parser.meshes)
        {
            std::vector<Triangle> triangles;
            for (auto& face : mesh.faces)
            {
                if(mesh.texture_id >= 0){
                    triangles.push_back(Triangle(
                        vertex_data[face.v0_id - 1],
                        vertex_data[face.v1_id - 1] - vertex_data[face.v0_id - 1],
                        vertex_data[face.v2_id - 1] - vertex_data[face.v0_id - 1],
                        tex_coord_data[face.v0_id-1],
                        tex_coord_data[face.v1_id-1],
                        tex_coord_data[face.v2_id-1]));
                }
                else{
                    triangles.push_back(Triangle(
                        vertex_data[face.v0_id - 1],
                        vertex_data[face.v1_id - 1] - vertex_data[face.v0_id - 1],
                        vertex_data[face.v2_id - 1] - vertex_data[face.v0_id - 1],
                        math::Vector2f(-1,-1),
                        math::Vector2f(-1,-1),
                        math::Vector2f(-1,-1)));
                }
            }
            
            std::string transformationsString = mesh.transformations;
            std::string operationString;
            int sizeOfTransformation = transformationsString.length();

            /*Identity Matrix*/
            Scaling idetityMatrix(1,1,1);

            /*Template Matrix*/
            math::Vector4f* templateMatrix = idetityMatrix.scalingMatrix;
            
            int i = 0;
            while(i < sizeOfTransformation){

                int emptyPosition = transformationsString.find(" ");

                if(emptyPosition < 0){
                    operationString = transformationsString; 
                }
                else{
                    operationString = transformationsString.substr(0, emptyPosition);
                    transformationsString = transformationsString.substr(emptyPosition+1);
                }

                i += operationString.length() + 1;

                char name = operationString[0];
                int id = std::stoi(operationString.substr(1,emptyPosition-1));

                if(name == 't'){
                    templateMatrix = multiplication(translations[id - 1].translationMatrix, templateMatrix);
                }
                else if(name == 'r'){
                    templateMatrix = multiplication(rotations[id - 1].rotationMatrix, templateMatrix);
                }
                else if(name == 's'){
                    templateMatrix = multiplication(scalings[id - 1].scalingMatrix, templateMatrix);
                }
            }

            meshes.push_back(Mesh(std::move(triangles), mesh.material_id, mesh.texture_id,templateMatrix));

        }

        for (auto& triangle : parser.triangles)
        {
            std::vector<Triangle> triangles;
            
            if(triangle.texture_id >= 0){
                triangles.push_back(Triangle(
                    vertex_data[triangle.indices.v0_id - 1],
                    vertex_data[triangle.indices.v1_id - 1] - vertex_data[triangle.indices.v0_id - 1],
                    vertex_data[triangle.indices.v2_id - 1] - vertex_data[triangle.indices.v0_id - 1],
                    tex_coord_data[triangle.indices.v0_id - 1],
                    tex_coord_data[triangle.indices.v1_id - 1],
                    tex_coord_data[triangle.indices.v2_id - 1]));
            }
            else{
                triangles.push_back(Triangle(
                    vertex_data[triangle.indices.v0_id - 1],
                    vertex_data[triangle.indices.v1_id - 1] - vertex_data[triangle.indices.v0_id - 1],
                    vertex_data[triangle.indices.v2_id - 1] - vertex_data[triangle.indices.v0_id - 1],
                    math::Vector2f(-1,-1),
                    math::Vector2f(-1,-1),
                    math::Vector2f(-1,-1)));
            }

            std::string transformationsString = triangle.transformations;
            std::string operationString;
            int sizeOfTransformation = transformationsString.length();

            /*Identity Matrix*/
            Scaling idetityMatrix(1,1,1);

            /*Template Matrix*/
            math::Vector4f* templateMatrix = idetityMatrix.scalingMatrix;
            
            int i = 0;
            while(i < sizeOfTransformation){

                int emptyPosition = transformationsString.find(" ");

                if(emptyPosition < 0){
                    operationString = transformationsString; 
                }
                else{
                    operationString = transformationsString.substr(0, emptyPosition);
                    transformationsString = transformationsString.substr(emptyPosition+1);
                }

                i += operationString.length() + 1;

                char name = operationString[0];
                int id = std::stoi(operationString.substr(1,emptyPosition-1));

                if(name == 't'){
                    templateMatrix = multiplication(translations[id - 1].translationMatrix, templateMatrix);
                }
                else if(name == 'r'){
                    templateMatrix = multiplication(rotations[id - 1].rotationMatrix, templateMatrix);
                }
                else if(name == 's'){
                    templateMatrix = multiplication(scalings[id - 1].scalingMatrix, templateMatrix);
                }
            }

            meshes.push_back(Mesh(std::move(triangles), triangle.material_id, triangle.texture_id,templateMatrix));

        }

        for (auto& sphere : parser.spheres)
        {
            std::string transformationsString = sphere.transformations;
            std::string operationString;
            int sizeOfTransformation = transformationsString.length();

            /*Identity Matrix*/
            Scaling idetityMatrix(1,1,1);

            /*Template Matrix*/
            math::Vector4f* templateMatrix = idetityMatrix.scalingMatrix;
            
            int i = 0;
            while(i < sizeOfTransformation){

                int emptyPosition = transformationsString.find(" ");

                if(emptyPosition < 0){
                    operationString = transformationsString; 
                }
                else{
                    operationString = transformationsString.substr(0, emptyPosition);
                    transformationsString = transformationsString.substr(emptyPosition+1);
                }

                i += operationString.length() + 1;

                char name = operationString[0];
                int id = std::stoi(operationString.substr(1,emptyPosition-1));

                if(name == 't'){
                    templateMatrix = multiplication(translations[id - 1].translationMatrix, templateMatrix);
                }
                else if(name == 'r'){
                    templateMatrix = multiplication(rotations[id - 1].rotationMatrix, templateMatrix);
                }
                else if(name == 's'){
                    templateMatrix = multiplication(scalings[id - 1].scalingMatrix, templateMatrix);
                }
            }

            //std::cout << "Texture ID: " << sphere.texture_id << std::endl;

            spheres.push_back(Sphere(vertex_data[sphere.center_vertex_id - 1],
                sphere.radius, sphere.material_id, sphere.texture_id ,templateMatrix));

        }

        for(auto& texture : parser.textures){
            
            std::string imageName = texture.imageName;
            std::string interpolation = texture.interpolation;
            std::string decalMode = texture.decalMode;
            std::string apperance = texture.appearance;

            char* imageNameCharPtr = &imageName[0];

            int width, height;

            read_jpeg_header(imageNameCharPtr, width, height);

            unsigned char* image = new unsigned char[width*height*3];

            read_jpeg(imageNameCharPtr, image, width, height);

            textures.push_back(
                Texture(width, height, image, imageName, interpolation, decalMode, apperance)
            );
        }

        background_color = math::Vector3f(parser.background_color.x, parser.background_color.y, parser.background_color.z);
        ambient_light = math::Vector3f(parser.ambient_light.x, parser.ambient_light.y, parser.ambient_light.z);
        shadow_ray_epsilon = parser.shadow_ray_epsilon;
        max_recursion_depth = parser.max_recursion_depth;
    }

    bool Scene::intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const
    {
        HitRecord temp;
        float min_distance = max_distance;
        int sphereId = 0;
        int meshId = 0;
        for (auto& sphere : spheres)
        {
            if (sphere.intersect(ray, temp, min_distance))
            {
                min_distance = temp.distance;
                temp.typeOfIntersectedObject = 0;
                temp.idOfIntersectedSphere = sphereId;
                hit_record = temp;
            }
            sphereId += 1;
        }

        for (auto& mesh : meshes)
        {
            if (mesh.intersect(ray, temp, min_distance))
            {
                min_distance = temp.distance;
                temp.typeOfIntersectedObject = 1;
                temp.idOfIntersectedMesh = meshId;
                hit_record = temp;
            }
            meshId += 1;
        }

        return min_distance != max_distance;
    }

    bool Scene::intersectShadowRay(const Ray& ray, float max_distance) const
    {
        for (auto& sphere : spheres)
        {
            if (sphere.intersectShadowRay(ray, max_distance))
            {
                return true;
            }
        }


        for (auto& mesh : meshes)
        {
            if (mesh.intersectShadowRay(ray, max_distance))
            {
		return true;
            }
        }

	return false;
    }
}
