#include <iostream>
#include <algorithm>    // std::reverse_copy
#include <string.h>
#include <chrono>       //std::chrono
#include <sstream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utility.h"
//////-------- Global Variables -------/////////

GLuint vertexCoordinateBuffer;
GLuint vertexNormalBuffer;
GLuint* vertexIndexBuffer;
int* indicesPerMesh;

// Sample usage for reading an XML scene file
parser::Scene scene;
static GLFWwindow* win = NULL;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void transformCameraCoordinates(){
    parser::Camera* currentCamera = &(scene.camera);
    parser::Vec3f center;
    center.x = currentCamera->position.x + currentCamera->gaze.x*currentCamera->near_distance;
    center.y = currentCamera->position.y + currentCamera->gaze.y*currentCamera->near_distance;
    center.z = currentCamera->position.z + currentCamera->gaze.z*currentCamera->near_distance;

    /* Set camera position */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(currentCamera->position.x, currentCamera->position.y, currentCamera->position.z,
              center.x, center.y, center.z,
              currentCamera->up.x, currentCamera->up.y, currentCamera->up.z);
    /*gluLookAt(0, 0.25, 0.1,
    0, 0.25, 0,
    0, 1, 0);*/
}

void initializeMeshes(){
    /*Enable client side arrays*/
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

    /*Generate buffers for vertex attributes*/
    glGenBuffers(1, &vertexCoordinateBuffer);
    glGenBuffers(1, &vertexNormalBuffer);

    /*Generate buffers for indices of each mesh*/
    int numberOfMeshes = scene.meshes.size();
    vertexIndexBuffer = new GLuint[numberOfMeshes];
    indicesPerMesh = new int[numberOfMeshes];

    for(int i=0; i<numberOfMeshes; i++){
        glGenBuffers(1, vertexIndexBuffer+i);
        //std::cout << vertexIndexBuffer[i] << std::endl;
    }

    /*Allocate vertex coordinates*/
    int numberOfVertexCoordinates = scene.vertex_data.size() * 3;
    GLfloat vertexCoordinates[numberOfVertexCoordinates];
    int vertexIndex = 0;
    for(auto& vertex:scene.vertex_data){
        vertexCoordinates[vertexIndex++] = vertex.x;
        vertexCoordinates[vertexIndex++] = vertex.y;
        vertexCoordinates[vertexIndex++] = vertex.z;
    }
    //Buffer the vertex coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vertexCoordinateBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoordinates), vertexCoordinates, GL_STATIC_DRAW);

    /*Allocate sum of vertex normals*/
    int numberOfVertex = scene.vertex_data.size();
    parser::Vec3f vertexSumOfNormals[numberOfVertex];

    for(int i=0; i < numberOfVertex; i++){
        vertexSumOfNormals[i] = parser::Vec3f(0,0,0);
    }

    /*Allocate vertex indices*/
    //Allocate space to store indices of each mesh
    int meshIndex = 0;
    for(auto& mesh:scene.meshes){
        //Allocate space to store indices of current mesh
        int numberOfIndicesPerMesh = mesh.faces.size() * 3;
        GLuint indicesOfMesh[numberOfIndicesPerMesh];
        //Store the indices of current mesh
        int indiceIndex = 0;
        for(auto& face:mesh.faces){
            //Convert face ids to index
            int index0 = face.v0_id - 1;
            int index1 = face.v1_id - 1;
            int index2 = face.v2_id - 1;
            //Store the indices of the meshes
            indicesOfMesh[indiceIndex++] = index0;
            indicesOfMesh[indiceIndex++] = index1;
            indicesOfMesh[indiceIndex++] = index2;
            
            //Store the sum of normal vetors and count of normal vetors
            parser::Vec3f vertex0(vertexCoordinates[index0*3], 
                                    vertexCoordinates[index0*3 + 1],
                                    vertexCoordinates[index0*3 + 2]);

            parser::Vec3f vertex1(vertexCoordinates[index1*3], 
                                    vertexCoordinates[index1*3 + 1],
                                    vertexCoordinates[index1*3 + 2]);

            parser::Vec3f vertex2(vertexCoordinates[index2*3], 
                                    vertexCoordinates[index2*3 + 1],
                                    vertexCoordinates[index2*3 + 2]);

            parser::Vec3f normalVector = normalVectorCalculator(vertex0, vertex1, vertex2);
            vertexSumOfNormals[index0] = vertexSumOfNormals[index0] + normalVector;
            vertexSumOfNormals[index1] = vertexSumOfNormals[index1] + normalVector;
            vertexSumOfNormals[index2] = vertexSumOfNormals[index2] + normalVector;
        }
        //Buffer the indices of the current mesh, to the mesh's buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer[meshIndex]);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesOfMesh), indicesOfMesh, GL_STATIC_DRAW);

        //Set the number of indices of the current mesh
        indicesPerMesh[meshIndex] = mesh.faces.size() * 3;
        //std::cout << indicesPerMesh[meshIndex] << std::endl;

        meshIndex++;
    }

    /*Allocate vertex normals*/
    int numberOfVertexNormalDirections = scene.vertex_data.size() * 3;
    GLfloat vertexNormals[numberOfVertexNormalDirections];
    for(int i=0; i<numberOfVertex;i++){
        parser::Vec3f normalizedNormalVector = normalize(vertexSumOfNormals[i]);
        vertexNormals[i*3] = normalizedNormalVector.x;
        vertexNormals[i*3 + 1] = normalizedNormalVector.y;
        vertexNormals[i*3 + 2] = normalizedNormalVector.z; 
    }
    //Buffer the vertex normals
    glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNormals), vertexNormals, GL_STATIC_DRAW);
}

void drawMesh(int currentMeshIndex){
    //Bind the buffers and vertex arrays
    glBindBuffer(GL_ARRAY_BUFFER, vertexCoordinateBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer[currentMeshIndex]);
    //std::cout << vertexIndexBuffer[currentMeshIndex] << std::endl;
    glDrawElements(GL_TRIANGLES, indicesPerMesh[currentMeshIndex], GL_UNSIGNED_INT, 0);
}

void render(){
    glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
    glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    int meshIndex = 0;
    for(auto& mesh:scene.meshes){
        if(mesh.mesh_type.compare("Solid") == 0){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else if(mesh.mesh_type.compare("Wireframe") == 0){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        transformCameraCoordinates();
    
        std::vector<parser::Transformation> realOrderTransformations;
        realOrderTransformations.resize(mesh.transformations.size());
        std::reverse_copy(mesh.transformations.begin(), mesh.transformations.end(), realOrderTransformations.begin());

        for(auto& transformation:realOrderTransformations){
            int vectorID;
            if(transformation.transformation_type.compare("Translation") == 0){
                vectorID = transformation.id - 1;
                parser::Vec3f translate = scene.translations[vectorID];
                glTranslatef(translate.x, translate.y, translate.z);
            }
            else if(transformation.transformation_type.compare("Rotation") == 0){
                vectorID = transformation.id - 1;
                parser::Vec4f rotation = scene.rotations[vectorID];
                glRotatef(rotation.x, rotation.y, rotation.z, rotation.w);
            }
            else if(transformation.transformation_type.compare("Scaling") == 0){
                vectorID = transformation.id - 1;
                parser::Vec3f scaling = scene.scalings[vectorID];
                glScalef(scaling.x, scaling.y, scaling.z);
            }
        }
        int currentMaterialID = (mesh.material_id-1);
        parser::Vec3f currentAmbient = scene.materials[currentMaterialID].ambient;
        parser::Vec3f currentDiffuse = scene.materials[currentMaterialID].diffuse;
        parser::Vec3f currentSpecular = scene.materials[currentMaterialID].specular;
        
        GLfloat ambient[] = {currentAmbient.x, currentAmbient.y, currentAmbient.z, 1.0};
        GLfloat diffuse[] = {currentDiffuse.x, currentDiffuse.y, currentDiffuse.z, 1.0};
        GLfloat specular[] = {currentSpecular.x, currentSpecular.y, currentSpecular.z, 1.0};
        GLfloat specExp[] = {scene.materials[currentMaterialID].phong_exponent};

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, specExp);

        //glColor3f(1, 0, 0);
        drawMesh(meshIndex);
        meshIndex++;
    }
}

void setCamera(int width, int height){
    glViewport(0, 0, width, height);

    parser::Camera* currentCamera = &(scene.camera);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    transformCameraCoordinates();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(currentCamera->near_plane.x, currentCamera->near_plane.y, currentCamera->near_plane.z, currentCamera->near_plane.w,
              currentCamera->near_distance, currentCamera->far_distance);

}

void turnOnLights(){
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    
    GLfloat ambient[] = {scene.ambient_light.x, scene.ambient_light.y, scene.ambient_light.z};
    GLfloat black[] = {0.0f, 0.0f, 0.0f};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
    glLightfv(GL_LIGHT0, GL_SPECULAR, black);

    int numberOfPointLights = scene.point_lights.size();
    for(int i=0; i < numberOfPointLights; i++){
        
        GLfloat color[] = {scene.point_lights[i].intensity.x, scene.point_lights[i].intensity.y, scene.point_lights[i].intensity.z, 1.0f};
        GLfloat position[] = {scene.point_lights[i].position.x, scene.point_lights[i].position.y, scene.point_lights[i].position.z, 1.0f};

        glEnable(GL_LIGHT0 + (i+1));
        glLightfv(GL_LIGHT0 + (i+1), GL_POSITION, position);
        glLightfv(GL_LIGHT0 + (i+1), GL_DIFFUSE, color);
        glLightfv(GL_LIGHT0 + (i+1), GL_SPECULAR, color);
        glLightfv(GL_LIGHT0 + (i+1), GL_AMBIENT, black);
    }
}

void initialize(){
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    if(scene.culling_enabled == 1){
        glEnable(GL_CULL_FACE);  
        if(scene.culling_face == 1){
            glCullFace(GL_FRONT);
        }
        else{
            glCullFace(GL_BACK);
        }
    }
    else{
        glDisable(GL_CULL_FACE);
    }

    turnOnLights();
    setCamera(scene.camera.image_width, scene.camera.image_height);
}

int main(int argc, char* argv[]) {
    scene.loadFromXml(argv[1]);

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    win = glfwCreateWindow(scene.camera.image_width, scene.camera.image_height, "CENG477 - HW3", NULL, NULL);
    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(win, keyCallback);

    //Initialization scene and buffers
    initialize();
    initializeMeshes();

    //Necessary variables for window title naming
    std::chrono::time_point<std::chrono::system_clock> startTime;
    std::chrono::time_point<std::chrono::system_clock> endTime;
    char windowTitle[512] = {0};
    int framesRendered = 0;

    while(!glfwWindowShouldClose(win)) {
        glfwWaitEvents();
        /**********************************************/
        //Change the window size
        int width, height;
        glfwGetFramebufferSize(win, &width, &height);
        glViewport(0, 0, width, height);
        /**********************************************/
        //Start the timer
        if(framesRendered == 0){
            startTime = std::chrono::system_clock::now();
        }
        /**********************************************/
        //Render
        render();
        /**********************************************/
        framesRendered+=1;
        endTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsedTime = endTime - startTime;

        if(elapsedTime.count() > 1.0){
            double fps = framesRendered/elapsedTime.count();

            std::stringstream fpsStream;
            fpsStream << fps;

            strcpy(windowTitle, "CENG477 - HW3");
            strcat(windowTitle, " [");
            strcat(windowTitle, fpsStream.str().c_str());
            strcat(windowTitle, " FPS]");

            glfwSetWindowTitle(win, windowTitle);

            framesRendered = 0;
        }
        /**********************************************/
        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}