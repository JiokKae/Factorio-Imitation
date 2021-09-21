#pragma once
#include "../framework.h"

class VertexBufferObject;
typedef class VertexArrayObject
{
	GLuint ID;
	map<GLuint, VertexBufferObject*> mapVBOs;
	map<GLuint, VertexBufferObject*>::iterator it;

public:
	GLuint GetID() { return ID; }
	void AddVBO(GLuint index, VertexBufferObject* vbo, GLuint size);
	void SetVBOData(GLuint index, GLsizeiptr size, const void* data, GLenum usage);
	void SetDivisor(GLuint index, GLuint divisor);
	VertexArrayObject();
	~VertexArrayObject();
} VAO;

