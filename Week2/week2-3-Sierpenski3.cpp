
///////////////////////////////////////////////////////////////////////
//
// Sierpenski3.cpp
//
///////////////////////////////////////////////////////////////////////


using namespace std;

#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <array>
#include <string>


#define BUFFER_OFFSET(x)  ((const void*) (x))

GLuint Buffers[1];

const int Ndivisions = 3; // 5;
const int NumTetrahedrons = 64; //  1024;            // 4^5 tetrahedrons
const int NumTriangles = 4 * NumTetrahedrons;  // 4 triangles / tetrahedron
const int NumVertices = 3 * NumTriangles;      // 3 vertices / triangle
std::array<glm::vec2, NumVertices> vertices1;
GLfloat vertices[NumVertices][2] = { 0 };

glm::vec2 v[3];

void triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c)
/* specify one triangle */
{
	static int i = 0;
	vertices1[i] = a;
	i++;
	vertices1[i] = b;
	i++;
	vertices1[i] = c;
	i++;
}

void divide_triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, int k)
{
	if (k > 0)
	{
		// compute midpoints of sides
		glm::vec2 ab = (a + b) / 2.0f;
		glm::vec2 ac = (a + c) / 2.0f;
		glm::vec2 bc = (b + c) / 2.0f;
		// subdivide all but inner triangle
		divide_triangle(a, ab, ac, k - 1);
		divide_triangle(c, ac, bc, k - 1);
		divide_triangle(b, bc, ab, k - 1);
	}
	else triangle(a, b, c); /* draw triangle at end of recursion */
}

static unsigned int
program,
vertexShaderId,
fragmentShaderId;


void init(void)
{

	// Create shader program executable.

	vertexShaderId = setShader((char*)"vertex", (char*)"tetrahedron.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"tetrahedron.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);



	v[0] = glm::vec2(-1.0, -1.0);
	v[1] = glm::vec2(0.0, 1.0);
	v[2] = glm::vec2(1.0, -1.0);


	divide_triangle(v[0], v[1], v[2], Ndivisions);


	//// compute and store N-1 new points
	for (int i = 0; i < NumVertices; ++i) {


		vertices[i][0] = vertices1[i][0];
		vertices[i][1] = vertices1[i][1];
	}


	//GLuint vao;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vertex_position");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background

}


//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//glFlush();
	glutSwapBuffers();
}

void idle()
{
	//glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Sierpenski");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();



}


