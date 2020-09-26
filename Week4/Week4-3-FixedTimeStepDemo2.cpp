
///////////////////////////////////////////////////////////////////////
//
// FixedTimeStep movement
// Different Color Per Face
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

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

GLuint vao, ibo, points_vbo, colors_vbo, modelID;
float rotAngle = 0.0f;
int deltaTime, currentTime, lastTime = 0;

float timePerFrame = 1.0f/60.0f; //fixed time step
float timeSinceLastUpdate = 0.0f;

static unsigned int
program,
vertexShaderId,
fragmentShaderId;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

GLfloat cube_vertices[NumVertices][3] = { 0 };
GLfloat cube_colors[NumVertices][3] = { 0 };

GLshort cube_indices[] = {
	// Front.
	2, 1, 0,
	0, 3, 2,
	// Left.
	3, 0, 4,
	4, 7, 3,
	// Bottom.
	0, 1, 5,
	5, 4, 0,
	// Right.
	1, 2, 6,
	6, 5, 1,
	// Back.
	5, 6, 7,
	7, 4, 5,
	// Top.
	6, 2, 3,
	3, 7, 6
};


std::array<glm::vec3, 36> vertices = {};
std::array<glm::vec3, 36> colors = {};

std::array<glm::vec3, 8> unique_vertices = {
	glm::vec3(-0.9f, -0.9f, 0.9f),		// 0.
	glm::vec3(0.9f, -0.9f, 0.9f),		// 1.
	glm::vec3(0.9f, 0.9f, 0.9f),		// 2.
	glm::vec3(-0.9f, 0.9f, 0.9f),		// 3.
	glm::vec3(-0.9f, -0.9f, -0.9f),	    // 4.
	glm::vec3(0.9f, -0.9f, -0.9f),		// 5.
	glm::vec3(0.9f, 0.9f, -0.9f),		// 6.
	glm::vec3(-0.9f, 0.9f, -0.9f)		// 7.
};

std::array<glm::vec3, 8> unique_colors = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 1.0f, 1.0f),
	glm::vec3(0.5f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.5f, 0.0f)
};

int Index = 0;

void
quad(int a, int b, int c, int d, int e, int f)
{
	//colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[a]; Index++;
	//colors[Index] = unique_colors[b]; vertices[Index] = unique_vertices[b]; Index++;
	//colors[Index] = unique_colors[c]; vertices[Index] = unique_vertices[c]; Index++;
	//colors[Index] = unique_colors[d]; vertices[Index] = unique_vertices[d]; Index++;
	//colors[Index] = unique_colors[e]; vertices[Index] = unique_vertices[e]; Index++;
	//colors[Index] = unique_colors[f]; vertices[Index] = unique_vertices[f]; Index++;

	//a color for each face

	colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[a]; Index++;
	colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[b]; Index++;
	colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[c]; Index++;
	colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[d]; Index++;
	colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[e]; Index++;
	colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[f]; Index++;

}
// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
	quad(2, 1, 0, 0, 3, 2);
	quad(3, 0, 4, 4, 7, 3);
	quad(0, 1, 5, 5, 4, 0);
	quad(1, 2, 6, 6, 5, 1);
	quad(5, 6, 7, 7, 4, 5);
	quad(6, 2, 3, 3, 7, 6);

	for (int i = 0; i < NumVertices; ++i) {

		cube_vertices[i][0] = vertices[i][0];
		cube_vertices[i][1] = vertices[i][1];
		cube_vertices[i][2] = vertices[i][2];

		cube_colors[i][0] = colors[i][0];
		cube_colors[i][1] = colors[i][1];
		cube_colors[i][2] = colors[i][2];
	}
}

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

	colorcube();

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);*/

	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	//Enable depth test.
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
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

	timeSinceLastUpdate += deltaTime;
	if (timeSinceLastUpdate > timePerFrame)
	{
		timeSinceLastUpdate -= timePerFrame;
		transformObject(0.4f, YZ_AXIS, rotAngle -= ((float)90 * timePerFrame), glm::vec3(0.0f, 0.0f, 0.0f));

		//Ordering the GPU to start the pipeline
		glDrawArrays(GL_TRIANGLES, 0, 36); // Try GL_LINE_STRIP too!
	}

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Now for a potentially smoother render.
}

void idle()
{
	glutPostRedisplay();
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
	glutCreateWindow("Cube Color");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();
}
