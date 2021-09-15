/////////////////////////////////////////////////////////////////////////////////////
// testGLFWWithShader.cpp
// GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop. 
// It provides a simple API for creating windows, contexts and surfaces, receiving input and events.
// This program is rewritten to use GLFW to manage the window
// instead of FreeGLUT. All GLFW calls are commentated.
//
//Hooman
/////////////////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>  
#include <GLFW/glfw3.h> // Include GLFW header.

#include "stdlib.h"
#include "time.h" 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <string>

#define BUFFER_OFFSET(x)  ((const void*) (x))

GLuint VBO[1];

const GLuint NumVertices = 3;

GLfloat vertices[NumVertices][2] = {
		{0.0, 0.0},
		{0.5,0.5},
		{-0.5, 0.5}
};

static unsigned int
program,
vertexShaderId,
fragmentShaderId;

// Globals.
GLFWwindow* window; // Handle to the OpenGL window created by GLFW.
int w, h; // Window width, height.

GLFWwindow* window2;

// Initialization routine.
void setup(void)
{
	// Create shader program executable.

	vertexShaderId = setShader((char*)"vertex", (char*)"cube.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cube.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	//Generating two buffers, one is used to store the coordinates of the vertices
	//The other one is not used. Just wanted to show that we can allocate as many as buffers, some of which might left unused.
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
}




// OpenGL window reshape routine.
void resize(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	//what if you jusy wanted to see just edges and not a filled polygon, then uncomment this
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Selecting the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);


	//Pushing the coordinates of the vertices into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glfwSwapBuffers(window); // Swap buffers. 
							 // See comments about double-buffering in main() below.
}

// Keyboard input processing routine.
void keyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS || GLFW_REPEAT) // If escape is pressed or kept pressed, then ...
		glfwSetWindowShouldClose(window, GLFW_TRUE); // ... set close alert.
}

// Main routine.
int main(int argc, char** argv)
{
	glfwInit(); // Initialize GLFW.

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); // Client API is OpenGL.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // OpenGL window is resizable.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL major version number.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // OpenGL minor version number.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // OpenGL compatiblity profile.
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // Double buffering. This is the default which some systems may not even allow to be turned off


	window = glfwCreateWindow(1024, 1024, "testGLFW.cpp", NULL, NULL); // Create OpenGL window and return
																	   // a handle to the window.

	//I am just creating another window to have my non opengl stuff there.
	//window2 = glfwCreateWindow(1024, 1024, "None OpenGL", NULL, NULL); // Create a None OpenGL window and return

	glfwSetWindowPos(window, 100, 100); // Set window position.
	glfwMakeContextCurrent(window); // Sets the OpenGL context of the window as current.
	glfwSetWindowSizeCallback(window, resize); // Register window resize callback function.
	glfwSetKeyCallback(window, keyInput); // Register key input callback function.

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glfwGetWindowSize(window, &w, &h); // Obtain window size.
	resize(window, w, h); // Initial call to resize() which is not automatically
						  // instigated by window creation as in FreeGLUT.

	while (!glfwWindowShouldClose(window)) // Loop while window close alert is not set.
	{
		glfwPollEvents(); // Process events in queue.
		drawScene(); // Redraw scene.
	}
	glfwTerminate(); // Destroy window and free GLFW resources.
}