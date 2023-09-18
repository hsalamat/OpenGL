
///////////////////////////////////////////////////////////////////////
//
// trianglewithcolor.cpp
// This tutorial demonstrates a very important part of the 3D pipeline - the interpolation that 
// the rasterizer performs on variables that come out of the vertex shader. 
// As you have already seen, in order to get something meaningful on the screen you need to designate 
// one of the VS output variables as 'gl_Position'. This is a 4-vector that contains 
// the homogenuous coordinates of the vertex. The XYZ components of that vector are divided 
// by the W component (a process known as perspective divide ) and any component which goes outside the normalized box ([-1,1]) gets clipped. 
// The result is transformed to screen space coordinates and then the triangle (or any other supported primitive type) is rendered to screen by the rasterizer.
//The rasterizer performs interpolation between the three triangle vertices(either going line by line or any other technique) 
// and "visits" each pixel inside the triangle by executing the fragment shader.The fragment shader is 
// expected to return a pixel color which the rasterizer places in the color buffer for display(after passing a few additional tests like depth test, etc).
// Any other variable which comes out of the vertex shader does not go through the steps above.
// If the fragment shader does not explicitly requests that variable(and you can mix and match multiple fragment shaders with the same vertex shader) 
// then a common driver optimization will be to drop any instructions in the VS that only affect this variable
// (for that particular shader program that combines this VSand FS pair).
// However, if the FS does use that variable the rasterizer interpolates it during rasterization and 
// each FS invocation is provided as the interpolated value that matches that specific location.
// This usually means that the values for pixels that are right next to each other will be a 
// bit different(though as the triangle becomes further and further away from the camera 
// that becomes less likely).
//Hooman Salamat

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



GLuint VBO[1];

const GLuint NumVertices = 3;
const GLfloat scale = 0.5f;
GLfloat vertices[NumVertices][2] = {
		{0.0, 0.0},
		{0.5,0.5},
		{-0.5, 0.5},
};


//const is a contract. By receiving a const int * parameter, 
// you "tell" the caller that you (the called function) will not modify the objects the pointer points to.
//const can also be used by compiler to perform optimization it couldn't otherwise
//void * is used in C for "generic" data

#define BUFFER_OFFSET(x)  ((const void*) (x))

static unsigned int program, vertexShaderId, fragmentShaderId;


void init(void)
{

	// Create shader program executable.

	vertexShaderId = setShader((char*)"vertex", (char*)"trianglewithcolor.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"trianglewithcolor.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
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
	//glutPostRedisplay();
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

	glutCreateWindow("Hello Color Triangle");

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
