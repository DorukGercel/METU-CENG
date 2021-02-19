#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"

static GLFWwindow* win = NULL;
static GLFWmonitor* monitorObj = NULL;
int widthWindow = 1000, heightWindow = 1000;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idHeightTexture;
GLuint idMVPMatrix;

// Buffers
GLuint idVertexBuffer;
GLuint idTextCoordBuffer;
GLuint idIndexBuffer;

int textureWidth, textureHeight;
float heightFactor;
int shiftOfTexture;

//Camera camera;
//Camera initialCamera;

/*glm::vec3 lightPos;
GLuint depthMapFBO;
GLuint depthCubemap;
bool lightPosFlag = false;*/

//GLM Matrices
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
glm::mat4 modelingMatrix;
glm::mat3 normalVecModelingMatrix;

glm::vec3 cameraPos;
glm::vec3 cameraGaze;
glm::vec3 cameraLeft;
glm::vec3 cameraUp;

glm::vec3 lightPos;

float speedOfPlane;

//Shader Uniform Variables Locations
GLint modelingMatrixLoc;
GLint viewMatrixLoc;
GLint projectionMatrixLoc;
GLint rgbTextureSamplerLoc;
GLint heightTextureSamplerLoc;
GLint heightFactorLoc;
GLint cameraWorldPositionLoc;
GLint lightWorldPositionLoc;
GLint normalVecModelingMatrixLoc;
GLint unitULoc;
GLint unitVLoc;
GLint shiftOfTextureLoc;

void enterFullscreen(){
  const GLFWvidmode* modeObj = glfwGetVideoMode(monitorObj);
  glfwSetWindowMonitor(win, monitorObj, 0, 0, modeObj->width, modeObj->height, modeObj->refreshRate);
}

void resetCamera(int width){
  //GLM View Matrix
  cameraPos = glm::vec3(width/2, width/10, -width/4);

  cameraGaze = glm::vec3(0, 0, 1);
	cameraGaze = cameraGaze / glm::length(cameraGaze);
  cameraLeft = glm::vec3(-1, 0, 0);
  cameraLeft = cameraLeft / glm::length(cameraLeft);
  cameraUp = glm::vec3(0, 1, 0);
  cameraUp = cameraUp/ glm::length(cameraUp);

	glm::vec3 center = cameraPos + cameraGaze;
  viewMatrix = glm::lookAt(cameraPos, center, glm::vec3(0, 1, 0));
  
  //Set speed of camera to 0
  speedOfPlane = 0;
}

void rotateGazeAroundUp(float angle){
  glm::vec3 gazePoint = cameraPos + cameraGaze;
  glm::mat4 transformationMatrix;
  cameraUp = cameraUp/glm::length(cameraUp);

  //Translate back
  transformationMatrix = glm::translate(glm::mat4(1.0), cameraPos);
  //Rotate around up
  transformationMatrix = glm::rotate(transformationMatrix, angle, cameraUp);
  //Translate to origin
  transformationMatrix = glm::translate(transformationMatrix, cameraPos*(-1.0f));
  //Transform point
  gazePoint = glm::vec3(transformationMatrix*glm::vec4(gazePoint,1));

  //Set camera vectors
  cameraGaze = gazePoint - cameraPos;
  cameraGaze = cameraGaze/glm::length(cameraGaze);

  cameraLeft = glm::cross(cameraUp, -cameraGaze);
  cameraLeft = cameraLeft/length(cameraLeft);

}

void rotateGazeAroundLeft(float angle){
  glm::vec3 gazePoint = cameraPos + cameraGaze;
  glm::mat4 transformationMatrix;
  cameraLeft = cameraLeft/glm::length(cameraLeft);

  //Translate back
  transformationMatrix = glm::translate(glm::mat4(1.0), cameraPos);
  //Rotate around up
  transformationMatrix = glm::rotate(transformationMatrix, angle, cameraLeft);
  //Translate to origin
  transformationMatrix = glm::translate(transformationMatrix, cameraPos*(-1.0f));
  //Transform point
  gazePoint = glm::vec3(transformationMatrix*glm::vec4(gazePoint,1));

  //Set camera vectors
  cameraGaze = gazePoint - cameraPos;
  cameraGaze = cameraGaze/glm::length(cameraGaze);

  cameraUp = glm::cross(cameraLeft, cameraGaze);
  cameraUp = cameraUp/length(cameraUp);

}

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    //Enter fullscreen
    if (key == GLFW_KEY_P && action == GLFW_PRESS){
        enterFullscreen();
    }
    //Increase height factor
    if(key == GLFW_KEY_R ){
      heightFactor += 0.5;
    }
    //Decrease height factor
    if(key == GLFW_KEY_F ){
      heightFactor -= 0.5;
    }

    //Shift texture
    //Left shift
    if(key == GLFW_KEY_Q ){
      shiftOfTexture = shiftOfTexture + 1;
    }
    //Right shift
    if(key == GLFW_KEY_E ){
      shiftOfTexture = shiftOfTexture - 1;
    }

    //Increase height of light source
    if(key == GLFW_KEY_T ){
      lightPos.y = lightPos.y + 5;
    }
    //Decrease height of light source
    if(key == GLFW_KEY_G ){
      lightPos.y = lightPos.y - 5;
    }
    
    //Light directions
    //Right
    if(key == GLFW_KEY_RIGHT ){
      lightPos.x = lightPos.x - 5;
    }
    //Left
    if(key == GLFW_KEY_LEFT ){
      lightPos.x = lightPos.x + 5;
    }
    //Up
    if(key == GLFW_KEY_UP ){
      lightPos.z = lightPos.z + 5;
    }
    //Down
    if(key == GLFW_KEY_DOWN ){
      lightPos.z = lightPos.z - 5;
    }

    //Camera speed options
    //Reset
    if(key == GLFW_KEY_I ){
      resetCamera(textureWidth);
    }
    //Increase speed
    if(key == GLFW_KEY_Y ){
      speedOfPlane += 0.01;
    }
    //Decrease speed
    if(key == GLFW_KEY_H ){
      speedOfPlane -= 0.01;
    }
    //Stop
    if(key == GLFW_KEY_X && action == GLFW_PRESS){
      speedOfPlane = 0;
    }

    //Camera rotate options
    //Move around up
    //Left
    if(key == GLFW_KEY_A ){
      float angleRadian = 0.05;
      rotateGazeAroundUp(angleRadian);
    }
    //Right
    if(key == GLFW_KEY_D ){
      float angleRadian = 0.05;
      rotateGazeAroundUp(-angleRadian);
    }
    //Up
    if(key == GLFW_KEY_W ){
      float angleRadian = 0.05;
      rotateGazeAroundLeft(angleRadian);
    }
    //Down
    if(key == GLFW_KEY_S ){
      float angleRadian = 0.05;
      rotateGazeAroundLeft(-angleRadian);
    }
}

void activateShaders(){
  std::string shaderVert = "shader.vert";
  std::string shaderFrag = "shader.frag";
  initShaders(idProgramShader, shaderVert, shaderFrag);
  
  //Set the locations of uniform variables
  modelingMatrixLoc = glGetUniformLocation(idProgramShader, "modelingMatrix");
  viewMatrixLoc = glGetUniformLocation(idProgramShader, "viewMatrix");
  projectionMatrixLoc = glGetUniformLocation(idProgramShader, "projectionMatrix");
  rgbTextureSamplerLoc = glGetUniformLocation(idProgramShader, "rgbTextureSampler");
  heightTextureSamplerLoc = glGetUniformLocation(idProgramShader, "heightTextureSampler");
  heightFactorLoc = glGetUniformLocation(idProgramShader, "heightFactor");
  cameraWorldPositionLoc = glGetUniformLocation(idProgramShader, "cameraWorldPosition");
  lightWorldPositionLoc = glGetUniformLocation(idProgramShader, "lightWorldPosition");
  normalVecModelingMatrixLoc = glGetUniformLocation(idProgramShader, "normalVecModelingMatrix");
  unitULoc = glGetUniformLocation(idProgramShader, "unitU");
  unitVLoc = glGetUniformLocation(idProgramShader, "unitV");
  shiftOfTextureLoc = glGetUniformLocation(idProgramShader, "shiftOfTexture");
  
  glUseProgram(idProgramShader);
}

void initProjection(){
  //GLM Projection Matrix
	projectionMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);
}

void initView(int width){
  //GLM View Matrix
  cameraPos = glm::vec3(width/2, width/10, -width/4); 

  cameraGaze = glm::vec3(0, 0, 1);
	cameraGaze = cameraGaze / glm::length(cameraGaze);
  cameraLeft = glm::vec3(-1, 0, 0);
  cameraLeft = cameraLeft / glm::length(cameraLeft);
  cameraUp = glm::vec3(0, 1, 0);
  cameraUp = cameraUp/ glm::length(cameraUp);

	glm::vec3 center = cameraPos + cameraGaze;
  viewMatrix = glm::lookAt(cameraPos, center, glm::vec3(0, 1, 0));
  
  //Set speed of camera to 0
  speedOfPlane = 0;
}

void initLight(int width, int height){
  lightPos = glm::vec3(width/2, 100, height/2);
}

void initConfig(){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  //glShadeModel(GL_SMOOTH);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  initProjection();

  //Height factor initial value is 10
  heightFactor = (float)10.0;
  //Shift of texture initial value is 0
  shiftOfTexture = 0;
}

void initMeshes(int width, int height){
  
  //Initialize Vertex Array
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  //Generate Buffers
  glGenBuffers(1, &idVertexBuffer);
  glGenBuffers(1, &idTextCoordBuffer);
  glGenBuffers(1, &idIndexBuffer);
  
  /*Allocate vertex coordinates and texture coordinates*/
  int noRowVertex = width + 1;
  int noColumnVertex = height + 1;
  int numberOfVertexCoordinates = noRowVertex * noColumnVertex * 3;
  int numberOfTextureCoordinates = noRowVertex * noColumnVertex * 2;

  GLfloat* vertexCoordinates = new GLfloat[numberOfVertexCoordinates];
  GLfloat* textureCoordinates = new GLfloat[numberOfTextureCoordinates];
  //Assign vertex coordinates and texture coordinates
  for(int i = 0; i < noColumnVertex; i++){
    for(int j = 0; j < noRowVertex; j++){
        vertexCoordinates[i*noRowVertex*3 + j*3] = j;
        vertexCoordinates[i*noRowVertex*3 + j*3 + 1] = 0;
        vertexCoordinates[i*noRowVertex*3 + j*3 + 2] = i;

        textureCoordinates[i*noRowVertex*2 + j*2] = (noRowVertex - j)/((float)noRowVertex);
        textureCoordinates[i*noRowVertex*2 + j*2 + 1] = (noColumnVertex - i)/((float)noColumnVertex);
    }
  }

  /*Allocate vertex indices*/
  int numberOfIndices = 2 * width * height * 3;
  GLuint* vertexIndices = new GLuint[numberOfIndices];
  //Assign vertex indices
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      //Upper triangle
      vertexIndices[i*width*6 + j*6] = i*(width + 1) + j;
      vertexIndices[i*width*6 + j*6 + 1] = (i+1)*(width + 1) + (j+1);
      vertexIndices[i*width*6 + j*6 + 2] = (i+1)*(width + 1) + j;

      //Lower triangle
      vertexIndices[i*width*6 + j*6 + 3] = i*(width + 1) + j;
      vertexIndices[i*width*6 + j*6 + 4] = i*(width + 1) + (j+1);
      vertexIndices[i*width*6 + j*6 + 5] = (i+1)*(width + 1) + (j+1);
    }
  }

  //Buffer data
  glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);  
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numberOfVertexCoordinates, vertexCoordinates, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, idTextCoordBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numberOfTextureCoordinates, textureCoordinates, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numberOfIndices, vertexIndices, GL_STATIC_DRAW);

  //Free heap area
  delete[] vertexCoordinates;
  delete[] textureCoordinates;
  delete[] vertexIndices;
}

void drawEarth(){
  //glColor3f(1.0, 1.0, 0.0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
  glVertexPointer(3, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, idTextCoordBuffer);
  glTexCoordPointer(2, GL_FLOAT, 0, 0);

  int numberOfIndices = 2*textureWidth*textureHeight*3;
  glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);
}


void render(){
  //Clear
  glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
  glClearStencil(0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  //Compute view matrix
  cameraGaze = cameraGaze / glm::length(cameraGaze);
  cameraPos = cameraPos + cameraGaze * speedOfPlane;
	glm::vec3 center = cameraPos + cameraGaze;
  viewMatrix = glm::lookAt(cameraPos, center, glm::vec3(0, 1, 0));
  
  //Compute model matrix
  modelingMatrix = glm::mat4(1.0);
  normalVecModelingMatrix = glm::inverse(glm::transpose(glm::mat3(modelingMatrix))) ;
  
  //Set the values of its uniform variables
  glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(modelingMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelingMatrix));
  glUniformMatrix3fv(normalVecModelingMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalVecModelingMatrix));
  glUniform1i(rgbTextureSamplerLoc, 0);
  glUniform1i(heightTextureSamplerLoc, 1);
  glUniform1f(heightFactorLoc, heightFactor);
  glUniform1f(unitULoc, (1.0/textureWidth));
  glUniform1f(unitVLoc, (1.0/textureHeight));
  glUniform1i(shiftOfTextureLoc, shiftOfTexture);
  glUniform3fv(cameraWorldPositionLoc, 1, glm::value_ptr(cameraPos));
  glUniform3fv(lightWorldPositionLoc, 1, glm::value_ptr(lightPos));

  //Makes a draw call
  drawEarth();
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Please provide height and texture image files!\n");
    exit(-1);
  }

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) {
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This is required for remote
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This might be used for local

  win = glfwCreateWindow(widthWindow, heightWindow, "CENG477 - HW4", NULL, NULL);
  monitorObj = glfwGetPrimaryMonitor();

  if (!win) {
      glfwTerminate();
      exit(-1);
  }
  glfwMakeContextCurrent(win);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

      glfwTerminate();
      exit(-1);
  }

  //Initiliaze shaders
  activateShaders();

  glfwSetKeyCallback(win, keyCallback);

  //Make inits
  initTexture(argv[1], argv[2], &textureWidth, &textureHeight);
  //My functions
  initConfig();
  initMeshes(textureWidth, textureHeight);
  initView(textureWidth);
  initLight(textureWidth, textureHeight);

  while(!glfwWindowShouldClose(win)) {
    /**********************************************/
    //Change the window size
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);
    glViewport(0, 0, width, height);
    /**********************************************/
    //Render
    render();
    /***********************************************/
    glfwSwapBuffers(win);
    glfwPollEvents();
    /***********************************************/
  }

  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}