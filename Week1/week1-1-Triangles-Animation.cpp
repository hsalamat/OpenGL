
///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//Hooman Salamat
//In this tutorial we will see the usage of vertex buffer objects (VBOs) for the first time. 
//As the name implies, they are used to store vertices. The objects that exist in the 3D world you are trying to visualize, be it monsters, castles or a simple revolving cube, are always built by connecting together a group of vertices. 
//VBOs are the most efficient way to load vertices into the GPU. 
//They are buffers that can be stored in video memory and provide the shortest access time to the GPU so they are definitely recommended.
// Before reaching the rasterizer (that actually draws points, lines and triangles using screen coordinates) 
//the visible vertices have their X, Y and Z coordinates in the range [-1.0,1.0]. 
//The rasterizer maps these coordinates to screen space (e.g, if the screen width is 1024 then the X coodinate -1.0 is mapped to 0 and 1.0 is mapped to 1023). Finally, the rasterizer draws the primitives according to the topology which is specified in the draw call 

////http://glew.sourceforge.net/
//The OpenGL Extension Wrangler Library (GLEW) is a cross-platform open-source C/C++ extension loading library. 
//GLEW provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target
//platform. OpenGL core and extension functionality is exposed in a single header file. GLEW has been tested on a 
//variety of operating systems, including Windows, Linux, Mac OS X, FreeBSD, Irix, and Solaris.
//
//http://freeglut.sourceforge.net/
//The OpenGL Utility Toolkit(GLUT) is a library of utilities for OpenGL programs, which primarily perform system - level I / O with the host operating system.
//Functions performed include window definition, window control, and monitoring of keyboardand mouse input.
//Routines for drawing a number of geometric primitives(both in solid and wireframe mode) are also provided, including cubes, spheresand the Utah teapot.
//GLUT also has some limited support for creating pop - up menus..

//OpenGL functions are in a single library named GL (or OpenGL in Windows). Function names begin with the letters glSomeFunction*();
//Shaders are written in the OpenGL Shading Language(GLSL)
//To interface with the window system and to get input from external devices into our programs, we need another library. For the XWindow System, this library is called GLX, for Windows, it is wgl,
//and for the Macintosh, it is agl. Rather than using a different library for each system,
//we use two readily available libraries, the OpenGL Extension Wrangler(GLEW) and the OpenGLUtilityToolkit(GLUT).
//GLEW removes operating system dependencies. GLUT provides the minimum functionality that should be expected in any modern window system.
//OpenGL makes heavy use of defined constants to increase code readability and avoid the use of magic numbers.Thus, strings such as GL_FILL and GL_POINTS are defined in header(#include <GL/glut.h>)

//https://glm.g-truc.net/0.9.9/index.html
////OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
///////////////////////////////////////////////////////////////////////

using namespace std;

#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <string>
#include <iostream>

using namespace std; 

GLint gScaleLocation;

GLuint VBO[1];

const GLuint NumVertices = 3;
const GLfloat scale = 0.5f;
GLfloat vertices[NumVertices][2] = {
		{0.0, 0.0},
		{0.5,0.5},
		{-0.5, 0.5},
};


//const is a contract. By receiving a const int * parameter, you "tell" the caller that you (the called function) will not modify the objects the pointer points to.
//const can also be used by compiler to perform optimization it couldn't otherwise
//void * is used in C for "generic" data

#define BUFFER_OFFSET(x)  ((const void*) (x))

static unsigned int program, vertexShaderId, fragmentShaderId;


void init(void)
{

	// Create shader program executable.

	vertexShaderId = setShader((char*)"vertex", (char*)"vertexanim2.shader");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cube.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);

	glLinkProgram(program);

	GLint Success;
	glGetProgramiv(program, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		char temp[1024];
		glGetProgramInfoLog(program, 1024, 0, temp);
		fprintf(stderr, "Failed to link program:\n%s\n", temp);
		glDeleteProgram(program);
		program = 0;
		exit(EXIT_FAILURE);
	}


	//you have to do this after linking the shader program
	gScaleLocation = glGetUniformLocation(program, "gScale");
	if (gScaleLocation == -1) {
		cout << "Error getting uniform location of 'gScale'" << endl;
		exit(1);
	}


	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &Success);
	if (Success == 0) {
		char temp[1024];
		glGetProgramInfoLog(program, 1024, 0, temp);
		fprintf(stderr, "Invalid Shader program:\n%s\n", temp);
		glDeleteProgram(program);
		program = 0;
		exit(EXIT_FAILURE);
	}
	glUseProgram(program);

	//Generating two buffers, one is used to store the coordinates of the vertices
	//The other one is not used. Just wanted to show that we can allocate as many as buffers, some of which might left unused.

	//OpenGL defines several glGen* functions for generating objects of various types. They often take two parameters - the first one specifies the number of objects you want to create and the second is the address of an array of GLuints to store the handles that the driver allocates for you (make sure the array is large enough to handle your request!). Future calls to this function will not generate the same object handles unless you delete them first with glDeleteBuffers. Note that at this point you don't specify what you intend to do with the buffers so they can be regarded as "generic".
	glGenBuffers(2, VBO);

	//OpenGL has a rather unique way of using handles. In many APIs the handle is simply passed to any relevant function and the action is taken on that handle. In OpenGL we bind the handle to a target name and then execute commands on that target. These commmands affect the bounded handle until another one is bound in its stead or the call above takes zero as the handle. 
	//The target GL_ARRAY_BUFFER means that the buffer will contain an array of vertices. 
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	glBindAttribLocation(program, 0, "vPosition");

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	

}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);


	static float Scale = 0.0f;
	static float Delta = 0.005f;

	Scale += Delta;
	if ((Scale > 1.0f) || (Scale < -1.0f))
	{
		Delta *= -1.0f;
	}

	glUniform1f(gScaleLocation, Scale);


	//what if you jusy wanted to see just edges and not a filled polygon, then uncomment this
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//we are enabling the vertex attibute zero (this is the index for our only attribute: vertex position)
	glEnableVertexAttribArray(0);

	//Selecting the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);


	//Pushing the coordinates of the vertices into the buffer
	//After binding our object we fill it with data. The call above takes the target name (same as what we used for binding), the size of the data in bytes, address of the array of vertices and a flag that indicates the usage pattern for this data. Since we are not going to change the buffer contents we specify GL_STATIC_DRAW. The opposite will be GL_DYNAMIC_DRAW. While this is only a hint to OpenGL it is a good thing to give some thought as to the proper flag to use. The driver can rely on it for optimization heuristics (such as what is the best place in memory to store the buffer).
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//It is good practice to disable each vertex attribute when it is not immediately used.Leaving it enabled when a shader is not using it is a sure way of asking for trouble.
	glDisableVertexAttribArray(0);
	
	//glFlush();
	glutSwapBuffers();
}


void idle()
{
	glutPostRedisplay();
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


int main(int argc, char** argv)
{
	//Before we can open a window, theremust be interaction between the windowing systemand OpenGL.In GLUT, this interaction is initiated by the following function call :
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	//if you comment out this line, a window is created with a default size
	glutInitWindowSize(512, 512);

	//the top-left corner of the display
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Hello Animation");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	//clulling clockwise triangles(back facing!)
	glEnable(GL_CULL_FACE); 
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	//If there are events in the queue, our program responds to them through functions
	//called callbacks.A callback function is associated with a specific type of event.
	//A display callback is generated when the application program or the
	//operating system determines that the graphics in a window need to be redrawn.
	glutDisplayFunc(display);

	glutIdleFunc(idle);
	glutKeyboardFunc(keyInput);

	//begin an event-processing loop
	glutMainLoop();
}
