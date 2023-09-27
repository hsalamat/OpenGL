/** @file Week3-1-Cube.cpp
 *  @brief Cube Demo
 *
 *  Making Cube using GL_TRIANGLES
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

GLuint vao, ibo, points_vbo, colors_vbo, modelID;
float rotAngle = 0.0f;
int deltaTime, currentTime, lastTime = 0;

//GLshort cube_indices[] = {
//	// Front.
//	2, 1, 0,
//	0, 3, 2,
//	// Left.
//	3, 0, 4,
//	4, 7, 3,
//	// Bottom.
//	0, 1, 5,
//	5, 4, 0,
//	// Right.
//	1, 2, 6,
//	6, 5, 1,
//	// Back.
//	5, 6, 7,
//	7, 4, 5,
//	// Top.
//	6, 2, 3,
//	3, 7, 6
//};


GLshort cube_indices[] = {
	// Front.
	0, 1, 2,
	2, 3, 0,
	// Left.
	0, 3, 7,
	7, 4, 0,
	// Bottom.
	0, 4, 5,
	5, 1, 0,
	// Right.
	5, 6, 2,
	2, 1, 5,
	// Back.
	5, 4, 7,
	7, 6, 5,
	// Top.
	3, 2, 6,
	6, 7, 3
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


GLshort quad_indices[] = {
	// Front.
	0, 1, 2, 3,
	// Left.
	0, 3, 7, 4,
	// Bottom.
	0, 4, 5, 1, 
	// Right.
	5, 6, 2, 1, 
	// Back.
	5, 4, 7, 6, 
	// Top.
	3, 2, 6, 7, 
};


GLfloat colors1[] = {
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
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

	vao = 1;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	points_vbo = 1;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 2;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.


	vao = 2;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);

	points_vbo = 3;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 4;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.


	// Enable depth test.
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


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(1);

	transformObject(0.5f, YZ_AXIS, (float)45, glm::vec3(0.0f, 0.0f, 0.0f));
	//transformObject(0.4f, YZ_AXIS, rotAngle -= ((float)90 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	//glDrawArrays(GL_TRIANGLES, 0, 8);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0); // Try GL_LINE_STRIP too!
	

	glBindVertexArray(2);
	glDrawElements(GL_LINE_LOOP, 24, GL_UNSIGNED_SHORT, 0);
	
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
	glutCreateWindow("Cube Tranformation");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();
}
