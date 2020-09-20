#version 430 core

layout(location = 0) in vec3 vertex_position;

out vec3 color;


void main()
{
	gl_Position = vec4(vertex_position, 1.0);
	//color = vertex_color;
	//makes the color of each point depend on that point’s location
	color = vec3((1.0 + vertex_position.xyz)/2.0);
}
