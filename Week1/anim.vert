#version 460 core
layout(location = 0) in vec4 vPosition;

uniform float gScale;
out vec4 myColor;

void main()
{
	
	gl_Position = vec4(gScale*vPosition.x, gScale*vPosition.y, gScale*vPosition.z, 1.0);

}