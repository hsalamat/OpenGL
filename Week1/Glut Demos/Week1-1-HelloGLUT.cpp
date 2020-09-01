//freeglut is a free - software / open - source alternative to the OpenGL Utility Toolkit(GLUT) library.GLUT was originally written by Mark Kilgard to support the sample programs in the second edition OpenGL 'RedBook'.Since then, GLUT has been used in a wide variety of practical applications because it is simple, widely availableand highly portable.
//GLUT(and hence freeglut) takes care of all the system - specific chores required for creating windows, initializing OpenGL contexts, and handling input events, to allow for trully portable OpenGL programs.
//FreeGLUT 64 bit: Download and unzip  freeglut-MSVC-3.0.0-2.mp  from https://www.transmissionzero.co.uk/software/freeglut-devel/ and save the folder C:\OpenGLwrappers  in OpenGLwrappers
//Under Project Properties page: C/C++ : Additional Include Directories --> C:\OpenGLwrappers\freeglut\include
//Copy freeglut.dll from C:\OpenGLwrappers\freeglut\bin\x64 to C:\Windows\ system32.
//Click Configuration Properties->Linker->General->Additional Library Directories, add C:\OpenGLwrappers\freeglut\lib\x64 
//Click Configuration Properties->Linker->Input->Additional Dependencies add freeglut.lib and opengl32.lib

#include <GL/freeglut.h> 

void drawScene()
{

}

// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);


	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Draw Nothing");

	glutDisplayFunc(drawScene);

	glutMainLoop();
}
