#version 430 core

in vec4 colour;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
out vec4 frag_colour;


struct AmbientLight
{
	vec3 ambientColour;
	float ambientStrength;
};



uniform sampler2D texture0;

uniform vec3 eyePosition;

uniform AmbientLight aLight;

void main()
{
	vec4 calcColour = vec4(0,0,0,0);
	
	vec4 ambient = vec4(aLight.ambientColour, 1.0f) * aLight.ambientStrength;
	calcColour += ambient;
		
	frag_colour = texture(texture0, texCoord) * vec4(colour) * calcColour;
}