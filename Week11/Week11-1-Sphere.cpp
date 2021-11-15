
/** @file Week11-1-sphere.cpp
 *  @brief Rotating Sphere
 *  Orthographic projection of a sphere
 *  For our model, we use our approximation of a sphere geometry that we created in the Week5.
 *  @note press WASD to move the rotating sphere
 *  @author Hooman Salamat
 *  @bug No known bugs.
 */



#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <array>
using namespace std;

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

GLuint vao, ibo, points_vbo, colors_vbo, modelID;
float rotAngle = 0.0f;
int deltaTime, currentTime, lastTime = 0;

// Horizontal and vertical ortho offsets.
float osH = 0.0f, osV = 0.0f, scrollSpd = 0.25f;

glm::mat4 mvp, view, projection;

const int Ndivisions = 5; // 7; // 5;
const int NumTetrahedrons = 1024; // 16384; // 1024;            // 4^4 tetrahedrons      
const int NumTriangles = 4 * NumTetrahedrons;  // 4 triangles / tetrahedron
const int NumVertices = 3 * NumTriangles;      // 3 vertices / triangle

std::array<glm::vec3, NumVertices> vertices1;
GLfloat vertices[NumVertices][3] = { 0 };
glm::vec3 v[4];

std::array<glm::vec3, NumVertices> colors1;
GLfloat colors[NumVertices][3] = { 0 };
glm::vec3 c[3] = { glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0),glm::vec3(0.0, 0.0, 1.0) };



int  Index = 0;
int  colorIndex = 0;

static unsigned int
program,
vertexShaderId,
fragmentShaderId;


void triangle(glm::vec3& a, glm::vec3& b, glm::vec3& c)
/* specify one triangle */
{

	int i = colorIndex++ % 4;
	glm::vec3 tColor = glm::vec3(0.0, 0.0, 0.0);
	switch (i)
	{
	case 0:
		tColor = glm::vec3(1.0, 0.0, 0.0);
		break;
	case 1:
		tColor = glm::vec3(0.0, 1.0, 0.0);
		break;
	case 2:
		tColor = glm::vec3(0.0, 0.0, 1.0);
		break;
	case 3:
		tColor = glm::vec3(1.0, 0.0, 1.0);
		break;
	default:
		tColor = glm::vec3(0.0, 0.0, 0.0);
	}


	//tColor = glm::vec3(0.0, 1.0, 0.0);

	colors1[Index] = tColor;
	vertices1[Index++] = a;
	colors1[Index] = tColor;
	vertices1[Index++] = b;
	colors1[Index] = tColor;
	vertices1[Index++] = c;
}


void divide_triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, int k)
{
	if (k > 0)
	{
		// compute midpoints of sides
		glm::vec3 ab = glm::normalize((a + b) / 2.0f);
		glm::vec3 ac = glm::normalize((a + c) / 2.0f);
		glm::vec3 bc = glm::normalize((b + c) / 2.0f);
		// subdivide all but inner triangle
		divide_triangle(a, ab, ac, k - 1);
		divide_triangle(ab, b, bc, k - 1);
		divide_triangle(bc, c, ac, k - 1);
		divide_triangle(ab, bc, ac, k - 1);
	}
	else triangle(a, b, c); /* draw triangle at end of recursion */
}

void tetra(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d, int n)
{
	divide_triangle(a, b, c, n);
	divide_triangle(a, c, d, n);
	divide_triangle(a, d, b, n);
	divide_triangle(b, d, c, n);
}


void init(void)
{
	vertexShaderId = setShader((char*)"vertex", (char*)"sphere.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"sphere.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	modelID = glGetUniformLocation(program, "mvp");

	// frustum parameters: left, right, bottom, top, nearVal, farVal
	//projection = glm::frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); // In world coordinates

	// Camera matrix
	view = glm::lookAt(
		glm::vec3(0, 0, 1),	// Camera pos in World Space
		glm::vec3(0, 0, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);

	v[0] = glm::vec3(0.0, 0.0, -1.0);
	v[1] = glm::vec3(0.0, 0.942809, 0.333333);
	v[2] = glm::vec3(-0.816497, -0.471405, 0.333333);
	v[3] = glm::vec3(0.816497, -0.471405, 0.333333);


	tetra(v[0], v[1], v[2], v[3], Ndivisions);


	//// compute and store N-1 new points
	for (int i = 0; i < NumVertices; ++i) {


		vertices[i][0] = vertices1[i][0];
		vertices[i][1] = vertices1[i][1];
		vertices[i][2] = vertices1[i][2];


		colors[i][0] = colors1[i][0];
		colors[i][1] = colors1[i][1];
		colors[i][2] = colors1[i][2];
	}

	vao = 0;	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	points_vbo = 0;

	glGenBuffers(1, &points_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

	glBufferData(GL_ARRAY_BUFFER, 3 * NumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * NumVertices * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

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


void display(void)
{
	// Delta time stuff.
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	// Update the ortho projection.
	projection = glm::ortho(-1.0f + osH, 1.0f + osH, -1.0f + osV, 1.0f + osV, 0.0f, 100.0f);
	//projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.5f, 14.0f);

	//transformObject(0.4f, YZ_AXIS, rotAngle+=((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));
	transformObject(0.5f, X_AXIS, rotAngle += ((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	//glDrawArrays(GL_LINE_LOOP, 0, NumVertices);

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
	case 'w':
		osV -= scrollSpd;
		break;
	case 's':
		osV += scrollSpd;
		break;
	case 'a':
		osH += scrollSpd;
		break;
	case 'd':
		osH -= scrollSpd;
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
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Sphere");

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
	glutMainLoop();
}
