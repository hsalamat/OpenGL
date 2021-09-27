/////////////////////////////////////////////////////////////////////////////////////
// GamepadGLFW.cpp
//
//The joystick functions provide unlabeled axes, buttons and hats, with no indication of where they are located on the device. Their order may also vary between platforms even with the same device.
//
//GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun 	callback)
//Parameters
//[in]	callback	The new callback, or NULL to remove the currently set callback.
//Returns
//The previously set callback, or NULL if no callback was set or the library had not been initialized.
//Callback signature
//void function_name(int jid, int event)
//https://www.glfw.org/docs/3.3/group__input.html#ga2f60a0e5b7bd8d1b7344dc0a7cb32b4c
//Hooman
/////////////////////////////////////////////////////////////////////////////////////

using namespace std;
#include <iostream>

#include <GL/glew.h>  
#include <GLFW/glfw3.h> // Include GLFW header.

#include "stdlib.h"
#include "time.h" 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <string>

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

GLuint vao, ibo, points_vbo, colors_vbo, modelID;
float rotAngle = 0.0f;
int deltaTime, currentTime, lastTime = 0;
glm::vec3 axis(1, 1, 0);
glm::vec3 translation(0.0f, 0.0f, 0.0f);
float scale = 1.0f;
float translationX = 0.0f, translationY = 0.0f, translationZ = 0.0f;

static unsigned int
program,
vertexShaderId,
fragmentShaderId;

// Globals.
GLFWwindow* window; // Handle to the OpenGL window created by GLFW.
int w, h; // Window width, height.


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

	modelID = glGetUniformLocation(program, "model");

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

// Enable depth test.
	glEnable(GL_DEPTH_TEST);
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

	// Delta time stuff.
	currentTime = glfwGetTime(); // Gets delta time in seconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	deltaTime = 30.0f;

	glBindVertexArray(vao);


	translation = glm::vec3(translationX * deltaTime / (float)1000, translationY * deltaTime / (float)1000, translationZ * deltaTime / (float)1000);

	scale = 0.2f;

	transformObject(scale, axis, rotAngle = 45.0f, translation);
	//Ordering the GPU to start the pipeline
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0); // Try GL_LINE_STRIP too!

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glfwSwapBuffers(window); // Swap buffers. 
							 // See comments about double-buffering in main() below.
}

// Keyboard input processing routine.
void keyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS || GLFW_REPEAT) // If escape is pressed or kept pressed, then ...
		glfwSetWindowShouldClose(window, GLFW_TRUE); // ... set close alert.	
}

//the callback is not working....
void gamepadInput(int jid, int event)
{
	cout << "test" << endl;
	GLFWgamepadstate state;

	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
	{
		if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
		{
			cout << "button A is pressed " << endl;
		}

		cout << "speed " << state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] << endl;
	}
}


// Main routine.
int main(int argc, char** argv)
{
	glfwInit(); // Initialize GLFW.

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); // Client API is OpenGL.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // OpenGL window is resizable.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL major version number.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL minor version number.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // OpenGL compatiblity profile.
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // Double buffering. This is the default which some
												  // systems may not even allow to be turned off
												  // (say, to get single-buffering as in square.cpp). 
												  // Therefore, buffer swapping may be required
												  // as in drawScene() above.

	window = glfwCreateWindow(1024, 1024, "RotatingCubeGLFW.cpp", NULL, NULL); // Create OpenGL window and return
																	   // a handle to the window.

	glfwSetWindowPos(window, 100, 100); // Set window position.
	glfwMakeContextCurrent(window); // Sets the OpenGL context of the window as current.
	glfwSetWindowSizeCallback(window, resize); // Register window resize callback function.
	glfwSetKeyCallback(window, keyInput); // Register key input callback function.
	glfwSetJoystickCallback(gamepadInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glfwGetWindowSize(window, &w, &h); // Obtain window size.
	resize(window, w, h); // Initial call to resize() which is not automatically
						  // instigated by window creation as in FreeGLUT.

	const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);
	cout << "Your Joystick/Gamepad is called :" << name << endl;

	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	cout << "Joystick/Gamepad 1 is connected" << present << endl;
	int axesCount;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

	cout << "Number of axes available: " << axesCount << endl;

	const unsigned char* buttons = nullptr;

	if (1 == present)
	{

		cout << "Left Analog Stick X Axis: " << axes[0] << endl;
		cout << "Left Analog Stick Y Axis: " << axes[1] << endl;
		cout << "Right Analog Stick X Axis: " << axes[2] << endl;
		cout << "Right Analog Stick Y Axis: " << axes[3] << endl;
		cout << "Left Trigger/L2: " << axes[4] << endl;
		cout << "Right Trigger/R2: " << axes[5] << endl;
		int buttonCount;
		buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		cout << "Number of buttons available: " << buttonCount << endl;

	}
	
	while (!glfwWindowShouldClose(window)) // Loop while window close alert is not set.
	{		
			if (GLFW_PRESS == buttons[1])
			{
				cout << "B button pressed" << endl;
			}
			///else if (GLFW_RELEASE == buttons[1])
			///{
			///	cout << "B button released" << endl;
			///}

			GLFWgamepadstate state;

			if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
			{
				if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
				{
					cout << "button A is pressed " << endl;
				}

				if (glm::abs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]) > 0.0001)
				{
					cout << "Left Analog Stick X Axis speed: " << state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] << endl;
					translationX += state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
				}
					
				if (glm::abs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]) > 0.0001)
				{
					cout << "Left Analog Stick Y Axis speed: " << state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] << endl;
					translationY -= state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
				}
				
				 
			


			}

		glfwPollEvents(); // Process events in queue.
		drawScene(); // Redraw scene.
	}
	glfwTerminate(); // Destroy window and free GLFW resources.
}