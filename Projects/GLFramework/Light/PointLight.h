#pragma once
// CC
#include "../framework.h"
using namespace glm;

class PointLight
{
public:
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	float_t constant;
	float_t linear;
	float_t quadratic;

public:
	static ULONG64 std140Size() { return sizeof(vec4) * 4; }
};

