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
	//GLfloat constant, linear, exponent;
	/*PointLight(glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp,
		glm::vec3 dCol, GLfloat dStr) : Light(dCol, dStr)
	{
		position = pos;
		constant = con;
		linear = lin;
		exponent = exp;
	}*/
	GLfloat constant, linear, exponent, range;
	PointLight(glm::vec3 pos, GLfloat range,
		glm::vec3 dCol, GLfloat dStr) : Light(dCol, dStr)
	{
		position = pos;
		constant = 1.0f;
		linear = 4.5f / range;
		exponent = 75.0f / (range * range);
	}
};

struct SpotLight : public Light
{
	glm::vec3 position;
	glm::vec3 direction;
	GLfloat edge, edgeRad;
	SpotLight(glm::vec3 pos, glm::vec3 dCol, GLfloat dStr, glm::vec3 dir,
		GLfloat e) : Light(dCol, dStr)
	{
		position = pos;
		direction = dir;
		edge = e;
		edgeRad = cosf(glm::radians(edge));
	}
};

struct Material
{
	GLfloat specularStrength;
	GLfloat shininess;
};