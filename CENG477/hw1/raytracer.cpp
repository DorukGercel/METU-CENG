#include <iostream>
#include "parser.h"
#include "ppm.h"

//Our Header files
#include <thread>
#include <mutex>
#include <string.h>

#include "./Utilities/commonDefinitions.h"
#include "./Utilities/mathHelper.h"
#include "./Utilities/positionRayTracer.h"
#include "./Utilities/vectorRayTracer.h"
#include "./Utilities/triangleRayTracer.h"
#include "./Utilities/sphereRayTracer.h"
#include "./Utilities/diffuseShading.h"
#include "./Utilities/ambientShading.h"
#include "./Utilities/specularShading.h"
#include "./Utilities/mirrorShading.h"

typedef unsigned char RGB[3];

std::mutex* triangleMatrixMutex;
std::mutex** meshMatrixMutex;

std::mutex* triangleNormalMutex;
std::mutex** meshNormalMutex;

enum TYPES{
    TRIANGLE,
    SPHERE,
    MESH,
    NONE
};

struct IntersectingObject{
    float t;
    TYPES typeOfT;
    int idOfElement;
    int idOfFace;

    Vec3f* currentVertices;
};

void imageClamper(unsigned char* image, int pixelNoR, float pixelR, float pixelG, float pixelB){
    
    pixelR = round(pixelR);
    pixelG = round(pixelG);
    pixelB = round(pixelB);

    //DO NOT DISTURB IMAGE LOCK
    //mutexImage.lock();

    if(pixelR > 255){
        image[pixelNoR] = 255;
    }
    else{
        image[pixelNoR] = (int)pixelR;
    }
    
    if(pixelG > 255){
        image[pixelNoR + 1] = 255;
    }
    else{
        image[pixelNoR + 1] = (int)pixelG;
    }

    if(pixelB > 255){
        image[pixelNoR + 2] = 255;
    }
    else{
        image[pixelNoR + 2] = (int)pixelB;
    }    

    //Unlock
    //mutexImage.unlock();
}

bool isMirror(Vec3f* mirrorCoefficient){
    if(mirrorCoefficient->x || mirrorCoefficient->y || mirrorCoefficient->z){
        return true;
    }
    else{
        return false;
    }
}

Vec3f topLeftPixelCoordinateCalculator(Camera* camera, float unitPixelLengthWidth, float unitPixelLengthHeight){
    
    Vec3f cameraPosition = camera->position;
    Vec3f upVector = camera->up;
    Vec3f gazeVector = camera->gaze;
    Vec3f rightVector = crossProduct(gazeVector, upVector);
    float distance = camera->near_distance;
     
    Vec3f mPoint = addition(cameraPosition, scalerProduct(gazeVector, distance));
    Vec3f top_left_PointCoordinate = addition(mPoint, 
                                              addition(scalerProduct(rightVector, camera->near_plane.x), 
                                                                     scalerProduct(upVector, camera->near_plane.w)));
    
    Vec3f unitShiftVector = addition(scalerProduct(upVector, -0.5 * unitPixelLengthHeight), scalerProduct(rightVector, 0.5 * unitPixelLengthWidth));

    Vec3f top_left_PixelCoordinate = addition(top_left_PointCoordinate, unitShiftVector);


    return top_left_PixelCoordinate;
}

//Returns the closest (smallest t) intersecting object
IntersectingObject* rayObjectIntersector(Ray* primaryRay, Scene* scene, Vec3f** triangleMatrix, Vec3f*** meshFaceMatrix, Vec3f** triangleSolutionMatrix, Vec3f*** meshFaceSolutionMatrix,int numberOfTriangleIntersections, int numberOfSphereIntersections, int numberOfMeshes, int* numberOfFaceIntersectionPerMesh){
    
    float t = T_MAX, templateT;
            
    //ID of element is the element id and face is for the face no of meshes 
    int idOfElement = -1;
    int idOfFace = -1;

    //Type of Intersecting low t element
    TYPES typeOfT = NONE;
    //Vertice set of intersecting low t element (can be vertice set for triangles or center of the spher)
    Vec3f* currentVertices = NULL;
    //Triangle Intersection
    for(int x = 0; x < numberOfTriangleIntersections; x++){
        
        Face triangleFace = scene->triangles[x].indices;
        
        Vec3f* verticesOfTriangle = new Vec3f[3];
        verticesOfTriangle[0] = scene->vertex_data[triangleFace.v0_id - 1];
        verticesOfTriangle[1] = scene->vertex_data[triangleFace.v1_id - 1];
        verticesOfTriangle[2] = scene->vertex_data[triangleFace.v2_id - 1];

        templateT = isPointInTriangle(verticesOfTriangle, *primaryRay, triangleMatrix[x], triangleSolutionMatrix[x],&triangleMatrixMutex[x], true);
        
        //If the required t value is found, sets the new t
        if(templateT >= 0 && templateT < t){
            
            //Deallocate the previous t from heap
            delete[] currentVertices;
            t = templateT;
            typeOfT = TRIANGLE;
            idOfElement = x;
            currentVertices = verticesOfTriangle;
        }
        else{
            //Deallocate the vertices of triangle
            delete[] verticesOfTriangle;
        }
    }
    //Sphere Intersection
    for(int x = 0; x < numberOfSphereIntersections; x++){
        
        Vec3f* sphereCenter = new Vec3f;
        sphereCenter[0] = scene->vertex_data[scene->spheres[x].center_vertex_id - 1];
        float sphereRadius = scene->spheres[x].radius;
        templateT = t_FinderForSphere(*sphereCenter, sphereRadius, *primaryRay);

        if(templateT >= 0 && templateT < t){
           //Deallocate the previous t from heap
           delete[] currentVertices;
           t = templateT;
           typeOfT = SPHERE;
           idOfElement = x;
           currentVertices = sphereCenter;
        }
        else{
            delete[] sphereCenter;
        }
    }
    //Mesh Intersection
    for(int x = 0; x < numberOfMeshes; x++){
        for(int y = 0; y < numberOfFaceIntersectionPerMesh[x]; y++){
            
            Face meshTriangularFace = scene->meshes[x].faces[y];
            Vec3f* verticesOfMeshTriangle = new Vec3f[3];
            verticesOfMeshTriangle[0] = scene->vertex_data[meshTriangularFace.v0_id - 1];
            verticesOfMeshTriangle[1] = scene->vertex_data[meshTriangularFace.v1_id - 1];
            verticesOfMeshTriangle[2] = scene->vertex_data[meshTriangularFace.v2_id - 1];
            
            templateT = isPointInTriangle(verticesOfMeshTriangle, *primaryRay, meshFaceMatrix[x][y], meshFaceSolutionMatrix[x][y],&meshMatrixMutex[x][y], true);
            
            //If the required t value is found, sets the new t
            if(templateT >= 0 && templateT < t){
                
                //Deallocate the previous t from heap
                delete[] currentVertices;
                t = templateT;
                typeOfT = MESH;
                idOfElement = x;
                idOfFace = y;
                currentVertices = verticesOfMeshTriangle;
            }
            else{
                //Deallocate the vertices of triangle
                delete[] verticesOfMeshTriangle;
            }
        }
    }

    IntersectingObject* result = new IntersectingObject;
    result->t = t;
    result->typeOfT = typeOfT;
    result->idOfElement = idOfElement;
    result->idOfFace = idOfFace;
    result->currentVertices = currentVertices;

    return result;
}

//Returns the closest (smallest t) intersecting object
IntersectingObject* mirrorRayObjectIntersector(IntersectingObject* mirrorIntersectingObject,Ray* primaryRay, Scene* scene, Vec3f** triangleMatrix, Vec3f*** meshFaceMatrix, int numberOfTriangleIntersections, int numberOfSphereIntersections, int numberOfMeshes, int* numberOfFaceIntersectionPerMesh){
    
    float t = T_MAX, templateT;
            
    //ID of element is the element id and face is for the face no of meshes 
    int idOfElement = -1;
    int idOfFace = -1;

    //Type of Intersecting low t element
    TYPES typeOfT = NONE;
    //Vertice set of intersecting low t element (can be vertice set for triangles or center of the spher)
    Vec3f* currentVertices = NULL;
    //Triangle Intersection
    for(int x = 0; x < numberOfTriangleIntersections; x++){
        
        Face triangleFace = scene->triangles[x].indices;
        
        Vec3f* verticesOfTriangle = new Vec3f[3];
        verticesOfTriangle[0] = scene->vertex_data[triangleFace.v0_id - 1];
        verticesOfTriangle[1] = scene->vertex_data[triangleFace.v1_id - 1];
        verticesOfTriangle[2] = scene->vertex_data[triangleFace.v2_id - 1];

        templateT = isPointInTriangle(verticesOfTriangle, *primaryRay, triangleMatrix[x], triangleMatrix[x], &triangleMatrixMutex[x], false);
        
        //If the required t value is found, sets the new t
        if(templateT >= 0 && templateT < t && !(mirrorIntersectingObject->idOfElement == x && mirrorIntersectingObject->typeOfT == TRIANGLE)){
            
            //Deallocate the previous t from heap
            delete[] currentVertices;
            t = templateT;
            typeOfT = TRIANGLE;
            idOfElement = x;
            currentVertices = verticesOfTriangle;
        }
        else{
            //Deallocate the vertices of triangle
            delete[] verticesOfTriangle;
        }
    }
    //Sphere Intersection
    for(int x = 0; x < numberOfSphereIntersections; x++){
        
        Vec3f* sphereCenter = new Vec3f;
        sphereCenter[0] = scene->vertex_data[scene->spheres[x].center_vertex_id - 1];
        float sphereRadius = scene->spheres[x].radius;
        templateT = t_FinderForSphere(*sphereCenter, sphereRadius, *primaryRay);

        if(templateT >= 0 && templateT < t && !(mirrorIntersectingObject->idOfElement == x && mirrorIntersectingObject->typeOfT == SPHERE)){
           //Deallocate the previous t from heap
           delete[] currentVertices;
           t = templateT;
           typeOfT = SPHERE;
           idOfElement = x;
           currentVertices = sphereCenter;
        }
        else{
            delete[] sphereCenter;
        }
    }
    //Mesh Intersection
    for(int x = 0; x < numberOfMeshes; x++){
        for(int y = 0; y < numberOfFaceIntersectionPerMesh[x]; y++){
            
            Face meshTriangularFace = scene->meshes[x].faces[y];
            Vec3f* verticesOfMeshTriangle = new Vec3f[3];
            verticesOfMeshTriangle[0] = scene->vertex_data[meshTriangularFace.v0_id - 1];
            verticesOfMeshTriangle[1] = scene->vertex_data[meshTriangularFace.v1_id - 1];
            verticesOfMeshTriangle[2] = scene->vertex_data[meshTriangularFace.v2_id - 1];
            
            templateT = isPointInTriangle(verticesOfMeshTriangle, *primaryRay, meshFaceMatrix[x][y], meshFaceMatrix[x][y],&meshMatrixMutex[x][y], false);
            
            //If the required t value is found, sets the new t
            if(templateT >= 0 && templateT < t && !(mirrorIntersectingObject->idOfElement == x && mirrorIntersectingObject->idOfFace == y && mirrorIntersectingObject->typeOfT == MESH)){
                
                //Deallocate the previous t from heap
                delete[] currentVertices;
                t = templateT;
                typeOfT = MESH;
                idOfElement = x;
                idOfFace = y;
                currentVertices = verticesOfMeshTriangle;
            }
            else{
                //Deallocate the vertices of triangle
                delete[] verticesOfMeshTriangle;
            }
        }
    }

    IntersectingObject* result = new IntersectingObject;
    result->t = t;
    result->typeOfT = typeOfT;
    result->idOfElement = idOfElement;
    result->idOfFace = idOfFace;
    result->currentVertices = currentVertices;

    return result;
}

//Checks if there is an intersecting object with the given ray
bool isThereRayObjectIntersection(Ray* ray, Scene* scene, Vec3f** triangleMatrix, Vec3f*** meshFaceMatrix, int numberOfTriangleIntersections, int numberOfSphereIntersections, int numberOfMeshes, int* numberOfFaceIntersectionPerMesh){

    float templateT;

     //Triangle Intersection
    for(int x = 0; x < numberOfTriangleIntersections; x++){
        Face triangleFace = scene->triangles[x].indices;
        Vec3f* verticesOfTriangle = new Vec3f[3];
        verticesOfTriangle[0] = scene->vertex_data[triangleFace.v0_id - 1];
        verticesOfTriangle[1] = scene->vertex_data[triangleFace.v1_id - 1];
        verticesOfTriangle[2] = scene->vertex_data[triangleFace.v2_id - 1];
        templateT = isPointInTriangle(verticesOfTriangle, *ray, triangleMatrix[x], triangleMatrix[x],&triangleMatrixMutex[x], false);
        delete[] verticesOfTriangle;
        //If the required t value is found, sets the new t
        if(templateT >= 0 && templateT <= 1){
            return true;
        }
    }
    //Sphere Intersection
    for(int x = 0; x < numberOfSphereIntersections; x++){
    
        Vec3f* sphereCenter = new Vec3f;
        sphereCenter[0] = scene->vertex_data[scene->spheres[x].center_vertex_id - 1];
        float sphereRadius = scene->spheres[x].radius;
        templateT = t_FinderForSphere(*sphereCenter, sphereRadius, *ray);
        delete[] sphereCenter;
        //If the required t value is found, sets the new t
        if(templateT >= 0 && templateT <= 1){
            return true;
        }    
    }
    //Mesh Intersection
    for(int x = 0; x < numberOfMeshes; x++){
        for(int y = 0; y < numberOfFaceIntersectionPerMesh[x]; y++){
        
            Face meshTriangularFace = scene->meshes[x].faces[y];
            Vec3f* verticesOfMeshTriangle = new Vec3f[3];
            verticesOfMeshTriangle[0] = scene->vertex_data[meshTriangularFace.v0_id - 1];
            verticesOfMeshTriangle[1] = scene->vertex_data[meshTriangularFace.v1_id - 1];
            verticesOfMeshTriangle[2] = scene->vertex_data[meshTriangularFace.v2_id - 1];
            templateT = isPointInTriangle(verticesOfMeshTriangle, *ray, meshFaceMatrix[x][y], meshFaceMatrix[x][y],&meshMatrixMutex[x][y], false);
            delete[] verticesOfMeshTriangle;
            //If the required t value is found, sets the new t
            if(templateT >= 0 && templateT <= 1){
                return true;
            }
        }
    }
    
    return false;
}

/********************************************************Shading*********************************************************************************************/
Vec3f* shading(int noRecursion, int noMaxRecursion, Ray* ray, IntersectingObject* intersectingObject, Scene* scene, int noOfLights,Vec3f** triangleNormalVectors, Vec3f*** meshFaceNormalVector, Vec3f** triangleMatrix, Vec3f*** meshFaceMatrix, int numberOfTriangleIntersections, int numberOfSphereIntersections, int numberOfMeshes, int* numberOfFaceIntersectionPerMesh){
    
    int idOfElement = intersectingObject->idOfElement;
    int idOfFace = intersectingObject->idOfFace;
    TYPES typeOfT = intersectingObject->typeOfT;
    float t = intersectingObject->t;
    Vec3f* currentVertices = intersectingObject->currentVertices;
    
    float templateT;

    //Used in storing floating results and then stored in image
    Vec3f* resultRGB = new Vec3f;

    resultRGB->x = 0;
    resultRGB->y = 0;
    resultRGB->z = 0;

    Vec3f pointX = addition(ray->originPosition, scalerProduct(ray->direction, t));

    //Triangle Shading
    if(typeOfT == TRIANGLE){

        Triangle currentTriangle = scene->triangles[idOfElement];
        Material currentTriangleMaterial = scene->materials[currentTriangle.material_id-1];

        /*bool isCalculated = true;

        if(triangleNormalVectors[idOfElement] == NULL){
            isCalculated = false;
        }*/

        for(int l = 0; l < noOfLights; l++){

            PointLight currentPointLight = scene->point_lights[l];
            Vec3f vectorWi = subtraction(currentPointLight.position, pointX);
            Vec3f shadowPointX = addition(pointX, scalerProduct(vectorWi,scene->shadow_ray_epsilon));

            Ray shadowRay;
            shadowRay.originPosition = shadowPointX;
            shadowRay.direction = vectorWi;

            bool isInShadow = isThereRayObjectIntersection(&shadowRay, scene, triangleMatrix, meshFaceMatrix,numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);
            
            /*********************************************************************************************/                        
            if(!isInShadow){
                float distanceRSquare = (pointX.x - currentPointLight.position.x) * (pointX.x - currentPointLight.position.x) + 
                  (pointX.y - currentPointLight.position.y) * (pointX.y - currentPointLight.position.y) +
                  (pointX.z - currentPointLight.position.z) * (pointX.z - currentPointLight.position.z);

                Vec3f* diffuseResult = diffuseForTriangle(currentTriangleMaterial.diffuse,
                                        distanceRSquare,
                                        currentPointLight.intensity,
                                        vectorWi,
                                        currentVertices,
                                        triangleNormalVectors[idOfElement],
                                        &triangleNormalMutex[idOfElement]);

                
                /*if(isCalculated == false){
                    triangleNormalVectors[idOfElement] = new Vec3f;
                    triangleNormalVectors[idOfElement]->x = diffuseResult[1].x;
                    triangleNormalVectors[idOfElement]->y = diffuseResult[1].y;
                    triangleNormalVectors[idOfElement]->z = diffuseResult[1].z;
                }*/

                //Unlock the lock
                //mutexTriangleNormal.unlock();

                Vec3f* specularResult = specularForTriangle(currentTriangleMaterial.specular,
                                                            currentTriangleMaterial.phong_exponent,
                                                            distanceRSquare,
                                                            vectorWi,
                                                            scalerProduct(ray->direction,-1),
                                                            currentPointLight.intensity,
                                                            triangleNormalVectors[idOfElement]);

                resultRGB->x += diffuseResult[0].x + specularResult[0].x;
                resultRGB->y += diffuseResult[0].y + specularResult[0].y;
                resultRGB->z += diffuseResult[0].z + specularResult[0].z;

                delete[] diffuseResult;
                delete[] specularResult;
            }
        }

        if(isMirror(&currentTriangleMaterial.mirror) && noRecursion <= noMaxRecursion){

                    triangleNormalMutex[idOfElement].lock();
    
                    if(triangleNormalVectors[idOfElement] == NULL){
                        
                        //Normal vector of triangle
                        Vec3f calculatedNormalVector = triangleNormalVectorCalculator(currentVertices);
        
                        triangleNormalVectors[idOfElement] = new Vec3f;
                        triangleNormalVectors[idOfElement]->x = calculatedNormalVector.x;
                        triangleNormalVectors[idOfElement]->y = calculatedNormalVector.y;
                        triangleNormalVectors[idOfElement]->z = calculatedNormalVector.z;

                    }

                    triangleNormalMutex[idOfElement].unlock();

                    //Vec3f* mirrorResult = shading(noRecursion+1, noMaxRecursion, );
                    Vec3f vectorWo = scalerProduct(ray->direction,-1);

                    Ray rayWr = mirrorRayCalculator(&pointX, &vectorWo, triangleNormalVectors[idOfElement]);

                    IntersectingObject* mirrorIntersectingObject = mirrorRayObjectIntersector(intersectingObject,&rayWr, scene, triangleMatrix, meshFaceMatrix, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);

                    Vec3f* mirrorResult;

                    if(mirrorIntersectingObject->idOfElement != -1){
                        mirrorResult = shading(noRecursion + 1, noMaxRecursion, &rayWr, mirrorIntersectingObject, scene, noOfLights, triangleNormalVectors, meshFaceNormalVector, triangleMatrix, meshFaceMatrix, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);
                    }
                    else{
                        mirrorResult = new Vec3f;
                        mirrorResult->x = scene->background_color.x;
                        mirrorResult->y = scene->background_color.y;
                        mirrorResult->z = scene->background_color.z;
                    }

                    resultRGB->x += currentTriangleMaterial.mirror.x * mirrorResult[0].x;
                    resultRGB->y += currentTriangleMaterial.mirror.y * mirrorResult[0].y;
                    resultRGB->z += currentTriangleMaterial.mirror.z * mirrorResult[0].z;

                    delete[] mirrorIntersectingObject;
                    delete[] mirrorResult;

        }

        Vec3f* ambientResult = ambient(currentTriangleMaterial.ambient, 
                                           scene->ambient_light);

        resultRGB->x += ambientResult[0].x;
        resultRGB->y += ambientResult[0].y;
        resultRGB->z += ambientResult[0].z;

        delete[] ambientResult;

        return resultRGB;
    }
    
    //Sphere Shading                 
    else if(typeOfT == SPHERE){

        Sphere currentSphere = scene->spheres[idOfElement];
        Material currentSphereMaterial = scene->materials[currentSphere.material_id - 1];

        for(int l = 0; l < noOfLights; l++){

            PointLight currentPointLight = scene->point_lights[l];
            Vec3f vectorWi = subtraction(currentPointLight.position, pointX);
            Vec3f shadowPointX = addition(pointX, scalerProduct(vectorWi,scene->shadow_ray_epsilon));

            Ray shadowRay;
            shadowRay.originPosition = shadowPointX;
            shadowRay.direction = vectorWi;

            bool isInShadow = isThereRayObjectIntersection(&shadowRay, scene, triangleMatrix, meshFaceMatrix, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);
            
            /*********************************************************************/
            if(!isInShadow){

                float distanceRSquare = (pointX.x - currentPointLight.position.x) * (pointX.x - currentPointLight.position.x) + 
                  (pointX.y - currentPointLight.position.y) * (pointX.y - currentPointLight.position.y) +
                  (pointX.z - currentPointLight.position.z) * (pointX.z - currentPointLight.position.z);

                Vec3f* diffuseResult = diffuseForSphere(currentSphereMaterial.diffuse,
                                                        distanceRSquare,
                                                        pointX,
                                                        currentPointLight.intensity,
                                                        vectorWi,
                                                        currentVertices,
                                                        currentSphere.radius);  

                Vec3f* specularResult = specularForSphere(currentSphereMaterial.specular,
                                                          currentSphereMaterial.phong_exponent,
                                                          distanceRSquare,
                                                          pointX,
                                                          vectorWi,
                                                          scalerProduct(ray->direction, -1),
                                                          currentPointLight.intensity,
                                                          currentVertices,
                                                          currentSphere.radius);

                resultRGB->x += diffuseResult[0].x + specularResult[0].x;
                resultRGB->y += diffuseResult[0].y + specularResult[0].y;
                resultRGB->z += diffuseResult[0].z + specularResult[0].z;

                delete[] diffuseResult;
                delete[] specularResult;
            }
        }

        if(isMirror(&currentSphereMaterial.mirror) && noRecursion <= noMaxRecursion){
                    
                    //Normal vector of sphere, point x specific
                    Vec3f calculatedNormalVector = sphereNormalVectorCalculator(currentVertices, currentSphere.radius, &pointX);

                    //Vec3f* mirrorResult = shading(noRecursion+1, noMaxRecursion, );
                    Vec3f vectorWo = scalerProduct(ray->direction,-1);
                    Ray rayWr = mirrorRayCalculator(&pointX, &vectorWo, &calculatedNormalVector);

                    IntersectingObject* mirrorIntersectingObject = mirrorRayObjectIntersector(intersectingObject,&rayWr, scene, triangleMatrix, meshFaceMatrix, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);

                    Vec3f* mirrorResult;

                    if(mirrorIntersectingObject->idOfElement != -1){
                        mirrorResult = shading(noRecursion + 1, noMaxRecursion, &rayWr, mirrorIntersectingObject, scene, noOfLights, triangleNormalVectors, meshFaceNormalVector, triangleMatrix, meshFaceMatrix, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);
                    }
                    else{
                        mirrorResult = new Vec3f;
                        mirrorResult->x = scene->background_color.x;
                        mirrorResult->y = scene->background_color.y;
                        mirrorResult->z = scene->background_color.z;
                    }

                    resultRGB->x += currentSphereMaterial.mirror.x * mirrorResult[0].x;
                    resultRGB->y += currentSphereMaterial.mirror.y * mirrorResult[0].y;
                    resultRGB->z += currentSphereMaterial.mirror.z * mirrorResult[0].z;

                    delete[] mirrorIntersectingObject;
                    delete[] mirrorResult;

        }

        Vec3f* ambientResult = ambient(currentSphereMaterial.ambient, 
                                           scene->ambient_light);

        resultRGB->x += ambientResult[0].x;
        resultRGB->y += ambientResult[0].y;
        resultRGB->z += ambientResult[0].z;

        delete[] ambientResult;
        
        return resultRGB;
    }
    //Mesh Shading
    else if(typeOfT == MESH){

        Mesh currentMesh = scene->meshes[idOfElement];
        Material currentMeshMaterial = scene->materials[currentMesh.material_id-1];

        /*bool isCalculated = true;

        if(meshFaceNormalVector[idOfElement][idOfFace] == NULL){
            isCalculated = false;
        }*/

        for(int l = 0; l < noOfLights; l++){

            PointLight currentPointLight = scene->point_lights[l];
            Vec3f vectorWi = subtraction(currentPointLight.position, pointX);
            Vec3f shadowPointX = addition(pointX, scalerProduct(vectorWi,scene->shadow_ray_epsilon));

            Ray shadowRay;
            shadowRay.originPosition = shadowPointX;
            shadowRay.direction = vectorWi;

            bool isInShadow = isThereRayObjectIntersection(&shadowRay, scene, triangleMatrix, meshFaceMatrix, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);
            
            /***********************************************************************************************/
            if(!isInShadow){

                float distanceRSquare = (pointX.x - currentPointLight.position.x) * (pointX.x - currentPointLight.position.x) + 
                  (pointX.y - currentPointLight.position.y) * (pointX.y - currentPointLight.position.y) +
                  (pointX.z - currentPointLight.position.z) * (pointX.z - currentPointLight.position.z);
                  
                Vec3f* diffuseResult = diffuseForTriangle(currentMeshMaterial.diffuse,
                                        distanceRSquare,
                                        currentPointLight.intensity,
                                        vectorWi,
                                        currentVertices,
                                        meshFaceNormalVector[idOfElement][idOfFace],
                                        &meshNormalMutex[idOfElement][idOfFace]);

                /*if(isCalculated == false){

                    meshFaceNormalVector[idOfElement][idOfFace] = new Vec3f;
                    meshFaceNormalVector[idOfElement][idOfFace]->x = diffuseResult[1].x;
                    meshFaceNormalVector[idOfElement][idOfFace]->y = diffuseResult[1].y;
                    meshFaceNormalVector[idOfElement][idOfFace]->z = diffuseResult[1].z;
                }*/

                //Unlock
               // mutexMeshFaceNormal.unlock();

                Vec3f* specularResult = specularForTriangle(currentMeshMaterial.specular,
                                                            currentMeshMaterial.phong_exponent,
                                                            distanceRSquare,
                                                            vectorWi,
                                                            scalerProduct(ray->direction, -1),
                                                            currentPointLight.intensity,
                                                            meshFaceNormalVector[idOfElement][idOfFace]);

                resultRGB->x += diffuseResult[0].x + specularResult[0].x;
                resultRGB->y += diffuseResult[0].y + specularResult[0].y;
                resultRGB->z += diffuseResult[0].z + specularResult[0].z;

                delete[] diffuseResult;
                delete[] specularResult;
            }
        }

        if(isMirror(&currentMeshMaterial.mirror) && noRecursion <= noMaxRecursion){

                    meshNormalMutex[idOfElement][idOfFace].lock();
    
                    if(meshFaceNormalVector[idOfElement][idOfFace] == NULL){
                        
                        //Normal vector of triangle
                        Vec3f calculatedNormalVector = triangleNormalVectorCalculator(currentVertices);
        
                        meshFaceNormalVector[idOfElement][idOfFace] = new Vec3f;
                        meshFaceNormalVector[idOfElement][idOfFace]->x = calculatedNormalVector.x;
                        meshFaceNormalVector[idOfElement][idOfFace]->y = calculatedNormalVector.y;
                        meshFaceNormalVector[idOfElement][idOfFace]->z = calculatedNormalVector.z;

                    }

                    meshNormalMutex[idOfElement][idOfFace].unlock();

                    //Vec3f* mirrorResult = shading(noRecursion+1, noMaxRecursion, );
                    Vec3f vectorWo = scalerProduct(ray->direction,-1);
                    Ray rayWr = mirrorRayCalculator(&pointX, &vectorWo, meshFaceNormalVector[idOfElement][idOfFace]);

                    IntersectingObject* mirrorIntersectingObject = mirrorRayObjectIntersector(intersectingObject,&rayWr, scene, triangleMatrix, meshFaceMatrix, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);

                    Vec3f* mirrorResult;

                    if(mirrorIntersectingObject->idOfElement != -1){
                        mirrorResult = shading(noRecursion + 1, noMaxRecursion, &rayWr, mirrorIntersectingObject, scene, noOfLights, triangleNormalVectors, meshFaceNormalVector, triangleMatrix, meshFaceMatrix, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);
                    }
                    else{
                        mirrorResult = new Vec3f;
                        mirrorResult->x = scene->background_color.x;
                        mirrorResult->y = scene->background_color.y;
                        mirrorResult->z = scene->background_color.z;
                    }

                    resultRGB->x += currentMeshMaterial.mirror.x * mirrorResult[0].x;
                    resultRGB->y += currentMeshMaterial.mirror.y * mirrorResult[0].y;
                    resultRGB->z += currentMeshMaterial.mirror.z * mirrorResult[0].z;

                    delete[] mirrorIntersectingObject;
                    delete[] mirrorResult;

        }
        
        Vec3f* ambientResult = ambient(currentMeshMaterial.ambient, 
                                           scene->ambient_light);
        
        resultRGB->x += ambientResult[0].x;
        resultRGB->y += ambientResult[0].y;
        resultRGB->z += ambientResult[0].z;
        delete[] ambientResult;

        return resultRGB;

    }
    else{
        
        return resultRGB;
    }
}

void pixelCalculator(int currentHeight ,int height, int width, float unitPixelLengthHeight ,float unitPixelLengthWidth, Vec3f* top_left_PixelCoordinate, Vec3f* rightVector, Vec3f* upVector, Scene* scene, Ray primaryRay, unsigned char* image, int numberOfTriangleIntersections, int numberOfSphereIntersections, int numberOfMeshes, int* numberOfFaceIntersectionPerMesh, Vec3f** triangleNormalVectors, Vec3f*** meshFaceNormalVectors, Vec3f** triangleMatrix, Vec3f*** meshFaceMatrix, Vec3f** triangleSolutionMatrix, Vec3f*** meshFaceSolutionMatrix ){
    //Pixels Loop
    int targetHeight = currentHeight + height;
    
    primaryRay.direction = addition(primaryRay.direction, scalerProduct(*upVector, ((float)-1)*unitPixelLengthHeight*(float)currentHeight));
    
    //Is used to return to the leftest point
    Vec3f firstDirection = primaryRay.direction;
    
    for(int i = currentHeight; i < targetHeight; i++){
        for(int j = 0; j < width; j++){
            
            IntersectingObject* intersectingObject = rayObjectIntersector(&primaryRay, scene, triangleMatrix, meshFaceMatrix, triangleSolutionMatrix, meshFaceSolutionMatrix,numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);
            
            /****************************************Start Shading Computations******************************************************************************/
            int noOfLights = scene->point_lights.size();
            
            int currentPixelNo = i*width*3 + j*3;

            if(intersectingObject->idOfElement != -1){

                Vec3f* rgbValues = shading(0, scene->max_recursion_depth, &primaryRay, intersectingObject, scene, noOfLights, triangleNormalVectors, meshFaceNormalVectors, triangleMatrix, meshFaceMatrix, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh);

                imageClamper(image, currentPixelNo, rgbValues->x, rgbValues->y, rgbValues->z);

                delete[] rgbValues;

                
            }
            else{
                image[currentPixelNo] = scene->background_color.x;
                image[currentPixelNo + 1] = scene->background_color.y;
                image[currentPixelNo + 2] = scene->background_color.z;
            }

            //Free intersecting object
            delete[] intersectingObject->currentVertices;
            delete intersectingObject;

            //One pixel remove right
            primaryRay.direction = addition(primaryRay.direction, scalerProduct(*rightVector, unitPixelLengthWidth));
        }
        //Set to the leftest pixel
        primaryRay.direction = firstDirection;

        //One pixel remove down
        primaryRay.direction = addition(primaryRay.direction, scalerProduct(*upVector, -1*unitPixelLengthHeight));
        firstDirection = primaryRay.direction;
    }
}

/********************************************MAIN*****************************************************************/
int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;
    
    scene.loadFromXml(argv[1]);

    int noOfCameras = scene.cameras.size();
    Camera camera[noOfCameras];
    
    int noMaxRecursion = scene.max_recursion_depth;

    int height;
    int width;

    float unitPixelLengthWidth;
    float unitPixelLengthHeight;

    for(int i = 0; i < noOfCameras; i++){
        camera[i] = scene.cameras[i];

        height = camera[i].image_height;
        width = camera[i].image_width;

        unitPixelLengthHeight = pixelLengthCalculator(camera[i].near_plane.z, camera[i].near_plane.w, height);
        unitPixelLengthWidth = pixelLengthCalculator(camera[i].near_plane.x, camera[i].near_plane.y, width);

        unsigned char* image = new unsigned char [height * width * 3];

        //Number of Intersections of Objects are Calculated
        int numberOfTriangleIntersections = scene.triangles.size();
        int numberOfSphereIntersections = scene.spheres.size();
        int numberOfMeshes = scene.meshes.size();

        int* numberOfFaceIntersectionPerMesh = new int[numberOfMeshes];

        //Storage for normal vectors of the triangles and the faces of the meshes are created
        Vec3f** triangleNormalVectors = new Vec3f*[numberOfTriangleIntersections];
        Vec3f** triangleMatrix = new Vec3f*[numberOfTriangleIntersections];
        Vec3f** triangleSolutionMatrix = new Vec3f*[numberOfTriangleIntersections];

        triangleNormalMutex = new std::mutex[numberOfTriangleIntersections];
        triangleMatrixMutex = new std::mutex[numberOfTriangleIntersections];

        //Assign NULL normal vectors as none of them are computed
        for(int i = 0; i < numberOfTriangleIntersections; i++){
            triangleNormalVectors[i] = NULL;
            triangleMatrix[i] = NULL;
            triangleSolutionMatrix[i] = NULL;
        }

        Vec3f*** meshFaceNormalVectors = new Vec3f**[numberOfMeshes];
        Vec3f*** meshFaceMatrix = new Vec3f**[numberOfMeshes];
        Vec3f*** meshFaceSolutionMatrix = new Vec3f**[numberOfMeshes];

        meshNormalMutex = new std::mutex*[numberOfMeshes];
        meshMatrixMutex = new std::mutex*[numberOfMeshes];

        for(int i = 0; i < numberOfMeshes; i++){
            int noOfFaces = scene.meshes[i].faces.size();
            numberOfFaceIntersectionPerMesh[i] = noOfFaces;

            meshFaceNormalVectors[i] = new Vec3f*[noOfFaces];
            meshFaceMatrix[i] = new Vec3f*[noOfFaces];
            meshFaceSolutionMatrix[i] = new Vec3f*[noOfFaces];

            meshNormalMutex[i] = new std::mutex[noOfFaces];
            meshMatrixMutex[i] = new std::mutex[noOfFaces];

            //Assign NULL normal vectors as none of them are computed
            for(int j =  0; j < noOfFaces; j++){
                meshFaceNormalVectors[i][j] = NULL;
                meshFaceMatrix[i][j] = NULL;
                meshFaceSolutionMatrix[i][j] = NULL;
            }
        }

        Vec3f upVector = camera[i].up;
        Vec3f gazeVector = camera[i].gaze;
        Vec3f rightVector = crossProduct(gazeVector, upVector);

        //Calculate Top Left Pixel Coordinate
        Vec3f top_left_PixelCoordinate = topLeftPixelCoordinateCalculator(camera + i, unitPixelLengthWidth, unitPixelLengthHeight);

        //First Ray to (0,0) pixel is Calculated
        Ray primaryRay = rayCalculator(camera[i].position, top_left_PixelCoordinate);
    
        int heightDiv8 = height/8;

        std::thread th1(pixelCalculator, 0, heightDiv8, width, unitPixelLengthHeight, unitPixelLengthWidth, &top_left_PixelCoordinate, &rightVector, &upVector, &scene, primaryRay, image, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh, triangleNormalVectors, meshFaceNormalVectors, triangleMatrix, meshFaceMatrix, triangleSolutionMatrix, meshFaceSolutionMatrix);
        std::thread th2(pixelCalculator, heightDiv8, heightDiv8, width, unitPixelLengthHeight, unitPixelLengthWidth, &top_left_PixelCoordinate, &rightVector, &upVector, &scene, primaryRay, image, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh, triangleNormalVectors, meshFaceNormalVectors, triangleMatrix, meshFaceMatrix, triangleSolutionMatrix, meshFaceSolutionMatrix);
        std::thread th3(pixelCalculator, heightDiv8*2, heightDiv8, width, unitPixelLengthHeight, unitPixelLengthWidth, &top_left_PixelCoordinate, &rightVector, &upVector, &scene, primaryRay, image, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh, triangleNormalVectors, meshFaceNormalVectors, triangleMatrix, meshFaceMatrix, triangleSolutionMatrix, meshFaceSolutionMatrix);
        std::thread th4(pixelCalculator, heightDiv8*3, heightDiv8, width, unitPixelLengthHeight, unitPixelLengthWidth, &top_left_PixelCoordinate, &rightVector, &upVector, &scene, primaryRay, image, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh, triangleNormalVectors, meshFaceNormalVectors, triangleMatrix, meshFaceMatrix, triangleSolutionMatrix, meshFaceSolutionMatrix);
        std::thread th5(pixelCalculator, heightDiv8*4, heightDiv8, width, unitPixelLengthHeight, unitPixelLengthWidth, &top_left_PixelCoordinate, &rightVector, &upVector, &scene, primaryRay, image, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh, triangleNormalVectors, meshFaceNormalVectors, triangleMatrix, meshFaceMatrix, triangleSolutionMatrix, meshFaceSolutionMatrix);
        std::thread th6(pixelCalculator, heightDiv8*5, heightDiv8, width, unitPixelLengthHeight, unitPixelLengthWidth, &top_left_PixelCoordinate, &rightVector, &upVector, &scene, primaryRay, image, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh, triangleNormalVectors, meshFaceNormalVectors, triangleMatrix, meshFaceMatrix, triangleSolutionMatrix, meshFaceSolutionMatrix);
        std::thread th7(pixelCalculator, heightDiv8*6, heightDiv8, width, unitPixelLengthHeight, unitPixelLengthWidth, &top_left_PixelCoordinate, &rightVector, &upVector, &scene, primaryRay, image, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh, triangleNormalVectors, meshFaceNormalVectors, triangleMatrix, meshFaceMatrix, triangleSolutionMatrix, meshFaceSolutionMatrix);
        std::thread th8(pixelCalculator, heightDiv8*7, height - 7*heightDiv8, width, unitPixelLengthHeight, unitPixelLengthWidth, &top_left_PixelCoordinate, &rightVector, &upVector, &scene, primaryRay, image, numberOfTriangleIntersections, numberOfSphereIntersections, numberOfMeshes, numberOfFaceIntersectionPerMesh, triangleNormalVectors, meshFaceNormalVectors, triangleMatrix, meshFaceMatrix, triangleSolutionMatrix, meshFaceSolutionMatrix);

    

        th1.join();
        th2.join();
        th3.join();
        th4.join();
        th5.join();
        th6.join();
        th7.join();
        th8.join();


        int sizeOfName = camera[i].image_name.length();
        char imageName[sizeOfName + 1];

        strcpy(imageName, camera[i].image_name.c_str());

        write_ppm(imageName, image, width, height);

        delete[] image;

        //Heap free operations
        for(int i = 0; i < numberOfTriangleIntersections; i++){
            delete triangleNormalVectors[i];
            delete triangleMatrix[i];
            delete triangleSolutionMatrix[i];
        }

        for(int i = 0; i < numberOfMeshes; i++){
            int noOfFaces = numberOfFaceIntersectionPerMesh[i];
            for(int j = 0; j < noOfFaces; j++){
                delete meshFaceNormalVectors[i][j];
                delete[] meshFaceMatrix[i][j];
                delete[] meshFaceSolutionMatrix[i][j];
            }
            delete[] meshFaceNormalVectors[i];
            delete[] meshFaceMatrix[i];
            delete[] meshFaceSolutionMatrix[i];

            delete[] meshNormalMutex[i];
            delete[] meshMatrixMutex[i];
        }

        delete[] triangleNormalVectors;
        delete[] triangleMatrix;
        delete[] triangleSolutionMatrix;

        delete[] meshFaceNormalVectors;
        delete[] meshFaceMatrix;
        delete[] meshFaceSolutionMatrix;

        delete[] triangleMatrixMutex;
        delete[] triangleNormalMutex;

        delete[] meshMatrixMutex;
        delete[] meshNormalMutex;

        delete[] numberOfFaceIntersectionPerMesh;
    }

}
