#version 430 core

in vec3 colour;
in vec2 texCoord;
in vec3 normal;
out vec4 frag_colour;

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

uniform sampler2D texture0;
uniform AmbientLight aLight;
uniform DirectionalLight dLight;

void main()
{
	vec4 ambient = vec4(aLight.ambientColour, 1.0f) * aLight.ambientStrength;
	float dirFactor = max(dot(normalize(normal),normalize(dLight.direction)), 0.0f);
	vec4 directional = vec4(dLight.base.diffuseColour, 1.0f) * dLight.base.diffuseStrength * dirFactor;
	frag_colour = texture(texture0, texCoord) * vec4(colour, 1.0f) * (ambient+directional);
}