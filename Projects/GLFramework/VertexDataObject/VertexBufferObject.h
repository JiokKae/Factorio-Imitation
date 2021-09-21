#pragma once
#include "../framework.h"

typedef class VertexBufferObject
{
	GLuint ID;

public:
	GLuint GetID() { return ID; }
	void SetData(GLsizeiptr size, const void *data, GLenum usage);
	VertexBufferObject();
	~VertexBufferObject();
} VBO;

