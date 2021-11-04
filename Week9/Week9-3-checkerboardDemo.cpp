
///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////         
// TextureDemo2.cpp
// Checkerboard
// Hooman Salamat
///////////////////////////////////////////////////////////////////// 


//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//1111111100000000111111110000000011111111000000001111111100000000
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111
//0000000011111111000000001111111100000000111111110000000011111111



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

using namespace std;

// IDs.
GLuint vao, ibo, points_vbo, colors_vbo, modelID;

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
	0, 3, 2,
	2, 0, 1
};

GLfloat cube_vertices[] = {
	//	x,		y,		z,	  u,	v
		-1.0f, -1.0f, 1.0f,	0.0f, 0.0f,		// 0.
		1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,		// 1.
		1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		// 2.
		-1.0f,  1.0f, 1.0f, 0.0f, 1.0f		// 3.
};

//step2: try this - pay attention to texture coordinate: (u,v) = (0.5f, 1.0f)
//GLfloat cube_vertices[] = {
//	//	x,		y,		z,	  u,	v
//		-1.0f, -1.0f, 1.0f,	0.0f, 0.0f,		// 0.
//		1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,		// 1.
//		0.0f,   1.0f, 1.0f, 0.5f, 1.0f,		// 2.
//		//-1.0f,  1.0f, 1.0f, 0.0f, 1.0f		// 3.
//};

//step3: try this - pay attention to vertex coordinate: (x,y,z) = (1.0f,   2.0f, 1.0f)
//GLfloat cube_vertices[] = {
//	//	x,		y,		z,	  u,	v
//		-1.0f, -1.0f, 1.0f,	0.0f, 0.0f,		// 0.
//		1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,		// 1.
//		1.0f,   2.0f, 1.0f, 1.0f, 1.0f,		// 2.
//		-1.0f,  1.0f, 1.0f, 0.0f, 1.0f		// 3.
//};


//step4: the world-space polygon is mapped to a 3 X 2 rectangle in texture space
//GLfloat cube_vertices[] = {
//		x,		y,		z,	  u,	v
//		-1.0f, -1.0f, 1.0f,	-1.0f, 0.0f,	// 0.
//		1.0f,  -1.0f, 1.0f, 2.0f, 0.0f,	    // 1.
//		1.0f,   1.0f, 1.0f, 2.0f, 2.0f,		// 2.
//		-1.0f,  1.0f, 1.0f, -1.0f, 2.0f		// 3.
//};

GLfloat colors[] = {
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};

static unsigned int
program,
vertexShaderId,
fragmentShaderId;

static unsigned char chessboard[64][64][4]; // Storage for chessboard image.


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

// Create 64 x 64 RGBA image of a chessboard.
//j=0-7 black, j=8-15 white, j=16-23 black, j=24-31 white
void createChessboard(void)
{
	int i, j;
	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			if ((((i / 8) % 2) && ((j / 8) % 2)) || (!((i / 8) % 2) && !((j / 8) % 2)))
			{
				chessboard[i][j][0] = 0xFF;
				chessboard[i][j][1] = 0x00;
				chessboard[i][j][2] = 0x00;
				chessboard[i][j][3] = 0xFF;
			}
			else
			{
				chessboard[i][j][0] = 0xFF;
				chessboard[i][j][1] = 0xFF;
				chessboard[i][j][2] = 0xFF;
				chessboard[i][j][3] = 0xFF;
			}
}

void loadTexture()
{
    //we are making our own image
	//stbi_set_flip_vertically_on_load(true);

	//unsigned char* image = stbi_load("Media/spheremap.png", &width, &height, &bitDepth, 0);
	//if (!image) {
	//	cout << "Unable to load file!" << endl;
	//	// Could add a return too if you modify init.
	//}


	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	// Create texture object 
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Generate internal texture.
	createChessboard();

	// Specify image data for currently active texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
		64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, chessboard);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//step1-1: Comment out the line above. Change texture coordinates to be 3x2 (step4!)
	//This causes the wrapping mode to be clamped to the edge in the s - direction
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//step1-2: Comment out the line above.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set texture parameters for filtering. This causes the wrapping mode to be clamped to the edge in the t - direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Clean up. But we don't want to unbind the texture or we cannot use it.
	//stbi_image_free(image);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cube_vertices[0]) * 5, 0);
	glEnableVertexAttribArray(0);

	// Now for the UV/ST values.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(cube_vertices[0]) * 5, (void*)(sizeof(cube_vertices[0]) * 3));
	glEnableVertexAttribArray(2);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.
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

	glClearColor(0.0, 0.0, 1.0, 1.0); // blue background
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
	//projection = glm::ortho(-zoom + osH, zoom + osH, -zoom + osV, zoom + osV, 0.0f, 100.0f);
	projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	calculateView();
	transformObject(1.0f, YZ_AXIS, rotAngle -= 0, glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); // Try GL_LINE_STRIP too!

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
	glutCreateWindow("Texture Demo 1");

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
