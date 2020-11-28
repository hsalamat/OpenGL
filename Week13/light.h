#pragma once

#include "glm\glm.hpp"
#include <GL/glew.h>

struct Diffuse
{
	glm::vec4 light;
	glm::vec4 material;
	Diffuse(glm::vec4 dLight, glm::vec4 dMaterial)
	{
		light = dLight;
		material = dMaterial;
	}
};

struct Ambient
{
	glm::vec4 light;
	glm::vec4 material;
	Ambient(glm::vec4 aLight, glm::vec4 aMaterial)
	{
		light = aLight;
		material = aMaterial;
	}
};

struct Specular
{
	glm::vec4 light;
	glm::vec4 material;
	GLfloat shininess;
	Specular(glm::vec4 sLight, glm::vec4 sMaterial, GLfloat sShininess)
	{
		light = sLight;
		material = sMaterial;
		shininess = sShininess;
	}
};


struct DirectionalLight : public Diffuse
{
	glm::vec3 direction;
	DirectionalLight(glm::vec3 dir, glm::vec4 dLight, glm::vec4 dMaterial)
		: Diffuse(dLight, dMaterial)
	{
		direction = dir;
	}
};

struct PointLight : public Diffuse
{
	glm::vec3 position; 
	GLfloat constant, linear, exponent;
	PointLight(glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp,
		glm::vec4 dLight, glm::vec4 dMaterial) : Diffuse(dLight, dMaterial)
	{
		position = pos;
		constant = con;
		linear = lin;
		exponent = exp;
	}
};
