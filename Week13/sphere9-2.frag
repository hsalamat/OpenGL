#version 430 core


struct Light
{
	vec3 diffuseColour;
	float diffuseStrength;
};

struct AmbientLight
{
	vec3 ambientColour;
	float ambientStrength;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	//float constant;
	//float linear;
	//float exponent;
	float range;
};


struct Material
{
	float specularStrength;
	float shininess;
};


uniform AmbientLight aLight;
uniform Material mat;
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

nL = normalize(pLight.position + E);

vec3 H = normalize (-nL + nE);

//Lambert's cosine law
float lambertTerm = dot(nN,-nL);

//Ambient Term
float Kd = max (dot (nN, -nL) , 0.0);

//Ambient Light
vec4 Ia = vec4(aLight.ambientColour,1.0) * aLight.ambientStrength;

//Diffuse Light
vec4 Id = vec4(0.0,0.0,0.0,1.0);

//Specular Light
vec4 Is = vec4(0.0,0.0,0.0,1.0);

if(lambertTerm > 0.0) //only if lambertTerm is positive
{

Id = vec4(pLight.base.diffuseColour * pLight.base.diffuseStrength * lambertTerm, 1.0);

float specularTerm = pow(max(dot(H, nN), 0.0), mat.shininess );

Is = vec4(pLight.base.diffuseColour * mat.specularStrength * specularTerm, 1.0);

}

float distance = length(pLight.position + E);

//float attenuation = 1 / (pLight.exponent * distance * distance +
//					pLight.linear * distance +
//					pLight.constant);

//𝑨𝒕𝒕(𝒅) = 𝒔𝒂𝒕𝒖𝒓𝒂𝒕𝒆((𝒇𝒂𝒍𝒍𝒐𝒇𝒇𝑬𝒏𝒅−𝒅)/(𝒇𝒂𝒍𝒍𝒐𝒇𝒇𝑬𝒏𝒅 − 𝒇𝒂𝒍𝒍𝒐𝒇𝒇𝑺𝒕𝒂𝒓𝒕))
float attenuation = (2.0 - distance)/(2.0 - 0.0);
attenuation = clamp(attenuation, 0.0, 1.0);


finalColor = (Ia + Id + Is) * attenuation ;
finalColor.a = 1.0;

//finalColor = vColor; //to test
}