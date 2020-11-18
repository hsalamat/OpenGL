#version 430 core

layout(location = 0) in vec3 vPositionAttr;
layout(location = 1) in vec3 vColorAttr;
layout(location = 2) in vec4 vNormalAttr;

out vec4 vColor;

// Values that stay constant for the whole mesh.
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 lightDiffuse;
uniform vec4 materialDiffuse;
uniform vec4 lightAmbient;
uniform vec4 materialAmbient;
uniform vec4 lightSpecular;
uniform vec4 materialSpecular;
uniform vec4 lightPosition;
uniform mat4 normalMatrix;
uniform float shininess;

void main()
{
 // pos is vertex position in eye coordinates
  vec3 pos = (modelViewMatrix * vec4(vPositionAttr,1.0)).xyz;

  //Transformed normal position
  vec3 N = normalize(vec3(normalMatrix * vNormalAttr));

  //Normalize light to calculate lambertTerm
  vec3 L;

 // check for directional light

 if(lightPosition.w == 0.0) L = normalize(lightPosition.xyz);
 else L = normalize( lightPosition.xyz - pos );

 //Lambert's cosine law
 float lambertTerm = dot(N,-L);

 float Kd = max (dot (N, -L) , 0.0);

 //Ambient Term
 vec4 Ia = lightAmbient * materialAmbient;

 //Diffuse Term
 vec4 Id = vec4(0.0,0.0,0.0,1.0);

 //Specular Term
 vec4 Is = vec4(0.0,0.0,0.0,1.0);

 if(lambertTerm > 0.0) //only if lambertTerm is positive
 {
 Id = lightDiffuse* materialDiffuse * lambertTerm; //add diffuse term

 vec3 eyeVec = -vec3(pos);
 vec3 E = normalize(eyeVec);
 vec3 R = reflect(L, N);
 float specular = pow(max(dot(R, E), 0.0), shininess );

 Is = lightSpecular * materialSpecular * specular; //add specular term
 }


 //Final color
 vColor = Ia + Id + Is;
 vColor.a = 1.0;


  gl_Position = projectionMatrix * modelViewMatrix * vec4(vPositionAttr,1.0f);
}