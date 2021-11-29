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
   inFile.seekg(0, std::ios::end);
   int fileLength = inFile.tellg();
   char *fileContent = (char*) malloc((fileLength+1) * sizeof(char)); 
   
   // Read in shader file, set last character to NUL, close input stream.
   inFile.seekg(0, std::ios::beg);
   inFile.read(fileContent, fileLength);
   fileContent[fileLength] = '\0';
   inFile.close();
   
   return fileContent;
}

// Function to initialize shaders.
int setShader(char* shaderType, char* shaderFile)
{
   int shaderId;
   char* shader = readShader(shaderFile);
   
   if (shaderType == "vertex") shaderId = glCreateShader(GL_VERTEX_SHADER); 
   if (shaderType == "tessControl") shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);    
   if (shaderType == "tessEvaluation") shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER); 
   if (shaderType == "geometry") shaderId = glCreateShader(GL_GEOMETRY_SHADER); 
   if (shaderType == "fragment") shaderId = glCreateShader(GL_FRAGMENT_SHADER); 

   glShaderSource(shaderId, 1, (const char**) &shader, NULL); 
   glCompileShader(shaderId); 

   return shaderId;
}

