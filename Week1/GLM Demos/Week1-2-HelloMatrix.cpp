//GLM extends the core GLSL feature set with extensions.These extensions include : quaternion, transformation, spline, matrix inverse, color spaces, etc.

#include <iostream>
using namespace std;

#define GLM_SWIZZLE 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


glm::vec3 computeNormal(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c)
{
	return glm::normalize(glm::cross(c - a, b - a));
}

int main()
{
	glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
	glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::vec4 Transformed = Model * Position;

	cout << "Position (x,y,z): (" << Position.x << " ," << Position.y << ", " << Position.z << ")" << endl;
	cout << "Transformed (x,y,z): (" << Transformed.x << " , " << Transformed.y << ", " << Transformed.z << ")" << endl;


	glm::vec3 Translate = glm::vec3(1.0f);
	glm::vec3 Rotate = glm::vec3(glm::radians(90.0), glm::radians(45.0), glm::radians(30.0));
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f); 
	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), Translate); 
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f)); 
	glm::mat4 View = glm::rotate(ViewRotateX, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f)); 
	glm::mat4 Model2 = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

	return 0;
}