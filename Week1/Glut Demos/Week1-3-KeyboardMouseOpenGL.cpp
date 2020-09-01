//In this Demo, if users key in the escape button or rigth mouse button, the application stops

#include <GL/freeglut.h> 

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);


	// Draw a triangle with specified vertices.
	glBegin(GL_TRIANGLES);

	//By default the color state variable in OpenGL is white if you comment this out
	//glColor3f(1.0, 0.0, 0.0);
	glVertex2f(-0.5, -0.5);

	//glColor3f(0.0, 1.0, 0.0);
	glVertex2f(0.0, 0.5);

	//glColor3f(0.0, 0.0, 1.0);
	glVertex2f(0.5, -0.5);
	glEnd();

	glFlush();
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

void MouseInput(int button, int state, int x, int y)
{
	if(button == GLUT_RIGHT_BUTTON)
	{
		exit(0);
	}
}

// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Draw a Triangle");

	glutDisplayFunc(drawScene);

	glutKeyboardFunc(keyInput);
	glutMouseFunc(MouseInput);

	//background color (by default) is black if you comment this out
	glClearColor(1.0, 0.0, 1.0, 0.0);

	glutMainLoop();
}