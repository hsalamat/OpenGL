#version 430 core

struct Diffuse
{
	vec4 light;
	vec4 material;
};

struct Ambient
{
	vec4 light;
	vec4 material;
};

struct Specular
{
	vec4 light;
	vec4 material;
	float shininess;
};

struct DirectionalLight 
{
	Diffuse base;
	vec3 direction;
};

struct PointLight
{
	Diffuse base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

uniform Ambient aLight;
uniform Specular sLight;
uniform DirectionalLight dLight;
uniform PointLight pLight;

in vec4 vColor;
out vec4 finalColor;
in vec3 N;
in vec3 E;
vec3 nL, nN, nE, L;

void main(void) {

nE = normalize(E);
nN = normalize(N);

nL = normalize(dLight.direction);

vec3 H = normalize (-nL + nE);

//Lambert's cosine law
float lambertTerm = dot(nN,-nL);

//Ambient Term
float Kd = max (dot (nN, -nL) , 0.0);

//Ambient Light
vec4 Ia = aLight.light * aLight.material;

//Diffuse Light
vec4 Id = vec4(0.0,0.0,0.0,1.0);

//Specular Light
vec4 Is = vec4(0.0,0.0,0.0,1.0);

if(lambertTerm > 0.0) //only if lambertTerm is positive
{

Id = dLight.base.light * dLight.base.material * lambertTerm;


float specularTerm = pow(max(dot(H, nN), 0.0), sLight.shininess );

Is = sLight.light * sLight.material * specularTerm; //add specular term
}

finalColor = Ia + Id + Is;
finalColor.a = 1.0;

//finalColor = vColor; //to test
}