#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

out vec3 color;
uniform float time;

// Values that stay constant for the whole mesh.
uniform mat4 model;

void main()
{
	gl_Position = model * vec4(vertex_position, 1.0);
	gl_Position.x *= (1+sin(time));
	gl_Position.y *= (1+cos(time));
	color = vertex_color;
}