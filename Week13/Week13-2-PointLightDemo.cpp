///////////////////////////////////////////////////////////////////////
//Point Light using inverse square
//Physically, light intensity weakens as a function of distance based on the inverse squared law(attenuation = 1 / distance^2).
//The light intensity at a point a distance d away from the light source is given by : DiffuseLight / distance^2
//
// Hooman Salamat
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <array>
#include "light.h"
using namespace std;

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

GLuint vao, ibo, points_vbo, colors_vbo, normals_vbo, projectionID, normalID, modelID, viewID;
float rotAngle = 0.0f;
int deltaTime, currentTime, lastTime = 0;

// Horizontal and vertical ortho offsets.
float osH = 0.0f, osV = 0.0f, scrollSpd = 0.25f;

glm::mat4 mv, view, projection;

const int Ndivisions = 5;
const int NumTetrahedrons = 1024; // 4^5 tetrahedrons      
const int NumTriangles = 4 * NumTetrahedrons;  // 4 triangles / tetrahedron
const int NumVertices = 3 * NumTriangles;      // 3 vertices / triangle

std::array<glm::vec3, NumVertices> vertices1;
GLfloat vertices[NumVertices][3] = { 0 };
glm::vec3 v[4];

std::array<glm::vec3, NumVertices> colors1;
GLfloat colors[NumVertices][3] = { 0 };
glm::vec3 c[3] = { glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0),glm::vec3(0.0, 0.0, 1.0) };

std::array<glm::vec3, NumVertices> normals1;
GLfloat normals[NumVertices][4] = { 0 };

int  Index = 0;
int  colorIndex = 0;


// Light variables.
Ambient aLight(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

Specular sLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 40.0);

DirectionalLight dLight(glm::vec3(0.0f, -1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0), glm::vec4(1.0f, 0.8f, 0.0f, 1.0f));

PointLight pLight(glm::vec3(0.0f, 0.0f, -1.0f),	// Position.
	1.0f, 0.7f, 1.8f,				// Constant, Linear, Exponent.
	glm::vec4(1.0f, 1.0f, 1.0f, 1.0), glm::vec4(1.0f, 0.8f, 0.0f, 1.0f));



static unsigned int
program,
vertexShaderId,
fragmentShaderId;


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


	glm::vec3 qNormal = calculateNormal(a, b, c);
	normals1[Index] = qNormal;
	colors1[Index] = tColor;
	vertices1[Index++] = a;

	normals1[Index] = qNormal;
	colors1[Index] = tColor;
	vertices1[Index++] = b;

	normals1[Index] = qNormal;
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

void setupBuffers()
{
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

	normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * NumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
}
void  setupLights() {
	glUniform4f(glGetUniformLocation(program, "aLight.light"), aLight.light.x, aLight.light.y, aLight.light.z, aLight.light.w);
	glUniform4f(glGetUniformLocation(program, "aLight.material"), aLight.material.x, aLight.material.y, aLight.material.z, aLight.material.w);

	glUniform4f(glGetUniformLocation(program, "sLight.light"), sLight.light.x, sLight.light.y, sLight.light.z, sLight.light.w);
	glUniform4f(glGetUniformLocation(program, "sLight.material"), sLight.material.x, sLight.material.y, sLight.material.z, sLight.material.w);
	glUniform1f(glGetUniformLocation(program, "sLight.shininess"), sLight.shininess);

	glUniform3f(glGetUniformLocation(program, "pLight.position"), pLight.position.x, pLight.position.y, pLight.position.z);
	glUniform1f(glGetUniformLocation(program, "pLight.constant"), pLight.constant);
	glUniform1f(glGetUniformLocation(program, "pLight.linear"), pLight.linear);
	glUniform1f(glGetUniformLocation(program, "pLight.exponent"), pLight.exponent);
	glUniform4f(glGetUniformLocation(program, "pLight.base.light"), pLight.light.x, pLight.light.y, pLight.light.z, pLight.light.w);
	glUniform4f(glGetUniformLocation(program, "pLight.base.material"), pLight.material.x, pLight.material.y, pLight.material.z, pLight.material.w);

}

void init(void)
{
	vertexShaderId = setShader((char*)"vertex", (char*)"sphere8.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"sphere8.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	modelID = glGetUniformLocation(program, "modelMatrix");
	viewID = glGetUniformLocation(program, "viewMatrix");
	projectionID = glGetUniformLocation(program, "projectionMatrix");
	normalID = glGetUniformLocation(program, "normalMatrix");

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

		//normals[i][0] = normals1[i][0];
		//normals[i][1] = normals1[i][1];
		//normals[i][2] = normals1[i][2];
		//normals[i][3] = 0.0f;

		//The position of each vertex is the true normal why bother calculating normal!
		normals[i][0] = vertices1[i][0];
		normals[i][1] = vertices1[i][1];
		normals[i][2] = vertices1[i][2];
		normals[i][3] = 0.0f;
	}

	setupBuffers();
	setupLights();

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

	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
}


void display(void)
{
	setupLights();
	// Delta time stuff.
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	// Update the ortho projection.
	projection = glm::ortho(-1.0f + osH, 1.0f + osH, -1.0f + osV, 1.0f + osV, 0.0f, 100.0f);
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


void keyDownSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		pLight.position.y += 0.1;
		break;
	case GLUT_KEY_DOWN:
		pLight.position.y -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		pLight.position.x -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		pLight.position.x += 0.1;
		break;
	case GLUT_KEY_PAGE_UP:
		pLight.position.z += 0.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		pLight.position.z -= 0.1;
	}

	cout << "Light Position: (" << pLight.position.x << "," << pLight.position.y << "," << pLight.position.z << ")" << endl;
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
	glutCreateWindow("Point Light Demo");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.
	init();

	// Set all our glut functions.
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(33, timer, 0);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseDown);
	glutSpecialFunc(keyDownSpecial);
	glutPassiveMotionFunc(mouseMove); // or...
	glutMainLoop();
}
