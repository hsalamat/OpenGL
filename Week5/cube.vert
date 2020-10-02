#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

out vec3 color;

// Values that stay constant for the whole mesh.
uniform mat4 model;

void main()
{
	gl_Position = model * vec4(vertex_position, 1.0);
	//scale the cube by half by multiplying the position of each vertex by a scale factor (1/2)
	//gl_Position = model * vec4(vertex_position*0.5, 1.0);
	color = vertex_color;
}