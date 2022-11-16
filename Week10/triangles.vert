#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

//step14
layout(location = 2) in vec2 vertex_texture;

out vec3 color;

//step15
out vec2 texCoord;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;

void main()
{
	color = vertex_color;

	//step16
	texCoord = vertex_texture;

	gl_Position = mvp * vec4(vertex_position, 1.0f);
}