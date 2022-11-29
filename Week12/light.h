#pragma once
#include "glm\glm.hpp"
#include <GL/glew.h>

struct Light
{
	glm::vec3 diffuseColor;
	GLfloat diffuseStrength;
	Light(glm::vec3 dCol, GLfloat dStr)
	{
		diffuseColor = dCol;
		diffuseStrength = dStr;
	}
};

struct AmbientLight
{
	glm::vec3 ambientColor;
	GLfloat ambientStrength;
	AmbientLight(glm::vec3 aCol, GLfloat aStr)
	{
		ambientColor = aCol;
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