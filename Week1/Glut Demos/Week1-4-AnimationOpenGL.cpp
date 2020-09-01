//In this Demo, if users key in the escape button or rigth mouse button, the application stops

#include <GL/freeglut.h> 

float x_position = 0.0f;
int direction = 1;

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);


	// Draw a square with specified vertices.
	glBegin(GL_POLYGON);

	//By default the color state variable in OpenGL is white if you comment this out
	//glColor3f(1.0, 0.0, 0.0);
	glVertex2f(x_position, 0.1);

	//glColor3f(0.0, 1.0, 0.0);
	glVertex2f(x_position, -0.1);

	//glColor3f(0.0, 0.0, 1.0);
	glVertex2f(x_position+0.2, -0.1);

	//glColor3f(0.0, 0.0, 1.0);
	glVertex2f(x_position+0.2, 0.1);

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

void timer(int)
{
	glutPostRedisplay(); //call the drawScene
	glutTimerFunc(1000/60, timer, 0); //ine one second, this function calls itself 60 times

	switch (direction)
	{
	case 1:

		if (x_position < 0.8)
		{
			x_position += 0.005;
		}
		else
		{
			direction = -1;
		}
		break;

	case -1:
		if (x_position >= -1.0)
		{
			x_position -= 0.005;
		}
		else
		{
			direction = 1;
		}
		break;
	}
}

// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Simple Animation");

	glutDisplayFunc(drawScene);

	glutKeyboardFunc(keyInput);
	glutMouseFunc(MouseInput);

	//background color (by default) is black if you comment this out
	glClearColor(1.0, 0.0, 1.0, 0.0);

	glutTimerFunc(1000, timer, 0); //every second
	glutMainLoop();
}