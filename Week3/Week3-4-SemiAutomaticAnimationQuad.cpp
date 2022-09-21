//Semi-automatic animation: we call a routine timer function, called the timer function, 
//with a call to glutTimerFunc(period, timer function, value). 
//The timer function is called period milliseconds after the glutTimerFunc() statement is executed 
//and with the parameter value being passed to it.
//In this Demo, if users key in the escape button or rigth mouse button, the application stops


///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
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

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint vao, points_vbo, colors_vbo;

const GLfloat scale = 0.5f;

float x_position = 0.0f;
int direction = 1;


GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f
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
	GLfloat points[] = {
	x_position,			 0.1, 0.0f,
	x_position,			-0.1, 0.0f,
	x_position + 0.2,	-0.1, 0.0f,
	x_position + 0.2,	 0.1, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.
	glBindVertexArray(0);

}


//---------------------------------------------------------------------
//
// display
//

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	GLfloat points[] = {
	x_position,			 0.1, 0.0f,
	x_position,			-0.1, 0.0f,
	x_position + 0.2,	-0.1, 0.0f,
	x_position + 0.2,	 0.1, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_QUADS, 0, 4); // GL_LINE_LOOP works too!

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Instead of double buffering.
}

void idle()
{
	//glutPostRedisplay();
}

void timer(int)
{
	glutPostRedisplay(); //call the drawScene
	glutTimerFunc(1000 / 30, timer, 0); //ine one second, this function calls itself 60 times

	switch (direction)
	{
	case 1:

		if (x_position < 0.8)
		{
			x_position += 0.1;
		}
		else
		{
			direction = -1;
		}
		break;

	case -1:
		if (x_position >= -1.0)
		{
			x_position -= 0.1;
		}
		else
		{
			direction = 1;
		}
		break;
	}
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Quad Animation");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	//glutIdleFunc(idle);
	glutTimerFunc(1000, timer, 0); //every second

	glutMainLoop();
}
