
///////////////////////////////////////////////////////////////////////
//
// RotatingTriangle-DoubleBuffering.cpp
//depending on the speed of your computer
//and how much you increment the angle in the idle callback, you may see a display
//that does not show a rotating triangle but rather a somewhat broken - up display with
//pieces of the triangle showing.
//Typically the frame buffer is redisplayed at a regular rate, known as
//the refresh rate, which is in the range of 60 to 100 Hz(or frames per second).
//The more common solution is double buffering.Instead of a single frame buffer,
//the hardware has two frame buffers.One, called the front buffer, is one that is
//displayed.The other, called the back buffer, is then available for constructing what
//we would like to display.Once the drawing is complete, we swap the frontand back buffers
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

const float DegreesToRadians = 3.1415f / 180.0f;
float angle = 0.150 * DegreesToRadians; // small angle in radians

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

	for (int i = 0; i < 3; i = i + 3)
	{
		float x = cos(angle) * points[i] - sin(angle) * points[i + 1];
		float y = sin(angle) * points[i] + cos(angle) * points[i + 1];
		points[i] = x;
		points[i + 1] = y;
	}


	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glClear(GL_COLOR_BUFFER_BIT); // clear the window
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//glFlush(); // Instead of double buffering.
	//rather than using glFlush at the end of the display callback, we use
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//exit(0);
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

//Using menus involves taking a few simple steps.We must specify the actions
//corresponding to each entry in the menu.We must link the menu to a particular
//mouse button.Finally, we must register a callback function for each menu.We can
//demonstrate simple menus with the example of a pop - up menu that has three entries.
//The first selection allows us to exit our program.The second and third start and stop
//the rotation.The function calls to set up the menu and to link it to the right mouse
//button should be placed in our main function.

void demo_menu(int id)
{
	switch (id)
	{
	case 1:
		exit(0);
		break;
	case 2:
		glutIdleFunc(idle);
		break;
	case 3:
		glutIdleFunc(NULL);
		break;
	}
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	//Note that the default in GLUT is equivalent to using GLUT_SINGLE rather than GLUT_DOUBLE.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Dynamic Triangle");

	glutCreateMenu(demo_menu);
	glutAddMenuEntry("quit", 1);
	glutAddMenuEntry("start rotation", 2);
	glutAddMenuEntry("stop rotation", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


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

