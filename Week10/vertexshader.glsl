#version 430 core

#define FIELD 0
#define SKY 1

layout(location = 0) in vec3 field_position;
layout(location = 1) in vec3 field_color;
layout(location = 2) in vec2 field_texture;

layout(location = 3) in vec3 sky_position;
layout(location = 4) in vec3 sky_color;
layout(location = 5) in vec2 sky_texture;

vec3 vertex_position;
vec3 vertex_color;
vec2 vertex_texture;

out vec3 color;
out vec2 texCoord;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform uint object;
uniform float angle;

void main()
{

    if (object == FIELD)
    {
        vertex_position = field_position;
        vertex_color = field_color;
        vertex_texture = field_texture;
    }
    if (object == SKY)
    {
        vertex_position = sky_position;
        vertex_color = sky_color;
        vertex_texture = sky_texture;
    }

	color = vertex_color;
	texCoord = vertex_texture + vec2(0.1*cos(angle),0.1*sin(angle));
	gl_Position = mvp * vec4(vertex_position, 1.0f);
}