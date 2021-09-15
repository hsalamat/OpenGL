///////////////////////////////////
// testGlut.cpp
//
// OpenGL program to draw a square.
// This tutorial will rely on the fixed function pipeline instead of the programmable one.
// Hooman
///////////////////////////////////

#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <iostream>
using namespace std;

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	static GLclampf c = 0.0f;
	glColor3f(c, c, c);
	c += 1.0f / 256.0f;
	if (c >= 1.0) {
		c = 0.0f;
	}
	// The next iteration through glutMainLoop, the window's display callback will be called
	//to redisplay the window's normal plane
	glutPostRedisplay();

	// Draw a polygon with specified vertices.
	glBegin(GL_POLYGON);
	glVertex3f(20.0, 20.0, 0.0);
	glVertex3f(80.0, 20.0, 0.0);
	glVertex3f(80.0, 80.0, 0.0);
	glVertex3f(20.0, 80.0, 0.0);
	glEnd();

	//glFlush();  // single buffer
	glutSwapBuffers(); //double buffer
}

// Initialization routine.
void setup(void)
{
	//here we call our first opengl function to set the background color
	GLclampf Red = 1.0f, Green = 1.0f, Blue = 1.0f, Alpha = 0.0f;
	glClearColor(Red, Green, Blue, Alpha);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

// Main routine.
int main(int argc, char** argv)
{
	//Every library has be initialized before it's been used. This call initializes GLUT.The parameters can be provided directly from 
	//the command lineand include useful option such as '-gldebug' 
	//which  automatically checks for GL errors and displays them(respectively).
	glutInit(&argc, argv);

	//set up an OpenGL context of version 4.3 with a compatibility profile. 
	//Developers use the combination of an OpenGL context and profile to declare 
	//the program's hardware compatibility and whether deprecated APIs are present or allowed in this program.
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	int win = glutCreateWindow("square.cpp");
	cout << "window id: " << win << endl;

	//Since we are working in a windowing system most of the interaction with the running 
	//program occurs via event callback functions. GLUT takes care of interacting 
	//with the underlying windowing system and provides us with a few callback options. 
	//Here we use just one - a "main" callback to do all the rendering of one frame. 
	//This function is continuously called by GLUT internal loop.
	glutDisplayFunc(drawScene);


	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;

	//Must be done after glut is initialized
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		cout << "Error " << glewGetErrorString(res);
	}

	setup();

	//This call passes control to GLUT which now begins its own internal loop.
	//In this loop it listens to events from the windowing system and passes them via 
	//the callbacks that we configured.In our case GLUT will only call the function 
	//we registered as a display callback(drawScene) to give us a chace to render the frame.
	glutMainLoop();

	return 0;
}