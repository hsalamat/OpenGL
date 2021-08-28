
///////////////////////////////////////////////////////////////////////
//
// Tetrahedron3.cpp
//Adding color attribute
//notice that I am using triangles vertex shader that has color attribute
///////////////////////////////////////////////////////////////////////


using namespace std;

#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <array>
#include <string>


#define BUFFER_OFFSET(x)  ((const void*) (x))

GLuint Buffers[1];

//const int Ndivisions = 0;
//const int NumTetrahedrons = 1;            // 4^0 tetrahedrons      
//const int NumTriangles = 4 * NumTetrahedrons;  // 4 triangles / tetrahedron
//const int NumVertices = 3 * NumTriangles;      // 3 vertices / triangle

//const int Ndivisions = 1;
//const int NumTetrahedrons = 4;            // 4^1 tetrahedrons      
//const int NumTriangles = 4 * NumTetrahedrons;  // 4 triangles / tetrahedron
//const int NumVertices = 3 * NumTriangles;      // 3 vertices / triangle

const int Ndivisions = 4;
const int NumTetrahedrons = 512;            // 4^4 tetrahedrons      
const int NumTriangles = 4 * NumTetrahedrons;  // 4 triangles / tetrahedron
const int NumVertices = 3 * NumTriangles;      // 3 vertices / triangle

std::array<glm::vec3, NumVertices> vertices1;
GLfloat vertices[NumVertices][3] = { 0 };
glm::vec3 v[4];

std::array<glm::vec3, NumVertices> colors1;
GLfloat colors[NumVertices][3] = { 0 };
glm::vec3 c[3] = { glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0),glm::vec3(0.0, 0.0, 1.0) };



int  Index = 0;
int  colorIndex= 0;

GLuint vao, points_vbo, colors_vbo;


void triangle(glm::vec3& a, glm::vec3& b, glm::vec3& c)
/* specify one triangle */
{

	int i = colorIndex++ % 3;
	glm::vec3 tColor = glm::vec3(0.0, 0.0, 0.0);
	switch (i)
	{
	case 0: 
		tColor = glm::vec3(1.0, 0.0, 0.0);
		break;
	case 1: 
		tColor = glm::vec3(0.0, 1.0, 0.0);
		break;
	case 2:
		tColor = glm::vec3(0.0, 0.0, 1.0);
		break;
	default:
		tColor = glm::vec3(0.0, 0.0, 0.0);
	}
	colors1[Index] = tColor;
	vertices1[Index++] = a;
	colors1[Index] = tColor;
	vertices1[Index++] = b;
	colors1[Index] = tColor;
	vertices1[Index++] = c;
}

void tetra(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d)
{
	triangle(a, b, c);
	triangle(a, c, d);
	triangle(a, d, b);
	triangle(b, d, c);
}


void divide_tetra(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d, int count)
{
	if (count > 0) {
		glm::vec3 v0 = (a + b) / 2.0f;
		glm::vec3 v1 = (a + c) / 2.0f;
		glm::vec3 v2 = (a + d) / 2.0f;
		glm::vec3 v3 = (b + c) / 2.0f;
		glm::vec3 v4 = (c + d) / 2.0f;
		glm::vec3 v5 = (b + d) / 2.0f;
		divide_tetra(a, v0, v1, v2, count - 1);
		divide_tetra(v0, b, v3, v5, count - 1);
		divide_tetra(v1, v3, c, v4, count - 1);
		divide_tetra(v2, v4, v5, d, count - 1);
	}
	else {
		tetra(a, b, c, d);    // draw tetrahedron at end of recursion
	}
}

static unsigned int
program,
vertexShaderId,
fragmentShaderId;


void init(void)
{

	// Create shader program executable.

	vertexShaderId = setShader((char*)"vertex", (char*)"triangles.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"triangles.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);




	v[0] = glm::vec3(0.0, 0.0, -1.0);
	v[1] = glm::vec3(0.0, 0.942809, 0.333333);
	v[2] = glm::vec3(-0.816497, -0.471405, 0.333333);
	v[3] = glm::vec3(0.816497, -0.471405, 0.333333);


	divide_tetra(v[0], v[1], v[2], v[3], Ndivisions);


	//// compute and store N-1 new points
	for (int i = 0; i < NumVertices; ++i) {


		vertices[i][0] = vertices1[i][0];
		vertices[i][1] = vertices1[i][1];
		vertices[i][2] = vertices1[i][2];


		colors[i][0] = colors1[i][0];
		colors[i][1] = colors1[i][1];
		colors[i][2] = colors1[i][2];
	}

	//Vertex Array Object will allow us to bundle data associated with a vertex array.
	//Use of multiple vertex-array objects will make it easy to switch among different vertex arrays.
	vao = 0;
	//We use glGenVertexArray to find an unused name for the buffer.
	glGenVertexArrays(1, &vao);
	//The first time the function glBindVertexArray is executed for a given name, the object is created.
	//Subsequent calls to this function make the named object active.
	glBindVertexArray(vao);

	//we create a buffer object on the GPU and place our data in that object.
	points_vbo = 0;

	//we use glGenBuffers to give us an unused identifier for our buffer object that is put into the variable points_vbo.
	glGenBuffers(1, &points_vbo);

	//The function glBindBuffer creates the buffer with the identifier from glGenBuffers.
	//The type GL_ARRAY_BUFFER indicates that the data in the buffer will be vertex attribute data
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

	//with glBufferData, we allocate sufficient memory on the GPU for our dataand provide a pointer to the array holding the data.
	glBufferData(GL_ARRAY_BUFFER, 3 * NumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//The fourth parameter says that we do not want the data normalized to be the range(0.0, 1.0), 
	//whereas the fifth states that the values in the array are contiguous.
	//The last parameter is the address in the buffer where the data begin.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	//we could also do it the above two lines using follwing: Ask what is the location of the attribute
	//GLuint loc;
	//loc = glGetAttribLocation(program, "vertex_position");
	//glEnableVertexAttribArray(loc);
	//glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * NumVertices * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);



	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background

}


//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//The glFlush ensures that all the data are rendered as soon as possible.If you leave it out,
	//the program should work correctly, but you notice a delay in a busy or networked environment.
	glFlush();
}

void idle()
{
	//glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(512, 512);
	glutCreateWindow("tetrahedron3");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	//We use an initialization function init() to set the OpenGL state variables dealing
	//with viewing and attributes—parameters that we prefer to set once, independently
	//of the display function.
	init();

	//we need only a single callback called the
	//display callback.A display callback is generated when the application program or the
	//operating system determines that the graphics in a window need to be redrawn.
	glutDisplayFunc(display);

	glutIdleFunc(idle);

	//To begin an event-processing loop
	//If there are no events to process, the program will
	//sit in a wait state, with our graphics on the screen, until we terminate the program
	//through some external means—say, by hitting a special key or a combination of keys,
	//such as Control - C—that terminates the execution of the program.
	glutMainLoop();



}


