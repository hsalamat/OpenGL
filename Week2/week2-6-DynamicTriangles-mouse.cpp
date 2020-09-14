
///////////////////////////////////////////////////////////////////////
//
// Dynamic Triangles mouse.cpp
//To Demo this, you have to press the left mouse button three times,
//and it creates a triangle dynamically. Then you click three more times,
//it will create a new triangle
//A mouse event occurs when one of the mouse buttons is either
//depressed or released.When a button is depressed, the action generates a mouse
//down event.When it is released, a mouse up event is generated.The information returned
//includes the button that generated the event, the state of the button after the
//event(up or down), and the position of the cursor tracking the mouse in window
//coordinates(with the origin in the upper - left corner of the window).
//The right mouse button is used to end the program.The left mouse button is used to
//provide the vertex data for our triangle.We use the globals hand w to hold the height
//and width of the OpenGL window.
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

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint vao, points_vbo, colors_vbo;

const GLfloat scale = 0.5f;

int w = 512, h = 512;
int counter = 0;

GLfloat points[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

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


	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.
}


//---------------------------------------------------------------------
//
// display
//

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, 3); // GL_LINE_LOOP works too!

	//glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glFlush(); // Instead of double buffering.
}

void idle()
{
	//glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		exit(0);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		points[counter++] = (float)x / (w / 2) - 1.0;
		points[counter++] = (float)(h - y) / (h / 2) - 1.0;
		points[counter++] = 0;
	}
	if (counter == 9)
	{
		//What this function does is set an internal flag indicating that the display needs to be
		//redrawn.Each time the system goes through the event loop, multiple events may occur
		//whose callbacks require refreshing the display.Rather than each of these callbacks
		//explicitly executing the display function, each uses glutPostRedisplay to set the
		//display flag.Thus, at the end of the event loop, if the flag is set, the display callback
		//is invokedand the flag unset.This method prevents the display from being redrawn
		//multiple times in a single pass through the event loop.

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);

		glutPostRedisplay();
		counter = 0;
	}
}

void myKey(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q') exit(0);
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Dynamic Triangle");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	//We register the mouse callback function, usually in the main function, by means of the GLUT function
	glutMouseFunc(mouse);

	//When a keyboard event occurs, the ASCII code for the key that generated the
	//event and the location of the mouse are returned.All the key - press callbacks are
	//registered in a single callback function, such as
	glutKeyboardFunc(myKey);

	glutIdleFunc(idle);

	glutMainLoop();
}

