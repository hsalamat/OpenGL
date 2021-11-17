#pragma once
#include "glm\glm.hpp"
#include <GL/glew.h>

struct Light
{
	glm::vec3 diffuseColour;
	GLfloat diffuseStrength;
	Light(glm::vec3 dCol, GLfloat dStr)
	{
		diffuseColour = dCol;
		diffuseStrength = dStr;
	}
};

struct AmbientLight
{
	glm::vec3 ambientColour;
	GLfloat ambientStrength;
	AmbientLight(glm::vec3 aCol, GLfloat aStr)
	{
		ambientColour = aCol;
		ambientStrength = aStr;
	}
};

struct DirectionalLight : public Light
{
	glm::vec3 direction;
	DirectionalLight(glm::vec3 dir, glm::vec3 dCol, GLfloat dStr)
		: Light(dCol, dStr)
	{
		direction = dir;
	}
};

struct PointLight : public Light
{
	glm::vec3 position; //= glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat constant, linear, exponent;
	PointLight(glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp,
		glm::vec3 dCol, GLfloat dStr) : Light(dCol, dStr)
	{
		position = pos;
		constant = con;
		linear = lin;
		exponent = exp;
	}
};

struct Material
{
	GLfloat specularStrength;
	GLfloat shininess;
};