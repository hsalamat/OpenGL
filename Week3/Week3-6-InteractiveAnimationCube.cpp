/** @file Week3-6-InteractiveAnimationCube.cpp
 *  @brief Interactive Animation Demo
 *
 *  Interactively, via keyboard or mouse input, with the help of their callback routines
 *  to invoke transformations.
 *
 *  @author Hooman Salamat
 *  @bug No known bugs.
 */

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

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

glm::vec3 axis = X_AXIS;
int axis2 = 0;
float theta[3] = { 0.0f,0.0f,0.0f };

GLuint vao, ibo, points_vbo, colors_vbo, modelID;
float rotAngle = 0.0f;
int deltaTime, currentTime, lastTime = 0;
int w = 512, h = 512;
int counter = 0;

GLshort cube_indices[] = {
	// Front.
	3, 2, 1, 0,
	// Left.
	0, 3, 7, 4,
	// Bottom.
	4, 0, 1, 5,
	// Right.
	5, 1, 2, 6,
	// Back.
	6, 5, 4, 7,
	// Top.
	7, 6, 2, 3
};

GLfloat cube_vertices[] = {
	-0.9f, -0.9f, 0.9f,		// 0.
	0.9f, -0.9f, 0.9f,		// 1.
	0.9f, 0.9f, 0.9f,		// 2.
	-0.9f, 0.9f, 0.9f,		// 3.
	-0.9f, -0.9f, -0.9f,	// 4.
	0.9f, -0.9f, -0.9f,		// 5.
	0.9f, 0.9f, -0.9f,		// 6.
	-0.9f, 0.9f, -0.9f,		// 7.
};

GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f
};

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

	modelID = glGetUniformLocation(program, "model");

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);
}

//---------------------------------------------------------------------
//
// transformModel
//

void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
}

//---------------------------------------------------------------------
//
// display
//

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	transformObject(0.5f, axis, (float)theta[axis2]++, glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);
	//glDrawElements(GL_LINE_LOOP, 24, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Instead of double buffering.
}

void idle()
{
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		axis = X_AXIS; 
		axis2 = 0;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		axis = Y_AXIS;
		axis2 = 1;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		axis = Z_AXIS;
		axis2 = 2;
	}

}

void myKey(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q') exit(0);

	if (key == 'x')
	{
		axis = X_AXIS;
		axis2 = 0;
	}
	if (key == 'y')
	{
		axis = Y_AXIS;
		axis2 = 1;
	}

	if (key == 'z')
	{
		axis = Z_AXIS;
		axis2 = 2;
	}
}
//---------------------------------------------------------------------
//
// main
//


void spinCube()
{
	if (theta[axis2] > 360.0) theta[axis2] -= 360.0;
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
	glutCreateWindow("Interactive animation");


	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	//We register the mouse callback function, usually in the main function, by means of the GLUT function
	glutMouseFunc(mouse);

	//When a keyboard event occurs, the ASCII code for the key that generated the
	//event and the location of the mouse are returned.All the key - press callbacks are
	//registered in a single callback function, such as
	glutKeyboardFunc(myKey);

	glutIdleFunc(spinCube);


	glutMainLoop();


}
