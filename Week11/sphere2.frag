#version 430 core

in vec4 normalAttr;
in vec3 N, L;
uniform vec4 lightDiffuse;
uniform vec4 materialDiffuse;
out vec4 frag_color;

void main()
{
 //Lambert's cosine law
 float lambertTerm = dot(N, L);
 float Kd = max (dot (N, L) , 0.0);
 
 //Final Color
 vec4 vColor = materialDiffuse * lightDiffuse * Kd;
 vColor.a = 1.0;
 frag_color = vColor;
}