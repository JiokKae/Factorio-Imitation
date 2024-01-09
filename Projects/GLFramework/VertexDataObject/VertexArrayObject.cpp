#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &this->ID);
}

VertexArrayObject::~VertexArrayObject()
{
	glBindVertexArray(this->ID);

	for (auto& vbo : VBOs)
	{
		glDisableVertexAttribArray(vbo.first);
		if (vbo.second)
		{
			delete vbo.second;
			vbo.second = nullptr;
		}
	}

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &this->ID);
}

void VertexArrayObject::AddVBO(GLuint index, VertexBufferObject* vbo, GLuint size)
{
	if (VBOs.contains(index) == true)
	{
		return;
	}

	glBindVertexArray(this->ID);

	glBindBuffer(GL_ARRAY_BUFFER, vbo->GetID());

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);

	VBOs.emplace(index, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void VertexArrayObject::SetVBOData(GLuint index, GLsizeiptr size, const void* data, GLenum usage)
{
	if (VBOs.contains(index) == true)
	{
		VBOs[index]->SetData(size, data, usage);
	}
}

void VertexArrayObject::SetDivisor(GLuint index, GLuint divisor)
{
	glBindVertexArray(this->ID);

	glVertexAttribDivisor(index, divisor);

	glBindVertexArray(0);
}
