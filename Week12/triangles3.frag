#version 430 core

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
out vec4 frag_color;

struct Light
{
	vec3 diffusecolor;
	float diffuseStrength;
};

struct AmbientLight
{
	vec3 ambientcolor;
	float ambientStrength;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct Material
{
	float specularStrength;
	float shininess;
};

uniform sampler2D texture0;
uniform AmbientLight aLight;
uniform DirectionalLight dLight;
uniform Material mat;
uniform vec3 eyePosition;

void main()
{
	vec4 ambient = vec4(aLight.ambientcolor, 1.0f) * aLight.ambientStrength;
	float dirFactor = max(dot(normalize(normal),normalize(dLight.direction)), 0.0f);
	vec4 directional = vec4(dLight.base.diffusecolor, 1.0f) * dLight.base.diffuseStrength * dirFactor;

	vec4 specular = vec4(0,0,0,0);
	if (dirFactor > 0.0f && dLight.base.diffuseStrength > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(normalize(dLight.direction), normalize(normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, mat.shininess);
			specular = vec4(dLight.base.diffusecolor * mat.specularStrength * specularFactor, 1.0f);
		}
	}


	frag_color = texture(texture0, texCoord) * vec4(color, 1.0f) * (ambient+directional+specular);
}