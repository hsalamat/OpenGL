#version 430 core

in vec4 vColor;
out vec4 finalColor;

uniform vec4 lightDiffuse;
uniform vec4 materialDiffuse;
uniform vec4 lightAmbient;
uniform vec4 materialAmbient;
uniform vec4 lightSpecular;
uniform vec4 materialSpecular;
uniform float shininess;
uniform vec4 lightPosition;

in vec3 N;
in vec3 E;
vec3 nL, nN, nE, L;

void main(void) {

nE = normalize(E);
nN = normalize(N);

 // check for directional light

 if(lightPosition.w == 0.0) L = normalize(lightPosition.xyz);
 else L = lightPosition.xyz + E;

nL = normalize(L);

vec3 H = normalize (nL + nE);

//Lambert's cosine law
float lambertTerm = dot(nN,nL);

float Kd = max (dot (nN, nL) , 0.0);
//Ambient Term
vec4 Ia = lightAmbient * materialAmbient;

//Diffuse Term
vec4 Id = vec4(0.0,0.0,0.0,1.0);

//Specular Term
vec4 Is = vec4(0.0,0.0,0.0,1.0);

if(lambertTerm > 0.0) //only if lambertTerm is positive
{
Id = lightDiffuse* materialDiffuse * lambertTerm; //add diffuse term

//vec3 R = reflect(nL, nN);
//float specular = pow(max(dot(R, nE), 0.0), shininess );
float specular = pow(max(dot(H, nN), 0.0),  max(shininess,2.0) );

Is = lightSpecular * materialSpecular * specular; //add specular term
}

finalColor = Ia + Id + Is;
finalColor.a = 1.0;
}