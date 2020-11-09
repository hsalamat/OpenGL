#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec3 vertex_texture;
layout(location = 3) in vec4 vertex_normal;

out vec3 color;
out vec3 texCoord;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform mat4 normalMatrix;
out vec3 R;

void main()
{
	color = vertex_color;
	//texCoord = vertex_texture;
	texCoord = vertex_position;
	gl_Position = mvp * vec4(vertex_position, 1.0f);
	//Transformed normal position
    vec3 N = normalize((normalMatrix * vertex_normal).xyz);
    R = reflect(normalize(pos), N);
    texCoord = R;
}