#version 120

// Data from CPU 
//uniform mat4 MVP; // ModelViewProjection Matrix
//uniform mat4 MV; // ModelView idMVPMatrix
uniform mat4 modelingMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalVecModelingMatrix;

uniform vec4 cameraPosition;
uniform float heightFactor;

uniform vec3 cameraWorldPosition;
uniform vec3 lightWorldPosition;

// Texture-related data
uniform sampler2D rgbTextureSampler;
uniform sampler2D heightTextureSampler;

uniform float unitU;
uniform float unitV;
uniform int shiftOfTexture;

// Output to Fragment Shader
varying vec3 fragWorldNormal; // For Lighting computation
varying vec4 fragWorldPos;

varying vec3 ToLightVector; // Vector from Vertex to Light;
varying vec3 ToCameraVector; // Vector from Vertex to Camera;

varying vec2 textureCoordinate;

//varying vec2 textureCoordinate; // For texture-color
//varying vec3 ToLightVector; // Vector from Vertex to Light;
//varying vec3 ToCameraVector; // Vector from Vertex to Camera;

//Neighbour Points
vec3 point1 = vec3(0, 0, 0);
vec3 point2 = vec3(0, 0, 0);
vec3 point3 = vec3(0, 0, 0);
vec3 point4 = vec3(0, 0, 0);
vec3 point5 = vec3(0, 0, 0);
vec3 point6 = vec3(0, 0, 0);

vec2 texPoint1 = vec2(0, 0);
vec2 texPoint2 = vec2(0, 0);
vec2 texPoint3 = vec2(0, 0);
vec2 texPoint4 = vec2(0, 0);
vec2 texPoint5 = vec2(0, 0);
vec2 texPoint6 = vec2(0, 0);

//Neighbour Triangle Normals
vec3 norm1 = vec3(0, 0, 0);
vec3 norm2 = vec3(0, 0, 0);
vec3 norm3 = vec3(0, 0, 0);
vec3 norm4 = vec3(0, 0, 0);
vec3 norm5 = vec3(0, 0, 0);
vec3 norm6 = vec3(0, 0, 0);

//Height Values
vec4 heightVector;

vec4 heightVector1 = vec4(0, 0, 0, 0);
vec4 heightVector2 = vec4(0, 0, 0, 0);
vec4 heightVector3 = vec4(0, 0, 0, 0);
vec4 heightVector4 = vec4(0, 0, 0, 0);
vec4 heightVector5 = vec4(0, 0, 0, 0);
vec4 heightVector6 = vec4(0, 0, 0, 0);

//New gl_Vertex
vec4 new_gl_Vertex;

void main()
{
    // get texture coordinates
    //gl_TexCoord[0] = gl_MultiTexCoord0;
    textureCoordinate = vec2(gl_MultiTexCoord0);
    textureCoordinate.x = textureCoordinate.x + shiftOfTexture*unitU;

    // get height texture value, compute height
    heightVector = texture2D(heightTextureSampler, textureCoordinate);
    new_gl_Vertex = gl_Vertex + vec4(0, heightVector.x * heightFactor, 0, 0);

    // compute normal vector using also the heights of neighbor vertices
    point1 = vec3(gl_Vertex) + vec3(-1, 0, 0);
    point2 = vec3(gl_Vertex) + vec3(-1, 0, -1);
    point3 = vec3(gl_Vertex) + vec3(0, 0, -1);
    point4 = vec3(gl_Vertex) + vec3(1, 0, 0);
    point5 = vec3(gl_Vertex) + vec3(1, 0, 1);
    point6 = vec3(gl_Vertex) + vec3(0, 0, 1);

    float unitWidthChange = unitU;
    float unitHeightChange = unitV;

    texPoint1 = textureCoordinate + vec2(-1*unitWidthChange, 0);
    texPoint2 = textureCoordinate + vec2(-1*unitWidthChange, -1*unitHeightChange);
    texPoint3 = textureCoordinate + vec2(0, -1*unitHeightChange);
    texPoint4 = textureCoordinate + vec2(unitWidthChange, 0);
    texPoint5 = textureCoordinate + vec2(unitWidthChange, unitHeightChange);
    texPoint6 = textureCoordinate + vec2(0, unitHeightChange);
    
    //EDGE CASE leri de yap
    heightVector1 = texture2D(heightTextureSampler, texPoint1);
    point1 = point1 + vec3(0, heightVector1.x * heightFactor, 0);

    heightVector2 = texture2D(heightTextureSampler, texPoint2);
    point2 = point2 + vec3(0, heightVector2.x * heightFactor, 0);

    heightVector3 = texture2D(heightTextureSampler, texPoint3);
    point3 = point3 + vec3(0, heightVector3.x * heightFactor, 0);

    heightVector4 = texture2D(heightTextureSampler, texPoint4);
    point4 = point4 + vec3(0, heightVector4.x * heightFactor, 0);

    heightVector5 = texture2D(heightTextureSampler, texPoint5);
    point5 = point5 + vec3(0, heightVector5.x * heightFactor, 0);

    heightVector6 = texture2D(heightTextureSampler, texPoint6);
    point6 = point6 + vec3(0, heightVector6.x * heightFactor, 0);

    norm1 = normalize(cross(point2 - vec3(new_gl_Vertex) ,point1 - vec3(new_gl_Vertex)));
    norm2 = normalize(cross(point3 - vec3(new_gl_Vertex) ,point2 - vec3(new_gl_Vertex)));
    norm3 = normalize(cross(point4 - vec3(new_gl_Vertex) ,point3 - vec3(new_gl_Vertex)));
    norm4 = normalize(cross(point5 - vec3(new_gl_Vertex) ,point4 - vec3(new_gl_Vertex)));
    norm5 = normalize(cross(point6 - vec3(new_gl_Vertex) ,point5 - vec3(new_gl_Vertex)));
    norm6 = normalize(cross(point1 - vec3(new_gl_Vertex) ,point6 - vec3(new_gl_Vertex)));
    
    fragWorldNormal = normalVecModelingMatrix * normalize(norm1 + norm2 + norm3 + norm4 + norm5 + norm6);
    fragWorldPos = modelingMatrix * new_gl_Vertex;

    // compute toLight vector vertex coordinate in VCS
    ToLightVector = normalize(lightWorldPosition - vec3(modelingMatrix * new_gl_Vertex));
    ToCameraVector = normalize(cameraWorldPosition - vec3(modelingMatrix * new_gl_Vertex));

   // set gl_Position variable correctly to give the transformed vertex position
   gl_Position = projectionMatrix * viewMatrix * modelingMatrix * new_gl_Vertex;
    
}
