#version 120

//uniform mat4 MVP; // ModelViewProjection Matrix
//uniform mat4 MV; // ModelView idMVPMatrix
uniform vec3 cameraWorldPosition;
uniform vec3 lightWorldPosition;

// Texture-related data;
uniform sampler2D rgbTextureSampler;
uniform sampler2D heightTextureSampler;

// Data from Vertex Shader
varying vec3 fragWorldNormal; // For Lighting computation
varying vec4 fragWorldPos;

//varying vec2 textureCoordinate;
varying vec3 ToLightVector; // Vector from Vertex to Light;
varying vec3 ToCameraVector; // Vector from Vertex to Camera;

varying vec2 textureCoordinate;

void main() {

  // Assignment Constants below
  // get the texture color
  vec4 textureColor = texture2D(rgbTextureSampler, textureCoordinate);

  // apply Phong shading by using the following parameters
  vec4 ka = vec4(0.25,0.25,0.25,1.0); // reflectance coeff. for ambient
  vec4 Ia = vec4(0.3,0.3,0.3,1.0); // light color for ambient
  vec4 Id = vec4(1.0, 1.0, 1.0, 1.0); // light color for diffuse
  vec4 kd = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for diffuse
  vec4 Is = vec4(1.0, 1.0, 1.0, 1.0); // light color for specular
  vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for specular
  int specExp = 100; // specular exponent

  vec3 halfVector = normalize(ToLightVector + ToCameraVector);
  vec3 normalVector = normalize(fragWorldNormal);

  float diffuseComp = dot(normalVector, ToLightVector);
  float specularComp = dot(normalVector, halfVector);

  // compute ambient component
  vec4 ambient = Ia * ka;
  // compute diffuse component
  vec4 diffuse = Id * kd * max(0, diffuseComp);
  // compute specular component
  vec4 specular = Is * ks * pow(max(0, specularComp), specExp);

  // compute the color using the following equation
  gl_FragColor = vec4(clamp( textureColor.xyz * vec3(ambient + diffuse + specular), 0.0, 1.0), 1.0);
}
