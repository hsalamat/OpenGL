#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

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

struct AmbientLight : public Light
{
	AmbientLight(glm::vec3 aCol, GLfloat aStr) : Light(aCol, aStr) {}
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
	GLfloat constant, linear, quadratic;
	PointLight(glm::vec3 pos, GLfloat range, GLfloat con, GLfloat lin, GLfloat quad,
		glm::vec3 dCol, GLfloat dStr) : Light(dCol, dStr)
	{
		position = pos;
		constant = con; //= 1.0f
		linear = lin / range; //= 4.5f / range;
		quadratic = quad / (range * range); //= 75.0f / (range * range);
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
