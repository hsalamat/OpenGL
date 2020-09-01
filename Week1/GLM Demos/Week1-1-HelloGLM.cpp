//Make sure you already did the following:
//GLM : Download and unzip the file https ://github.com/g-truc/glm/releases/download/0.9.7.5/glm-0.9.7.5.zip and save the folder glm-0.9.7.5 under OpenGLwrappers
//Under Project Properties page: C/C++ : Additional Include Directories --> C:\OpenGLwrappers\glm-0.9.7.5\glm

//GLM is a header only library that makes a heavy usage of C++ templates.

//A common feature of shader languages like GLSL is the swizzle operators.Those allow selecting multiple components of a vector and change their order.
//For example, “variable.x”, “variable.xzy” and “variable.zxyy” form respectively a scalar, a three components vector and a four components vector.

#include <iostream>
using namespace std;

#define GLM_SWIZZLE 
#include <glm/glm.hpp>
int main()
{
	glm::vec4 ColorRGBA(1.0f, 0.5f, 0.0f, 1.0f); 
	glm::vec3 ColorBGR = ColorRGBA.bgr(); 
	glm::vec4 PositionA(1.0f, 0.5f, 0.0f, 1.0f); 
	glm::vec3 PositionB = PositionA.xyz() * 2.0f;

	cout << "ColorRGBA - Red: " << ColorRGBA.x << " , G:" << ColorRGBA.y << ", B:" << ColorRGBA.z << endl;

	//Swizzle operators return a copy of the component values hence they can’t be used as L - values to change the value of the variables.
	// /!\ No compiler error but ColorRGBA is not affected // This code modify the components of an anonymous copy. 
	ColorRGBA.bgr() = glm::vec3(2.0f, 1.5f, 1.0f); // ColorRGBA is not modified!
	cout << "ColorRGBA - Red: " << ColorRGBA.x << " , G:" << ColorRGBA.y << ", B:" << ColorRGBA.z << endl;

	// l-value: 
	glm::vec4 ColorBGRA = ColorRGBA.bgra;
	cout << "ColorBGRA - Red: " << ColorBGRA.x << " , G:" << ColorBGRA.y << ", B:" << ColorBGRA.z << endl;

	//restrict values between 0, 1
	glm::vec3 ClampedC = glm::clamp(PositionB.xyz(), 0.f, 1.f);
	cout << "ClampedC - Red: " << ClampedC.x << " , G:" << ClampedC.y << ", B:" << ClampedC.z << endl;

	return 0;
}
