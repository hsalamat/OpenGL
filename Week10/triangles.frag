#version 430 core

in vec3 color;

//step17
in vec2 texCoord;


out vec4 frag_color;

//step18
uniform sampler2D texture0;

void main()
{
    //step19
	frag_color = texture(texture0, texCoord) * vec4(color, 1.0f);
}