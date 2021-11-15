#version 430 core

in vec3 colour;
in vec2 texCoord;
in vec3 normal;
out vec4 frag_colour;

uniform sampler2D texture0;

uniform vec3 ambientColour;
uniform float ambientStrength;

uniform vec3 dirColour;
uniform float dirStrength;
uniform vec3 lightDirection;

void main()
{
	vec4 ambient = vec4(ambientColour, 1.0f) * ambientStrength;
	float dirFactor = max(dot(normalize(normal),normalize(lightDirection)), 0.0f);
	vec4 directional = vec4(dirColour, 1.0f) * dirStrength * dirFactor;
	frag_colour = texture(texture0, texCoord) * vec4(colour, 1.0f) * (ambient+directional);
}