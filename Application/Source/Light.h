#ifndef LIGHT_H
#define LIGHT_H

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct Light
{
	glm::vec3 position; //position of light source
	glm::vec3 color; //RGB color of light
	float power; //intensity of light source
	float kC, kL, kQ; //attenuation constants

	// For spot light
	glm::vec3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;

	enum LIGHT_TYPE
	{
		POINT,
		DIRECTIONAL,
		SPOT,
	};

	LIGHT_TYPE type;

	Light() : position(0, 20, 0), color(1, 1, 1), power(1),
		kC(1), kL(0.1f), kQ(0.001f), type(POINT), spotDirection(1.f), cosCutoff(45.f), cosInner(30.f), exponent(1.f)
	{
	}
};

#endif