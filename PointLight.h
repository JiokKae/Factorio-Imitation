#pragma once
// CC
#include "framework.h"
using namespace glm;

class PointLight
{
public:
	vec3 position;
	vec3 amient;
	vec3 diffuse;
	float_t constant;
	float_t linear;
	float_t quadratic;
};

