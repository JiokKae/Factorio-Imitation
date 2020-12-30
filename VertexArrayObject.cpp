#include "VertexArrayObject.h"

void VertexArrayObject::AddVertexBufferObject()
{
	glBindVertexArray(this->ID);

	glBindVertexArray(0);
}

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &this->ID);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &this->ID);
}
