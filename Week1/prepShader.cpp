#include <cstdlib>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

// Function to read external shader file.
char* readShader(std::string fileName)
{
	// Initialize input stream.
	std::ifstream inFile(fileName.c_str(), std::ios::binary);

	// Determine shader file length and reserve space to read it in.
	inFile.seekg(0, std::ios::end);  //Returns The istream object (*this).
	int fileLength = inFile.tellg();
	char* fileContent = (char*)malloc((fileLength + 1) * sizeof(char));

	// Read in shader file, set last character to NUL, close input stream.
	inFile.seekg(0, std::ios::beg); //begining
	inFile.read(fileContent, fileLength);
	fileContent[fileLength] = '\0';
	inFile.close();

	return fileContent;
}

// Function to initialize shaders.
int setShader(char* shaderType, char* shaderFile)
{
	int shaderId;

	if (shaderType == "vertex") shaderId = glCreateShader(GL_VERTEX_SHADER);
	if (shaderType == "tessControl") shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
	if (shaderType == "tessEvaluation") shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
	if (shaderType == "geometry") shaderId = glCreateShader(GL_GEOMETRY_SHADER);
	if (shaderType == "fragment") shaderId = glCreateShader(GL_FRAGMENT_SHADER);

	const char* shader = readShader(shaderFile);

	//or you could have done this instead...
	const char* vertex_shader_text =
		"#version 460 core\n"
		"layout(location = 0) in vec4 vPosition;\n"
		"out vec4 myColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vPosition;\n"
		"}\n";

	const char* fragment_shader_text =
		"#version 460 core\n"
		"out vec4 fColor;\n"
		"void main()\n"
	    "{\n"
		"fColor = vec4(1.0, 0.0, 0.0, 0.0);\n"
		"}\n";


	//if (shaderType == "vertex") shader = vertex_shader_text;
	//if (shaderType == "fragment") shader = fragment_shader_text;


	glShaderSource(shaderId, 1, (const char**)&shader, NULL);
	glCompileShader(shaderId);
	
	GLint shader_compiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shader_compiled);
	if (shader_compiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(shaderId, 1024, &log_length, message);
		// Write the error to a log
	}

	return shaderId;
}

