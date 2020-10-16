#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

out vec3 color;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform vec3 theta;

vec3 angles = radians( theta );

// quaternion multiplier

vec4 multq(vec4 a, vec4 b)
{
    return(vec4(a.x*b.x - dot(a.yzw, b.yzw), a.x*b.yzw+b.x*a.yzw+cross(b.yzw, a.yzw)));
}

// inverse quaternion
vec4 invq(vec4 a)
{
    return(vec4(a.x, -a.yzw)/dot(a,a));
}


void main()
 {
    vec4 r;
    vec4 p;
    vec4 rx, ry, rz;
    vec3 c =  cos(angles/2.0);
    vec3 s =  sin(angles/2.0);
    rx = vec4(c.x, -s.x, 0.0, 0.0); // x rotation quaternion
    ry = vec4(c.y, 0.0, s.y, 0.0); // y rotation quaternion
    rz = vec4(c.z, 0.0, 0.0, s.z); // z rotation quaternion
    r = multq(rx, multq(ry, rz)); // rotation quaternion
    p = vec4(0.0, vertex_position);  // input point quaternion
    p = multq(r, multq(p, invq(r))); // rotated point quaternion
    vec4 vPos = vec4( p.yzw, 1.0); // convert back to homogeneous coordinates


	gl_Position = model * vPos;

	color = vertex_color;
}