/////////////////////////////////////////////////////////////////////////////////////
// testGLFW.cpp
//
// This program is testGlut.cpp rewritten to use GLFW to manage the window
// instead of FreeGLUT. All GLFW calls are commentated.
//
//Hooman
/////////////////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>  
#include <GLFW/glfw3.h> // Include GLFW header.

// Globals.
GLFWwindow* window; // Handle to the OpenGL window created by GLFW.
int w, h; // Window width, height.

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);

    // Draw a polygon with specified vertices.
    glBegin(GL_POLYGON);
    glVertex3f(20.0, 20.0, 0.0);
    glVertex3f(80.0, 20.0, 0.0);
    glVertex3f(80.0, 80.0, 0.0);
    glVertex3f(20.0, 80.0, 0.0);
    glEnd();

    glfwSwapBuffers(window); // Swap buffers. 
                             // See comments about double-buffering in main() below.
}

// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL minor version number.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // OpenGL compatiblity profile.
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // Double buffering. This is the default which some
                                                  // systems may not even allow to be turned off
                                                  // (say, to get single-buffering as in square.cpp). 
                                                  // Therefore, buffer swapping may be required
                                                  // as in drawScene() above.

    window = glfwCreateWindow(500, 500, "squareGLFW.cpp", NULL, NULL); // Create OpenGL window and return
                                                                       // a handle to the window.

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