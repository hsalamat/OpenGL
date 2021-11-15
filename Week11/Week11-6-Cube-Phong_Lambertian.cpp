
/** @file Week11-6-cube-phong-lambertian.cpp
 *  @brief the Phong shading model and Lambertian reflection model
 *  @note press n,g,b to change the diffuse color of the cube
 *  @note press arrow keys to move the light position
 *  @note: camera: eye position is set using polar coordinates
        zNear = 0.5;
		zFar = 3.0;

		radius = 1.0;
		theta = 0.0;
		phi = 0.0;

        you can also change them dynamically using the following keys:
		case 'z': zNear *= 1.1; zFar *= 1.1; break;
		case 'Z': zNear *= 0.9; zFar *= 0.9; break;
		case 'r': radius *= 2.0; break;
		case 'R': radius *= 0.5; break;
		case 'o': theta += dr; break;
		case 'O': theta -= dr; break;
		case 'p': phi += dr; break;
		case 'P': phi -= dr; break;
		case ' ' : reset
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

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

GLfloat cube_vertices[NumVertices][3] = { 0 };
GLfloat cube_colors[NumVertices][3] = { 0 };
GLfloat cube_normals[NumVertices][4] = { 0 };

// IDs.
GLuint vao, ibo, points_vbo, colors_vbo, normals_vbo, modelViewID, projectionID, normalID;

// Fixed timestep variables.
int deltaTime, currentTime, lastTime = 0;

// Matrices.
glm::mat4 mv, view, projection;

float rotAngle = 0;

// Viewing transformation parameters

GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat  dr = 5.0 * 3.14f / 180.0f;

// Projection transformation parameters


GLfloat  zNear = 0.5, zFar = 3.0;

GLfloat  fovy = 45.0f;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect = 4.0f / 2.0f;       // Viewport aspect ratio

glm::vec4 lightPosition = glm::vec4(0.0f, -1.0f, -1.0f, 0.0f);
glm::vec4 lightDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0);
glm::vec4 materialDiffuse = glm::vec4(0.5f, 0.8f, 0.1f, 1.0f);


static unsigned int
program,
vertexShaderId,
fragmentShaderId;

std::array<glm::vec3, 36> vertices = {};
std::array<glm::vec3, 36> colors = {};
std::array<glm::vec3, 36> normals = {};

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

glm::vec3 calculateNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	glm::vec3 normal;
	glm::vec3 aVector = c - a;
	glm::vec3 bVector = b - a;
	glm::vec3 normalVector = glm::cross(aVector, bVector);
	glm::vec3 normalized = glm::normalize(normalVector);
	normal[0] = normalized[0];
	normal[1] = normalized[1];
	normal[2] = normalized[2];
	return normal;
}

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

void quad(int a, int b, int c, int d, int e, int f)
{

	glm::vec3 qNormal = calculateNormal(unique_vertices[a], unique_vertices[b], unique_vertices[c]);


	normals[Index] = qNormal; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[a]; Index++;
	normals[Index] = qNormal; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[b]; Index++;
	normals[Index] = qNormal; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[c]; Index++;
	normals[Index] = qNormal; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[d]; Index++;
	normals[Index] = qNormal; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[e]; Index++;
	normals[Index] = qNormal; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[f]; Index++;

}
//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
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

		cube_normals[i][0] = normals[i][0];
		cube_normals[i][1] = normals[i][1];
		cube_normals[i][2] = normals[i][2];
		cube_normals[i][3] = 0.0f;
	}
}

void init(void)
{
	vertexShaderId = setShader((char*)"vertex", (char*)"sphere2.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"sphere2.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	colorcube();

	modelViewID = glGetUniformLocation(program, "modelViewMatrix");
	projectionID = glGetUniformLocation(program, "projectionMatrix");
	normalID = glGetUniformLocation(program, "normalMatrix");


	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

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

	normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * NumVertices * sizeof(GLfloat), cube_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);



	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void  setupLights() {
	glUniform4f(glGetUniformLocation(program, "lightDiffuse"), lightDiffuse.x, lightDiffuse.y, lightDiffuse.z, 1.0f);
	glUniform4f(glGetUniformLocation(program, "materialDiffuse"), materialDiffuse.x, materialDiffuse.y, materialDiffuse.z, 1.0f);
	glUniform4f(glGetUniformLocation(program, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z, 1.0f);
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
	mv = view * Model;
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &mv[0][0]);

	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
	// normal matrix is only really needed if there is nonuniform scaling
	// it's here for generality but since there is
	// no scaling in this example we could just use modelView matrix in shaders
	//glUniformMatrix4fv(normalID, 1, GL_FALSE, &mv[0][0]);

	glm::mat4 normalMatrix;
	normalMatrix = glm::inverse(Model);
	glUniformMatrix4fv(normalID, 1, GL_TRUE, &normalMatrix[0][0]);

}

//---------------------------------------------------------------------
//
// display
//
void display(void)
{

	setupLights();
	// Delta time stuff.
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec3  eye(radius * sin(theta) * cos(phi),
		radius * sin(theta) * sin(phi),
		radius * cos(theta)
	);

	//glm::vec3  eye(0.0, 0.0, 1.0);
	glm::vec3  at(0.0, 0.0, 0.0);
	glm::vec3  up(0.0, 1.0, 0.0);
	view = glm::lookAt(eye, at, up);


	glBindVertexArray(vao);


	//projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.5f, 3.0f);
	//projection = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	projection = glm::ortho(-1.0f , 1.0f, -1.0f, 1.0f, -100.0f, 100.0f);



	//transformObject(0.2f, X_AXIS, rotAngle = 0.0 , glm::vec3(0.0f, 0.0f, 0.0f));
	transformObject(0.2f, YZ_AXIS, rotAngle += ((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));


	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); // Try GL_LINE_STRIP too!

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Now for a potentially smoother render.
}

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 'z': zNear *= 1.1; zFar *= 1.1; break;
	case 'Z': zNear *= 0.9; zFar *= 0.9; break;
	case 'r': radius *= 2.0; break;
	case 'R': radius *= 0.5; break;
	case 'o': theta += dr; break;
	case 'O': theta -= dr; break;
	case 'p': phi += dr; break;
	case 'P': phi -= dr; break;

	case ' ':  // reset values to their defaults
		zNear = 0.5;
		zFar = 3.0;

		radius = 1.0;
		theta = 0.0;
		phi = 0.0;
		break;

	case 'n':
		materialDiffuse = glm::vec4(1.0, 0.0, 0.0, 1.0);
		break;
	case 'g':
		materialDiffuse = glm::vec4(0.0, 1.0, 0.0, 1.0);
		break;
	case 'b':
		materialDiffuse = glm::vec4(0.0, 0.0, 1.0, 1.0);
		break;

	}

	glutPostRedisplay();
}

void keyDownSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		lightPosition.y += 0.1;
		break;
	case GLUT_KEY_DOWN:
		lightPosition.y -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		lightPosition.x += 0.1;
		break;
	case GLUT_KEY_RIGHT:
		lightPosition.x -= 0.1;
		break;
	}
}

//----------------------------------------------------------------------------

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	aspect = GLfloat(width) / height;
}


void idle()
{
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Cube");

	glewInit();

	init();

	glutIdleFunc(idle);
	glutSpecialFunc(keyDownSpecial);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);

	glutMainLoop();
	return 0;
}
