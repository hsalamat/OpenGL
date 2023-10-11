/** @file Week5-4-Sphere.cpp
 *  @brief how to create a sphere by subdiving triangle and normalizing their length.
 *
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

const int Ndivisions = 5;
const int NumTetrahedrons = 1024;            // 4^4 tetrahedrons      
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

	int i = colorIndex++ % 3;
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
	default:
		tColor = glm::vec3(0.0, 0.0, 0.0);
	}
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
		//glm::vec3 ab = (a + b) / 2.0f;
		//glm::vec3 ac = (a + c) / 2.0f;
		//glm::vec3 bc = (b + c) / 2.0f;
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
	vertexShaderId = setShader((char*)"vertex", (char*)"cubeProjection.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cubeProjection.frag");
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

	//Vertex Array Object will allow us to bundle data associated with a vertex array.
	//Use of multiple vertex-array objects will make it easy to switch among different vertex arrays.
	vao = 0;
	//We use glGenVertexArray to find an unused name for the buffer.
	glGenVertexArrays(1, &vao);
	//The first time the function glBindVertexArray is executed for a given name, the object is created.
	//Subsequent calls to this function make the named object active.
	glBindVertexArray(vao);

	//we create a buffer object on the GPU and place our data in that object.
	points_vbo = 0;

	//we use glGenBuffers to give us an unused identifier for our buffer object that is put into the variable points_vbo.
	glGenBuffers(1, &points_vbo);

	//The function glBindBuffer creates the buffer with the identifier from glGenBuffers.
	//The type GL_ARRAY_BUFFER indicates that the data in the buffer will be vertex attribute data
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

	//with glBufferData, we allocate sufficient memory on the GPU for our dataand provide a pointer to the array holding the data.
	glBufferData(GL_ARRAY_BUFFER, 3 * NumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//The fourth parameter says that we do not want the data normalized to be the range(0.0, 1.0), 
	//whereas the fifth states that the values in the array are contiguous.
	//The last parameter is the address in the buffer where the data begin.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	//we could also do it the above two lines using follwing: Ask what is the location of the attribute
	//GLuint loc;
	//loc = glGetAttribLocation(program, "vertex_position");
	//glEnableVertexAttribArray(loc);
	//glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

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

	// Update the ortho projection.
	projection = glm::ortho(-1.0f + osH, 1.0f + osH, -1.0f + osV, 1.0f + osV, 0.0f, 100.0f);

	//transformObject(0.4f, YZ_AXIS, rotAngle+=((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));
	transformObject(0.5f, X_AXIS, rotAngle+=((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	//glDrawArrays(GL_LINES, 0, NumVertices);

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
	case 'W':
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		break;
	case 'S':
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		break;
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
	cout << "Mouse pos: " << x << "," << y << endl;
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
	glutCreateWindow("Square of Walls");

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
