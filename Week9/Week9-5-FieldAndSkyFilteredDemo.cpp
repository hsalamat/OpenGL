////////////////////////////////////////////////////////////////////////////////////////
// fieldAndSkyFiltered.cpp
//
// This program is based on fieldAndSky.cpp. In addition to the viewpoint being movable, 
// the grass texture filter can be set at different levels.
//
// Interaction:
// Press the up and down arrow keys to move the viewpoint over the field.
// Press the left and right arrow keys to cycle through the filters.
//
// Hooman Salamat
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

static float d = 0.0; // Distance parameter in gluLookAt().
static int filter = 0; // Filter id.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.


GLshort cube_indices[] = {
	// Front.
	0, 3, 2,
	2, 0, 1
};

GLfloat grass_vertices[] = {
	//	x,		y,		z,	  u,	v
		-100.0f, 0.0f, 100.0f,	0.0f, 0.0f,		// 0.
		100.0f,  0.0f, 100.0f, 8.0f, 0.0f,		// 1.
		100.0f,   0.0f, -100.0f, 8.0f, 8.0f,		// 2.
		-100.0f,  0.0f, -100.0f, 0.0f, 8.0f		// 3.
};

GLfloat sky_vertices[] = {
	//	x,		y,		z,	  u,	v
		-100.0f, 0.0f, -70.0f,	0.0f, 0.0f,		// 0.
		100.0f,  0.0f, -70.0f, 1.0f, 0.0f,		// 1.
		100.0f,   120.0f, -70.0f, 1.0f, 1.0f,		// 2.
		-100.0f,  120.0f, -70.0f, 0.0f, 1.0f		// 3.
};

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


// Routine to draw a bitmap character string.
void writeBitmapString(void* font, char* string)
{
	char* c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Write messages.
void writeData(void)
{
	switch (filter)
	{
	case 0:
		glRasterPos3f(-30, 45, -5.1); writeBitmapString((void*)font, (char*)"Grass mag filter: GL_NEAREST");
		glRasterPos3f(-30, 40, -5.1); writeBitmapString((void*)font, (char*)"Grass min filter: GL_NEAREST");
		break;
	case 1:
		glRasterPos3f(-30, 45, -5.1); writeBitmapString((void*)font, (char*)"Grass mag filter: GL_LINEAR");
		glRasterPos3f(-30, 40, -5.1); writeBitmapString((void*)font, (char*)"Grass min filter: GL_LINEAR");
		break;
	case 2:
		glRasterPos3f(-30, 45, -5.1); writeBitmapString((void*)font, (char*)"Grass mag filter: GL_LINEAR");
		glRasterPos3f(-30, 40, -5.1); writeBitmapString((void*)font, (char*)"Grass min filter: GL_NEAREST_MIPMAP_NEAREST");
		break;
	case 3:
		glRasterPos3f(-30, 45, -5.1); writeBitmapString((void*)font, (char*)"Grass mag filter: GL_LINEAR");
		glRasterPos3f(-30, 40, -5.1); writeBitmapString((void*)font, (char*)"Grass min filter: GL_LINEAR_MIPMAP_NEAREST");
		break;
	case 4:
		glRasterPos3f(-30, 45, -5.1); writeBitmapString((void*)font, (char*)"Grass mag filter: GL_LINEAR");
		glRasterPos3f(-30, 40, -5.1); writeBitmapString((void*)font, (char*)"Grass min filter: GL_NEAREST_MIPMAP_LINEAR");
		break;
	case 5:
		glRasterPos3f(-30, 45, -5.1); writeBitmapString((void*)font, (char*)"Grass mag filter: GL_LINEAR");
		glRasterPos3f(-30, 40, -5.1); writeBitmapString((void*)font, (char*)"Grass min filter: GL_LINEAR_MIPMAP_LINEAR");
		break;
	default:
		break;
	}
}



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

void setFilter()
{
	switch (filter)
	{
	case 0:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case 1:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case 2:
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case 3:
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case 4:
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case 5:
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	default:
		break;
	}

}

void loadGrassTexture()
{
	stbi_set_flip_vertically_on_load(true);

	// Bind grass image to texture object texture0. 
	unsigned char* image0 = stbi_load("Media/grass.bmp", &width, &height, &bitDepth, 0);
	if (!image0) {
		cout << "Unable to load file!" << endl;
		// Could add a return too if you modify init.
	}

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	setFilter();


	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Clean up. But we don't want to unbind the texture or we cannot use it.
	stbi_image_free(image0);

}

void loadSkyTexture()
{
	stbi_set_flip_vertically_on_load(true);

	// Bind grass image to texture object texture1. 
	unsigned char* image0 = stbi_load("Media/sky.bmp", &width, &height, &bitDepth, 0);
	if (!image0) {
		cout << "Unable to load file!" << endl;
		// Could add a return too if you modify init.
	}

	// Bind sky image to texture object texture1
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	setFilter();

	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Clean up. But we don't want to unbind the texture or we cannot use it.
	stbi_image_free(image0);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vertices), grass_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(grass_vertices[0]) * 5, 0);
	glEnableVertexAttribArray(0);

	// Now for the UV/ST values.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(grass_vertices[0]) * 5, (void*)(sizeof(grass_vertices[0]) * 3));
	glEnableVertexAttribArray(2);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.


	vao = 1;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sky_vertices), sky_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sky_vertices[0]) * 5, 0);
	glEnableVertexAttribArray(0);

	// Now for the UV/ST values.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sky_vertices[0]) * 5, (void*)(sizeof(sky_vertices[0]) * 3));
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
	view = glm::lookAt(
		glm::vec3(0, 10.0, 15.0 + d),		// Camera pos in World Space
		glm::vec3(0.0, 10.0, d),		// and looks at the origin
		glm::vec3(0.0, 1.0, 0.0)		// Head is up (set to 0,-1,0 to look upside-down)
	);
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

	glBindVertexArray(1);
	loadGrassTexture();
	// Update the projection or view if perspective.
	projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	calculateView();
	transformObject(0.4f, YZ_AXIS, rotAngle -= 0, glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.


	glBindVertexArray(2);
	loadSkyTexture();
	// Update the projection or view if perspective.
	projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	calculateView();
	transformObject(0.4f, YZ_AXIS, rotAngle -= 0, glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	// Write data.
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	writeData();

	glutSwapBuffers(); // Now for a potentially smoother render.
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) d -= 0.1;
	if (key == GLUT_KEY_DOWN) d += 0.1;
	if (key == GLUT_KEY_RIGHT)
	{
		filter++;
		if (filter == 6) filter = 0;
	}
	if (key == GLUT_KEY_LEFT)
	{
		filter--;
		if (filter == -1) filter = 5;
	}
	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press the up and down arrow keys to move the viewpoint over the field." << std::endl
		<< "Press the left and right arrow keys to cycle through the filters." << std::endl;
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("fieldAndSkyFiltered.cpp");
	glewInit();
	glutDisplayFunc(display);
	//glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutMainLoop();
}
