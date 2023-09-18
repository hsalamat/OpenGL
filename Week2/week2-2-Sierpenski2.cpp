//we convert our two - dimensional Sierpinski gasket program to a program that will
//generate a three - dimensional gasket; that is, one that is not restricted to a plane.

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

const GLuint NumVertices = 90000;
const GLfloat scale = 0.5f;
GLfloat vertices[NumVertices][3] = { 0 };
std::array<glm::vec3, NumVertices> vertices1;


static unsigned int
program,
vertexShaderId,
fragmentShaderId;


void init(void)
{

	// Create shader program executable.

	vertexShaderId = setShader((char*)"vertex", (char*)"triangles.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"triangles.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);


	// arbitrary initial location inside tetrahedron
	vertices1[0][0] = 0.0;
	vertices1[0][1] = 0.0;
	vertices1[0][2] = 0.0;


	// vertices of an arbitrary tetrahedron
	std::array<glm::vec3, 4> points1 = { glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0,  0.5,  0.0), glm::vec3(0.0, -0.5,  0.5) };
	//std::array<glm::vec3, 4> points1 = { glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 0.942809, 0.333333), glm::vec3(-0.816497, -0.471405, 0.333333), glm::vec3(0.816497, -0.471405, 0.333333) };


	//// compute and store N-1 new points
	for (int i = 1; i < NumVertices; ++i) {
		int j = (int)(rand() % 4);   // pick a vertex at random
		vertices1[i] = (vertices1[i - 1] + points1[j]) / 2.0f;

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
	glDrawArrays(GL_POINTS, 0, NumVertices);

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
	glutCreateWindow("Sierpenski2");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();



}