#include "VertexBufferObject.h"

void VertexBufferObject::SetData(GLsizeiptr size, const void* data, GLenum usage)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &this->ID);
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &this->ID);
}
