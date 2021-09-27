#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

out vec3 color;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform vec3 theta;

void main()
{

   // Compute the sines and cosines of theta for each of
        //   the three axes in one computation.
        vec3 angles = radians( theta );
        vec3 c = cos( angles );
        vec3 s = sin( angles );

        // Remeber: these matrices are column-major
        mat4 rx = mat4( 
        1.0,  0.0,  0.0, 0.0,
        0.0,  c.x,  s.x, 0.0,
        0.0, -s.x,  c.x, 0.0,
        0.0,  0.0,  0.0, 1.0 );

        mat4 ry = mat4( 
        c.y, 0.0, -s.y, 0.0,
        0.0, 1.0,  0.0, 0.0,
        s.y, 0.0,  c.y, 0.0,
        0.0, 0.0,  0.0, 1.0 );


        mat4 rz = mat4( 
        c.z,  s.z, 0.0, 0.0,
        -s.z,  c.z, 0.0, 0.0,
        0.0,  0.0, 1.0, 0.0,
        0.0,  0.0, 0.0, 1.0 );

        vec4 vPos = rz * ry * rx * vec4(vertex_position, 1.0);

	   gl_Position = model * vPos;

	color = vertex_color;
}