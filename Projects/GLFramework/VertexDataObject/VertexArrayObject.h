#pragma once
#include "../framework.h"
#include <map>

class VertexBufferObject;
typedef class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	GLuint GetID() { return ID; }
	void AddVBO(GLuint index, VertexBufferObject* vbo, GLuint size);
	void SetVBOData(GLuint index, GLsizeiptr size, const void* data, GLenum usage);
	void SetDivisor(GLuint index, GLuint divisor);

private:
	GLuint ID{};
	std::map<GLuint, VertexBufferObject*> VBOs;
} VAO;
