#pragma once
#include "framework.h"

using namespace glm;
class DirectionalLight
{
public:
	vec3 ambient;
	vec3 diffuse;

public:
	static int std140Size() { return sizeof(vec4) * 2; }
};

