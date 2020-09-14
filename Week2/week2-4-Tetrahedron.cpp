
///////////////////////////////////////////////////////////////////////
//
// Tetrahedron2.cpp
// we start with a tetrahedron and, find the midpoints of its six edges and connect these midpoints
//There are now four smaller tetrahedrons, one for each of the original vertices, and another area in the middle that we will discard
//we will use recursive subdivision to subdivide the four tetrahedrons that we keep. Because the faces of a
//tetrahedron are the four triangles determined by its four vertices, at the end of the
//subdivisions, we can render each of the final tetrahedrons by drawing four triangles.
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

const int Ndivisions = 5;
const int NumTetrahedrons = 1024;            // 4^5 tetrahedrons      
const int NumTriangles = 4 * NumTetrahedrons;  // 4 triangles / tetrahedron
const int NumVertices = 3 * NumTriangles;      // 3 vertices / triangle
std::array<glm::vec3, NumVertices> vertices1;
GLfloat vertices[NumVertices][3] = { 0 };

glm::vec3 v[4];
int  Index = 0;

void triangle(glm::vec3& a, glm::vec3& b, glm::vec3& c)
/* specify one triangle */
{
	vertices1[Index++] = a;
	vertices1[Index++] = b;
	vertices1[Index++] = c;
}

void tetra(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d)
{
	triangle(a, b, c);
	triangle(a, c, d);
	triangle(a, d, b);
	triangle(b, d, c);
}


void divide_tetra(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d, int count)
{
	if (count > 0) {
		glm::vec3 v0 = (a + b) / 2.0f;
		glm::vec3 v1 = (a + c) / 2.0f;
		glm::vec3 v2 = (a + d) / 2.0f;
		glm::vec3 v3 = (b + c) / 2.0f;
		glm::vec3 v4 = (c + d) / 2.0f;
		glm::vec3 v5 = (b + d) / 2.0f;
		divide_tetra(a, v0, v1, v2, count - 1);
		divide_tetra(v0, b, v3, v5, count - 1);
		divide_tetra(v1, v3, c, v4, count - 1);
		divide_tetra(v2, v4, v5, d, count - 1);
	}
	else {
		tetra(a, b, c, d);    // draw tetrahedron at end of recursion
	}
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


	v[0] = glm::vec3(0.0, 0.0, -1.0);
	v[1] = glm::vec3(0.0, 0.942809, 0.333333);
	v[2] = glm::vec3(-0.816497, -0.471405, 0.333333);
	v[3] = glm::vec3(0.816497, -0.471405, 0.333333);


	divide_tetra(v[0], v[1], v[2], v[3], Ndivisions);


	//// compute and store N-1 new points
	for (int i = 0; i < NumVertices; ++i) {


		vertices[i][0] = vertices1[i][0];
		vertices[i][1] = vertices1[i][1];
		vertices[i][2] = vertices1[i][2];
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
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background

}


//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glFlush();
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	//glutInitContextVersion(4, 6);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("tetrahedron2");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();



}


