#pragma once
#include "../framework.h"

using namespace glm;
class DirectionalLight
{
public:
	vec3 ambient;
	vec3 diffuse;

public:
	static ULONGLONG std140Size() { return sizeof(vec4) * 2; }
};

