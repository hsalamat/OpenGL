
///////////////////////////////////////////////////////////////////////
//
// Sierpenski_2.cpp
// Load the vertices in initialization step
//Notice we moved glBufferData in the init() function when Pushing the coordinates of the vertices into the buffer
//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//why? becuase the coordinates are not changing!
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


GLuint Buffers[1];

const GLuint NumVertices = 30000;
const GLfloat scale = 0.5f;
GLfloat vertices[NumVertices][2] = { 0 };
std::array<glm::vec2, NumVertices> vertices1;

#define BUFFER_OFFSET(x)  ((const void*) (x))
static unsigned int
program,
vertexShaderId,
fragmentShaderId;


void init(void)
{

	// Create shader program executable.

	vertexShaderId = setShader((char*)"vertex", (char*)"cube.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cube.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);


	// Select an arbitrary initial point inside of the triangle
		vertices[0][0] = 0.25; 
		vertices[0][1] = 0.50;
		vertices1[0][0] = 0.25;
		vertices1[0][1] = 0.50;


	//using glm and array 
	std::array<glm::vec2, 3> points1 ={ glm::vec2(-1.0, -1.0), glm::vec2(0.0, 1.0), glm::vec2(1.0, -1.0) };

	//// compute and store N-1 new points
	for (int i = 1; i < NumVertices; ++i) {
		int j = (int)(rand() % 3);   // pick a vertex at random
		vertices1[i] = (vertices1[i - 1] + points1[j]) / 2.0f;
		
		vertices[i][0] = vertices1[i][0];
		vertices[i][1] = vertices1[i][1];
	}


	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//Notice that we are loading the coordinates here
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
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
	glDrawArrays(GL_POINTS, 0, NumVertices);

	glFlush();
}


void idle()
{
	//glutPostRedisplay();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	//Before we can open a window, theremust be interaction between the windowing systemand OpenGL.In GLUT, this interaction is initiated by the following function call :
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);

	//if you comment out this line, a window is created with a default size
	glutInitWindowSize(512, 512);

	//the top-left corner of the display
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Hello World");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	//If there are events in the queue, our program responds to them through functions
	//called callbacks.A callback function is associated with a specific type of event.
	//A display callback is generated when the application programm or the
	//operating system determines that the graphics in a window need to be redrawn.
	glutDisplayFunc(display);

	glutIdleFunc(idle);
	glutKeyboardFunc(keyInput);

	//begin an event-processing loop
	glutMainLoop();



}
