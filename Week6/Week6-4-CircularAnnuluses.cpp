
///////////////////////////////////////////////////////////////////////       
// CircularAnnuluses.cpp
//
// This program draws three identical-looking circular annuluses in three different ways - 
// see comments below.
//
//Upper-left: There is not a real hole. The white disc overwrites the red disc
//
//Upper-right: There is not a real hole either. A white disc is drawn closer to the
//viewer than the blue disc blocking it out :
//Observe that the z - value of the white disc's center is greater than the red disc's,
//bringing it closer to the viewing face.
//
// Interaction: 
// Press the space bar to toggle between wirefrime and filled for the lower annulus.
// Hooman Salamat
///////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////


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
#define N 40.0 // Number of vertices on the boundary of the disc.
const int MaxNumVertices = 500;
static int numVertices = 40;

// Globals.
static int isWire = 0; // Is wireframe?

// Font selection: A fixed width font with every character fitting in an 8 by 13 pixel rectangle.
static long font = (long)GLUT_BITMAP_8_BY_13;

#define PI 3.14159265358979324

float theta = 0.0f;

// Routine to draw a bitmap character string.
void writeBitmapString(void* font, char* string)
{
	char* c;
	//glutBitmapCharacter renders a bitmap character using OpenGL.
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}



std::array<glm::vec3, MaxNumVertices> vertices = {};
std::array<glm::vec3, MaxNumVertices> colors = {};

GLfloat shape_vertices[MaxNumVertices][3] = { 0 };
GLfloat shape_colors[MaxNumVertices][3] = { 0 };

std::array<glm::vec3, 8> unique_colors = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 1.0f, 1.0f),
	glm::vec3(0.5f, 0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)   //white
};

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

void drawDisc(float R, float X, float Y, float Z, int colorIndex)
{
	float t=0.0f;
	int i;
	for ( i = 0; i < numVertices; ++i)
	{

		vertices[i]= glm::vec3(X + R * cos(t), Y + R * sin(t), Z);
		//colors[i] = glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
		colors[i] = unique_colors[colorIndex];

		t += 2 * PI / numVertices;
	}

	for ( i = 0; i < numVertices; ++i) {

		shape_vertices[i][0] = vertices[i][0];
		shape_vertices[i][1] = vertices[i][1];
		shape_vertices[i][2] = vertices[i][2];

		shape_colors[i][0] = colors[i][0];
		shape_colors[i][1] = colors[i][1];
		shape_colors[i][2] = colors[i][2];
	}

	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_vertices), shape_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_colors), shape_colors, GL_STATIC_DRAW);

	//transformObject(0.4f, YZ_AXIS, rotAngle+=((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));
	transformObject(1.0f, X_AXIS, rotAngle += 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
}


void drawAnnulus(float R, float X, float Y, float Z, int colorIndex)
{
	float t=0.0f;
	int i;
	for (i = 0; i < numVertices*2; i=i+2)
	{
		vertices[i] = glm::vec3(X + R * cos(t), Y + R * sin(t), Z);
		//colors[i] = glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
		colors[i] = unique_colors[colorIndex];

		vertices[i + 1] = glm::vec3(X + (R + 10) * cos(t), Y + (R + 10) * sin(t), Z);
		//colors[i+1] = glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
		colors[i+1] = unique_colors[colorIndex];

		t = PI * i / numVertices * 2;	
	}

	for (i = 0; i < numVertices*2; ++i) {

		shape_vertices[i][0] = vertices[i][0];
		shape_vertices[i][1] = vertices[i][1];
		shape_vertices[i][2] = vertices[i][2];

		shape_colors[i][0] = colors[i][0];
		shape_colors[i][1] = colors[i][1];
		shape_colors[i][2] = colors[i][2];
	}

	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_vertices), shape_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_colors), shape_colors, GL_STATIC_DRAW);
	transformObject(1.0f, X_AXIS, rotAngle += 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices * 2);  //numVertices * 2
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

	//projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -4.0f, 4.0f); // In world coordinates

	// Camera matrix
	view = glm::lookAt(
		glm::vec3(0, 0, 1),		// Camera pos in World Space
		glm::vec3(0, 0, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);


	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


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

	
// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}



//---------------------------------------------------------------------
//
// display
//

void display(void)
{
	// Delta time stuff.
	//currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	//deltaTime = currentTime - lastTime;
	//lastTime = currentTime;

	if (isWire) {
		glPolygonMode(GL_FRONT, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	//theta += 0.1;
	//if (theta > 2.0f * 3.1415f) theta -= 2.0f * 3.1415f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update the ortho projection.
	projection = glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 100.0f);

	glBindVertexArray(1);

	//Upper-left: There is not a real hole. The white disc overwrites the green disc
	glDisable(GL_DEPTH_TEST);
	drawDisc(20.0, 25.0, 75.0, 0.0, 1);
	drawDisc(10.0, 25.0, 75.0, 0.0, 7);
	
	////Upper-right: There is not a real hole either. A white disc is drawn closer to the
	////viewer than the blue disc blocking it out :
	////
	glEnable(GL_DEPTH_TEST);
	drawDisc(20.0, 75.0, 75.0, 0.0, 2);
	drawDisc(10.0, 75.0, 75.0, 0.5, 7);
	glDisable(GL_DEPTH_TEST);

	//////
	////////Lower: A true circular annulus with a real hole :
	drawAnnulus(10.0, 50.0, 25.0, 0.0, 6);


	//////// Write labels.
	//////glColor3f(0.0, 0.0, 0.0);
	
	glRasterPos3f(15.0, 51.0, 0.0);
	writeBitmapString((void*)font, (char*)"Overwritten");
	glRasterPos3f(69.0, 51.0, 0.0);
	writeBitmapString((void*)font, (char*)"Floating");
	glRasterPos3f(38.0, 2.0, 0.0);
	writeBitmapString((void*)font, (char*)"The real deal!");


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
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Circular Annuluses");

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
