#pragma once
#include "../framework.h"

typedef class VertexBufferObject
{
public:
	VertexBufferObject();
	VertexBufferObject(GLsizeiptr size, const void* data, GLenum usage);
	~VertexBufferObject();

	GLuint GetID() const { return ID; }
	void SetData(GLsizeiptr size, const void *data, GLenum usage);

private:
	GLuint ID;
} VBO;
