
/** @file Week10-3-DiceDemo.cpp
 *  @brief Dice demo - using spritesheet
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
#include "glm\gtc\type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <array>
using namespace std;

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)
#define XYZ_AXIS glm::vec3(1,1,1)

using namespace std;

// IDs.
GLuint vao, ibo, points_vbo, colors_vbo, textures_vbo, modelID;

// Horizontal and vertical ortho offsets.
float osH = 0.0f, osV = 0.0f, scrollSpd = 0.25f, zoom = 10.0f;

// Fixed timestep variables.
int deltaTime, currentTime, lastTime = 0;

// Matrices.
glm::mat4 mvp, view, projection;

// Camera and transform variables.
glm::vec3 position, frontVec, worldUp, upVec, rightVec; // Set by function.
GLfloat pitch, yaw,
moveSpeed = 0.1f,
turnSpeed = 1.0f;
float rotAngle = 0.0f;

// Mouse variables.
bool mouseFirst = true, mouseClicked = false;
int lastX, lastY;

// Texture variables.
GLuint textureID;
GLint width, height, bitDepth;

GLshort cube_indices[] = {
	// Front.
	2, 1, 0,
	0, 3, 2,
	// Left. Reverse.
	3, 0, 12,
	12, 13, 3,
	// Bottom. Reverse.
	0, 1, 9,
	9, 8, 0,
	// Right. 
	1, 2, 6,
	6, 5, 1,
	// Back. Reverse.
	5, 6, 7,
	7, 4, 5,
	// Top.
	11, 2, 3,
	3, 10, 11
};

GLfloat cube_vertices[] = {
	-0.9f, -0.9f, 0.9f,  
	0.9f, -0.9f, 0.9f,   
	0.9f, 0.9f, 0.9f,    
	-0.9f, 0.9f, 0.9f,   
	-0.9f, -0.9f, -0.9f, 
	0.9f, -0.9f, -0.9f,  
	0.9f, 0.9f, -0.9f,   
	-0.9f, 0.9f, -0.9f,	 

	-0.9f, -0.9f, -0.9f, 
	0.9f, -0.9f, -0.9f,  
	-0.9f, 0.9f, -0.9f,  
	0.9f, 0.9f, -0.9f,   
	-0.9f, -0.9f, -0.9f, 
	-0.9f, 0.9f, -0.9f,  
};


GLfloat cube_textures[] = {
	 0.25f, 0.33f,	// 0.
	 0.5f, 0.33f,		// 1.
	 0.5f, 0.66f,		// 2.
	 0.25f, 0.66f, 	// 3.
	 1.0f, 0.33f,	// 4.
	 0.75f, 0.33f,	// 5.
	 0.75f, 0.66f, 	// 6.
	 1.0f, 0.66f,	// 7.

	 0.25f, 0.0f,	// 4b. 8
	 0.5f, 0.0f,		// 5b. 9
	 0.25f, 1.0f,	// 7b. 10
	 0.5f, 1.0f,		// 6b. 11
	 0.0f, 0.33f,	// 4c. 12
	 0.0f, 0.66f		// 7c. 13
};

GLfloat colors[] = {
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};


static unsigned int
program,
vertexShaderId,
fragmentShaderId;


// Globals.
static int isWire = 0; // Is wireframe?

void resetView()
{
	position = glm::vec3(0.0f, 0.0f, 5.0f);
	frontVec = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0, 1, 0);
	pitch = 0.0f;
	yaw = -90.0f;
}

void loadTexture()
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* image = stbi_load("Media/die.png", &width, &height, &bitDepth, 0);
	if (!image) {
		cout << "Unable to load file!" << endl;
		// Could add a return too if you modify init.
	}

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, 
		height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Clean up. But we don't want to unbind the texture or we cannot use it.
	stbi_image_free(image);
}

void createBuffer()
{
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Now for the UV/ST values.
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(cube_vertices[0]) * 5, (void*)(sizeof(cube_vertices[0]) * 3));
	//glEnableVertexAttribArray(2);


	textures_vbo = 0;
	glGenBuffers(1, &textures_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, textures_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_textures), cube_textures, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	



	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

		// Enable depth test.
	glEnable(GL_DEPTH_TEST);
	// Enable face culling.
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
}
void init(void)
{
	vertexShaderId = setShader((char*)"vertex", (char*)"triangles.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"triangles.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	modelID = glGetUniformLocation(program, "mvp");
	
	resetView();

	loadTexture();

	createBuffer();

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0); // black background
}

//---------------------------------------------------------------------
//
// calculateView
//
void calculateView()
{
	frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec.y = sin(glm::radians(pitch));
	frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec = glm::normalize(frontVec);
	rightVec = glm::normalize(glm::cross(frontVec, worldUp));
	upVec = glm::normalize(glm::cross(rightVec, frontVec));

	view = glm::lookAt(position, position + frontVec, upVec);
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

	// Update the projection or view if perspective.
	projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	calculateView();
	transformObject(0.4f, XYZ_AXIS, rotAngle -= 5, glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Now for a potentially smoother render.
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
	switch (key)
	{
	case 'w':
		position += frontVec * moveSpeed;
		break;
	case 's':
		position -= frontVec * moveSpeed;
		break;
	case 'a':
		position += rightVec * moveSpeed;
		break;
	case 'd':
		position -= rightVec * moveSpeed;
		break;
	case ' ':
		resetView();
		break;
	}
}

void keyDownSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		pitch -= turnSpeed;
		break;
	case GLUT_KEY_DOWN:
		pitch += turnSpeed;
		break;
	case GLUT_KEY_LEFT:
		yaw += turnSpeed;
		break;
	case GLUT_KEY_RIGHT:
		yaw -= turnSpeed;
		break;
	}
}

void mouseMove(int x, int y)
{
	//cout << "Mouse pos: " << x << "," << y << endl;
	if (mouseClicked)
	{
		pitch += (GLfloat)((y - lastY) * 0.1);
		yaw -= (GLfloat)((x - lastX) * 0.1);
		lastY = y;
		lastX = x;
	}
}

void mouseClick(int btn, int state, int x, int y)
{
	/*cout << "Clicked: " << (btn == 0 ? "left " : "right ") << (state == 0 ? "down " : "up ") <<
		"at " << x << "," << y << endl;*/
	if (state == 0)
	{
		lastX = x;
		lastY = y;
		mouseClicked = true;
		glutSetCursor(GLUT_CURSOR_NONE);
		cout << "Mouse clicked." << endl;
	}
	else
	{
		mouseClicked = false;
		glutSetCursor(GLUT_CURSOR_INHERIT);
		cout << "Mouse released." << endl;
	}
}

void clean()
{
	cout << "Cleaning up!" << endl;
	glDeleteTextures(1, &textureID);
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("Dice Demo");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.
	init();

	// Set all our glut functions.
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(33, timer, 0);
	glutKeyboardFunc(keyDown);
	glutSpecialFunc(keyDownSpecial);
	glutMouseFunc(mouseClick);
	//glutPassiveMotionFunc(mouseMove); // or...
	glutMotionFunc(mouseMove); // Requires click to register.
	atexit(clean); // This GLUT function calls specified function before terminating program. Useful!
	glutMainLoop();
}
