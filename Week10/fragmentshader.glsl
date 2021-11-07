#version 430 core

#define FIELD 0
#define SKY 1

in vec3 color;
in vec2 texCoord;
out vec4 frag_color;

uniform sampler2D grassTex;
uniform sampler2D skyTex;
uniform uint object;

vec4 fieldTexColor, skyTexColor;

void main()
{

	fieldTexColor = texture(grassTex, texCoord) * vec4(color, 1.0f);
	skyTexColor = texture(skyTex, texCoord) * vec4(color, 1.0f);
	if (object == FIELD) frag_color = fieldTexColor;
	if (object == SKY) frag_color = skyTexColor;
	//To test
	//frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}