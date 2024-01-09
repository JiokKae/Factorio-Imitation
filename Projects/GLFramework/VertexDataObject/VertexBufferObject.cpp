#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject()
	: ID{}
{
	glGenBuffers(1, &this->ID);
}

VertexBufferObject::VertexBufferObject(GLsizeiptr size, const void* data, GLenum usage)
	: ID{}
{
	glGenBuffers(1, &this->ID);
	SetData(size, data, usage);
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &this->ID);
}

void VertexBufferObject::SetData(GLsizeiptr size, const void* data, GLenum usage)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
