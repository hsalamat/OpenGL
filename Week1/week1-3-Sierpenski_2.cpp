
///////////////////////////////////////////////////////////////////////
//
// Sierpenski_2.cpp
// Load the vertices in initialization step
// 
// In previous algorithm, we compute all the points first and put them into an array or
//some other data structure.We then display all the points through a single function
//call.This approach avoids the overhead of sending small amounts of data to the
//graphics processor for each point we generate at the cost of having to store all the
//data.The strategy used in the first algorithm is known as immediatemode graphics
//and, until recently, was the standard method for displaying graphics, especially where
//interactive performance was needed.One consequence of immediate mode is that
//there is no memory of the geometric data.With our first example, if we want to
//display the points again, we would have to go through the entire creation and display
//process a second time.
// 
// In the following algorithm, because the data are stored in a data structure, we can
//redisplay the data, perhaps with some changes such as altering the color or changing
//the size of a displayed point, by resending the array without regenerating the points.
//The method of operation is known as retained mode graphics and goes back to some
//of the earliest special purpose graphics display hardware.The architecture of modern
//graphics systems that employ a GPU leads to a third version of our program.
//Our second approach has one major flaw.Suppose that, as we might in an
//animation, we wish to redisplay the same objects.The geometry of the objects is
//unchanged, but the objects may bemoving.Displaying all the points involves sending
//the data from the CPU to the GPU each time we wish to display the objects in a new
//position.For large amounts of data, this data transfer is the major bottleneck in the
//display process.
// 
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


GLuint VBO[1];

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

	//glFlush();
	glutSwapBuffers();
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
	//glutInitDisplayMode(GLUT_RGBA);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

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
