#version 460 core

in vec4 myColor;
out vec4 fColor;

void main()
{
	//fColor = vec4(1.0, 0.0, 0.0, 0.0);
	fColor = myColor;
}