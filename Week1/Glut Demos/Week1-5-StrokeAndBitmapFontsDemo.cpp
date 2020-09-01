//In this Demo, if users key in the escape button or rigth mouse button, the application stops
//GLUT supports two types of font rendering: stroke fonts, meaning each character is rendered as a set of line segments; and bitmap fonts, 
//where each character is a bitmap generated with glBitmap. 
//There are 7 bitmap fonts available in GLUT.They are
//GLUT_BITMAP_8_BY_13
//GLUT_BITMAP_9_BY_15
//GLUT_BITMAP_TIMES_ROMAN_10
//GLUT_BITMAP_TIMES_ROMAN_24
//GLUT_BITMAP_HELVETICA_10
//GLUT_BITMAP_HELVETICA_12
//GLUT_BITMAP_HELVETICA_18

//Stroke fonts have the advantage that because they are geometry, they can be arbitrarily scale and rendered. 
//Bitmap fonts are less ﬂexible since they are rendered as bitmaps but are usually faster than stroke fonts. 
//You can render the character in any raster position of the window.The raster position can be set using family of functions glRasterPos from OpenGL library.The syntax of the functions are :
//void glRasterPos2f(float x, float y);
//void glRasterPos3f(float x, float y, float z);

#include <GL/freeglut.h> 
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

float x_position = 0.0f, y_position = 0.0f, z_position = 0.0f;
int direction = 1;
static long font = (long)GLUT_BITMAP_8_BY_13; // Text font.

// Routine to draw a stroke character string.
void writeStrokeString(void* font, char* string)
{
	char* c;

	for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

// Routine to draw a bitmap character string.
void writeBitmapString(void* font, char* string)
{
	char* c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}


void intToString(char* destStr, int val)
{ 
	sprintf(destStr, "%d", val);
}

// Write data.
void writeData(void)
{
	char buffer[33];
	glPushMatrix();
	glTranslatef(-1.0, 0.7, 0.0);
	glScalef(0.001, 0.001, 0.001);
	writeStrokeString(GLUT_STROKE_ROMAN, (char*)"Stroke & Bitmap Fonts Demo");
	glPopMatrix();


	sprintf(buffer, "%f", x_position);
	glRasterPos3f(-1.0, 0.6, 0.0);
	writeBitmapString((void*)font, (char*)"x = ");
	writeBitmapString((void*)font, buffer);

	sprintf(buffer, "%f", y_position);
	glRasterPos3f(-1.0, 0.5, 0.0);
	writeBitmapString((void*)font, (char*)"y = ");
	writeBitmapString((void*)font, buffer);

	sprintf(buffer, "%f", z_position);
	glRasterPos3f(-1.0, 0.4, 0.0);
	writeBitmapString((void*)font, (char*)"z = ");
	writeBitmapString((void*)font, buffer);
}

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glColor3f(1, 1, 0);
	
	writeData();

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

	glutSwapBuffers();
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

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

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