#version 430 core

in vec3 color;
in vec2 texCoord;
out vec4 frag_color;

uniform sampler2D texture0;

uniform vec3 ambientColor;
uniform float ambientStrength;

void main()
{
	vec4 ambient = vec4(ambientColor, 1.0f) * ambientStrength;
	frag_color = texture(texture0, texCoord) * vec4(color, 1.0f) * ambient;
}