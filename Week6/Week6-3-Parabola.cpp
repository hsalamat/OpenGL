
///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////         
// @file Week6-3-Parabola.cpp
// @brief create Parabola
// This program draws a line strip with vertices on a parabola.
// In mathematics, a parabola is a plane curve which is mirror-symmetrical and is approximately U-shaped. 
// It fits several superficially different mathematical descriptions, 
// which can all be proved to define exactly the same curves.
// The graph of a quadratic function  y = ax^2 + bx + c  is a parabola if a!=0 
// and, conversively, a parabola is the graph of a quadratic function if its axis is parallel to the y-axis.
// 
// @attention Increasing the number of vertices makes the strip approximate the parabola.
//
// Interaction:
// @note: Press +/- to increase/decrease the number of vertices of the strip.
// @author Hooman Salamat
///////////////////////////////////////////////////////////////////// 



#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <array>
using namespace std;

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

GLuint vao, ibo, points_vbo, colors_vbo, modelID, colorID;
float rotAngle = 0.0f;
int deltaTime, currentTime, lastTime = 0;

// Horizontal and vertical ortho offsets.
float osH = 0.0f, osV = 0.0f, scrollSpd = 0.25f;

glm::mat4 mvp, view, projection;

static unsigned int
program,
vertexShaderId,
fragmentShaderId;

// Globals.
const int MaxNumVertices = 500; // Number of vertices on circle.
int numVertices = 0;


std::array<glm::vec3, MaxNumVertices> vertices = {};
std::array<glm::vec3, MaxNumVertices> colors = {};

GLfloat shape_vertices[MaxNumVertices][3] = { 0 };
GLfloat shape_colors[MaxNumVertices][3] = { 0 };
GLint shape_indices[MaxNumVertices] = { 0 };

// Globals.
static int isWire = 0; // Is wireframe?
static int M = 3; // Number of vertices on one half of the parabola.
static float C = 50.0;
static float B = 50.0;
static float A = 100.0;

void createModel()
{
	numVertices = 0;
	for (int i = -M; i <= M; ++i)
	{
		vertices[numVertices] = glm::vec3(C + B * (float)i / M, A * (float)(i * i) / (M * M), 0.0);
		//colors[numVertices] = glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
		colors[numVertices] = glm::vec3(0.0f, 191.0f, 255.0f);

		numVertices++;
	}

	for (int i = 0; i < numVertices; ++i) {

		shape_vertices[i][0] = vertices[i][0];
		shape_vertices[i][1] = vertices[i][1];
		shape_vertices[i][2] = vertices[i][2];

		shape_colors[i][0] = colors[i][0];
		shape_colors[i][1] = colors[i][1];
		shape_colors[i][2] = colors[i][2];

		shape_indices[i] = i;
	}
}



void init(void)
{
	vertexShaderId = setShader((char*)"vertex", (char*)"cubeProjection.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cubeProjection.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	modelID = glGetUniformLocation(program, "mvp");
	colorID = glGetAttribLocation(program, "vertex_color");

	// Camera matrix
	view = glm::lookAt(
		glm::vec3(0, 0, 2),		// Camera pos in World Space
		glm::vec3(0, 0, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);


	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shape_indices), shape_indices, GL_STATIC_DRAW);


	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(shape_vertices), shape_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(shape_colors), shape_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//// Number of vertices on one half of the parabola.
	createModel();

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
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
	mvp = projection * view * Model;
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mvp[0][0]);
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

	//uncomment for rotation
	//theta += 0.1;
	//if (theta > 2.0f * 3.1415f) theta -= 2.0f * 3.1415f;	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update the ortho projection.
	projection = glm::ortho(0.0, 100.0, 0.0, 100.0, -4.0, 4.0);

	//Comment this out if you are not interested in changing colors randomly
	createModel();

	glBindVertexArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shape_indices), shape_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_vertices), shape_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_colors), shape_colors, GL_STATIC_DRAW);

	//transformObject(0.4f, YZ_AXIS, rotAngle+=((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));
	transformObject(1.0f, X_AXIS, rotAngle = 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	//Ordering the GPU to start the pipeline

	if (isWire)
	{
		glDrawArrays(GL_LINE_STRIP, 0, numVertices);
		//glDrawElements(GL_LINE_LOOP, numVertices, GL_UNSIGNED_INT, NULL);
	}
	else
	{
		glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
		//glDrawElements(GL_TRIANGLE_FAN, numVertices, GL_UNSIGNED_INT, NULL);
	}



	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Instead of double buffering.
}

void idle()
{
	//glutPostRedisplay();
}

void timer(int id)
{
	glutPostRedisplay();
	glutTimerFunc(33, timer, 0);
}

void keyDown(unsigned char key, int x, int y)
{
	// Orthographic.
	switch (key)
	{
	case 'a':
		A--;
		glutPostRedisplay();
		break;
	case 'A':
		A++;
		glutPostRedisplay();
		break;
	case 'b':
		B--;
		glutPostRedisplay();
		break;
	case 'B':
		B++;
		glutPostRedisplay();
		break;
	case 'c':
		C--;
		glutPostRedisplay();
		break;
	case 'C':
		C++;
		glutPostRedisplay();
		break;
	case '+':
		M++;
		glutPostRedisplay();
		break;
	case '-':
		if (M > 2) M--;
		glutPostRedisplay();
		break;
	case ' ':
		if (isWire == 0) isWire = 1;
		else isWire = 0;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	// Empty for now.
}

void mouseMove(int x, int y)
{
	//cout << "Mouse pos: " << x << "," << y << endl;
}

void mouseDown(int btn, int state, int x, int y)
{
	cout << "Clicked: " << (btn == 0 ? "left " : "right ") << (state == 0 ? "down " : "up ") <<
		"at " << x << "," << y << endl;
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Dynamic Polygon");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.
	init();

	// Set all our glut functions.
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(33, timer, 0);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseDown);
	glutPassiveMotionFunc(mouseMove); // or...
	//glutMotionFunc(mouseMove); // Requires click to register.
	glutMainLoop();
}
