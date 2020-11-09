//texture2D Array Demo

//When the 3 dimensional texture image is specified, 
//then the depth has to be the number of images 
//which have to be stored in the array(e.g.imageCount).
//The widthand the height parameter represent the width
//and height of 1 tile(e.g.tileW, tileH).The layer should be 0 and the border parameter has to be 0. See glTexImage3D.glTexImage3D creates the data store for the texture image.The memory 
//which is required for the textures is reserved(GPU).

// Hooman Salamat

#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
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

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
int NUM_IMAGES = 3;
int frame = 0;

GLfloat cube_vertices[NumVertices][3] = { 0 };
GLfloat cube_colors[NumVertices][3] = { 0 };
GLfloat cube_texCoords[NumVertices][2] = { 0 };

// IDs.
GLuint vao, ibo, points_vbo, colors_vbo, texCoords_vbo, modelID, layerID;

// Camera and transform variables.
glm::vec3 position, frontVec, worldUp, upVec, rightVec; // Set by function.
GLfloat pitch, yaw,
moveSpeed = 0.1f,
turnSpeed = 1.0f;

// Mouse variables.
bool mouseFirst = true, mouseClicked = false;
int lastX, lastY;

// Fixed timestep variables.
int deltaTime, currentTime, lastTime = 0;

// Matrices.
glm::mat4 mvp, view, projection;

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

// Texture variables.
GLuint textureID;
GLint width, height, bitDepth;

static unsigned int
program,
vertexShaderId,
fragmentShaderId;

std::array<glm::vec3, 36> vertices = {};
std::array<glm::vec3, 36> colors = {};
std::array<glm::vec2, 36> texCoords = {};

std::array<glm::vec3, 8> unique_vertices = {
	glm::vec3(-1.0f, -1.0f, 1.0f),		// 0.
	glm::vec3(1.0f, -1.0f, 1.0f),		// 1.
	glm::vec3(1.0f, 1.0f, 1.0f),		// 2.
	glm::vec3(-1.0f, 1.0f, 1.0f),		// 3.
	glm::vec3(-1.0f, -1.0f, -1.0f),	    // 4.
	glm::vec3(1.0f, -1.0f, -1.0f),		// 5.
	glm::vec3(1.0f, 1.0f, -1.0f),		// 6.
	glm::vec3(-1.0f, 1.0f, -1.0f)		// 7.
};


std::array<glm::vec3, 8> unique_colors = {
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

std::array<glm::vec2, 6> unique_texCoords = {
	glm::vec2(1.0f, 0.0f),
	glm::vec2(1.0f, 1.0f),
	glm::vec2(0.0f, 1.0f),
	glm::vec2(0.0f, 1.0f),
	glm::vec2(0.0f, 0.0f),
	glm::vec2(1.0f, 0.0f),
};



//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

void quad(int a, int b, int c, int d, int e, int f)
{
	texCoords[Index] = unique_texCoords[0];  colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[a]; Index++;
	texCoords[Index] = unique_texCoords[1]; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[b]; Index++;
	texCoords[Index] = unique_texCoords[2]; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[c]; Index++;
	texCoords[Index] = unique_texCoords[3]; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[d]; Index++;
	texCoords[Index] = unique_texCoords[4]; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[e]; Index++;
	texCoords[Index] = unique_texCoords[5]; colors[Index] = unique_colors[a]; vertices[Index] = unique_vertices[f]; Index++;

}
//----------------------------------------------------------------------------


void loadTexture()
{
	stbi_set_flip_vertically_on_load(false);
	unsigned char* image = stbi_load("Media/animation-array.jpg", &width, &height, &bitDepth, 0);
	//unsigned char* image = stbi_load("Media/wicker_256.jpg", &width, &height, &bitDepth, 0);
	if (!image) {
		cout << "Unable to load file!" << endl;
		// Could add a return too if you modify init.
	}

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, width,
		540, NUM_IMAGES, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//If the tiles are in the 16x16 atlas, then the tiles have to by extracted from the texture atlasand to set each subimage in the texture array. (data[i] is the imaged data of one tile).
	//for (int i = 0; i < NUM_IMAGES; ++i)
	//{
	//	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
	//		0, 0, i,
	//		width, height, 1,
	//		GL_RGBA, GL_UNSIGNED_BYTE, (const void *)image[i]);
	//}


	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	// Clean up. But we don't want to unbind the texture or we cannot use it.
	stbi_image_free(image);
}


// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
	quad(2, 1, 0, 0, 3, 2); //front

	for (int i = 0; i < NumVertices; ++i) {

		cube_vertices[i][0] = vertices[i][0];
		cube_vertices[i][1] = vertices[i][1];
		cube_vertices[i][2] = vertices[i][2];

		cube_colors[i][0] = colors[i][0];
		cube_colors[i][1] = colors[i][1];
		cube_colors[i][2] = colors[i][2];

		cube_texCoords[i][0] = texCoords[i][0];
		cube_texCoords[i][1] = texCoords[i][1];
	}
}

void createBuffer()
{
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

	texCoords_vbo = 0;
	glGenBuffers(1, &texCoords_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texCoords_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texCoords), cube_texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

}



void resetView()
{
	position = glm::vec3(0.0f, 0.0f, 5.0f);
	frontVec = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0, 1, 0);
	pitch = 0.0f;
	yaw = -90.0f;
}

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

void init(void)
{
	vertexShaderId = setShader((char*)"vertex", (char*)"texture2darray.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"texture2darray.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	colorcube();

	modelID = glGetUniformLocation(program, "mvp");
	layerID = glGetUniformLocation(program, "layer");

	resetView();
	createBuffer();
	loadTexture();

// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
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

	transformObject(1.0f, X_AXIS, rotAngle = 0.0 , glm::vec3(0.0f, 0.0f, 0.0f));
	//transformObject(0.4f, YZ_AXIS, rotAngle += ((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));

	glUniform1i(layerID, frame);

	frame = (frame + 1) % NUM_IMAGES;

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, 6); // Try GL_LINE_STRIP too!

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
	glutTimerFunc(330, timer, 0);
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
