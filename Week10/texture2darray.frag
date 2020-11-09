#version 430 core

in vec3 color;
in vec2 texCoord;
out vec4 frag_color;

uniform sampler2DArray texture0;
uniform int layer;

void main()
{
	frag_color = texture(texture0, vec3(texCoord,layer)) * vec4(color, 1.0f);
}