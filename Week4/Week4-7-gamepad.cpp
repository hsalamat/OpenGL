
///////////////////////////////////////////////////////////////////////
//
// GamePad.cpp
//glutJoystickFunc — sets the gamepad callback for the current window.
// void glutJoystickFunc	(	void (*func)(unsigned int buttonMask, int x, int y, int z), int pollInterval);
//Parameters
//func
//The new joystick callback function.
//pollInterval
//Joystick polling interval in milliseconds
//The joystick callback is called either due to polling of the joystick at the uniform timer interval specified by pollInterval (in milliseconds) or in response to calling glutForceJoystickFunc.
//Limitations
//The GLUT joystick callback only reports the first 3 axes and 32 buttons.GLUT supports only a single joystick
//The x, y, and z callback parameters report the X, Y, and Z axes of the joystick.The joystick is centered at(0, 0, 0).X, Y, and Z are scaled to range between - 1000 and 1000. 
//Moving the joystick left reports negative X; right reports positive X.Pulling the stick towards you reports negative Y; push the stick away from you reports positive Y.
//If the joystick has a third axis(rudder or up / down), down reports negative Z; up reports positive Z.
//Passing a NULL func to glutJoystickFunc disables the generation of joystick callbacks.Without a joystick callback registered, glutForceJoystickFunc does nothing.
///////////////////////////////////////////////////////////////////////

using namespace std;
#include <iostream>

#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

GLuint vao, ibo, points_vbo, colors_vbo, modelID;
float rotAngle = 0.0f;
int deltaTime, currentTime, lastTime = 0;
glm::vec3 axis(1, 1, 0);
glm::vec3 translation(0.0f, 0.0f, 0.0f);
float scale = 1.0f;
float translationX = 0.0f, translationY = 0.0f, translationZ = 0.0f;

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
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

//glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

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
	// Delta time stuff.
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	
	translation = glm::vec3(translationX * deltaTime/ (float)1000000 , translationY * deltaTime / (float)1000000, translationZ * deltaTime / (float)1000000);

	scale = 0.2f;

	transformObject(scale, axis, rotAngle = 45.0f, translation);

	//Ordering the GPU to start the pipeline
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Instead of double buffering.
}

void idle()
{
	glutPostRedisplay();
}

void  gamepad(unsigned int buttonMask, int x, int y, int z)
{

	if (buttonMask & GLUT_JOYSTICK_BUTTON_A) {
		cout << "button A is pressed " << endl;
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_B) {
		cout << "button B is pressed " << endl;
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_C) {
		cout << "button X is pressed " << endl;
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_D) {
		cout << "button Y is pressed " << endl;
	}


	//left
	if (x < 0) {
		cout << x << " left" <<endl;
		translationX += x;
		cout << "translationX: " << translationX << " , translationY:" << translationY << endl;
		glutPostRedisplay();
	}


	//right
	if (x > 0) {
		cout << x << " right" << endl;
		translationX += x;
		cout << "translationX: " << translationX << " , translationY:" << translationY << endl;
		glutPostRedisplay();
	}

	//up
	if (y > 0) {
		cout << y << " up" << endl;
		translationY -= y;
		cout << "translationX: " << translationX << " , translationY:" << translationY << endl;
		glutPostRedisplay();
	}

	//down
	if (y < 0) {
		cout<< y << " down" << endl;
		translationY -= y;
		cout << "translationX: " << translationX << " , translationY:" << translationY << endl;
		glutPostRedisplay();
	}

	//forward
	if (z > 0) {
		cout << z << " forward" << endl;
		translationZ -= z;
		cout << "translationZ: " << translationZ << " , translationZ:" << translationZ << endl;
		glutPostRedisplay();
	}

	//backward
	if (z < 0) {
		cout << z << " backward" << endl;
		translationZ += z;
		cout << "translationZ: " << translationZ << " , translationZ:" << translationZ << endl;
		glutPostRedisplay();
	}
	

}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Hello World");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	//glutIdleFunc(idle);

	glutJoystickFunc(gamepad, 33);
	//glutForceJoystickFunc();

	glutMainLoop();
}
