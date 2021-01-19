#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

void VertexArrayObject::AddVBO(GLuint index, VertexBufferObject* vbo, GLuint size)
{
	// 만약 인덱스가 있다면 탈출
	if (mapVBOs.find(index) != mapVBOs.end())
		return;

	// Bind VAO
	glBindVertexArray(this->ID);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo->GetID());

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);

	mapVBOs.insert(make_pair(index, vbo));

	// Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
	glBindVertexArray(0);
}

void VertexArrayObject::SetVBOData(GLuint index, GLsizeiptr size, const void* data, GLenum usage)
{
	it = mapVBOs.find(index);
	
	if (it == mapVBOs.end())
		return;

	it->second->SetData(size, data, usage);
}

void VertexArrayObject::SetDivisor(GLuint index, GLuint divisor)
{
	// Bind VAO
	glBindVertexArray(this->ID);

	glVertexAttribDivisor(index, divisor);

	// Unbind VAO
	glBindVertexArray(0);
}

VertexArrayObject::VertexArrayObject()
{
	// Generate VAO
	glGenVertexArrays(1, &this->ID);
}

VertexArrayObject::~VertexArrayObject()
{
	// Bind VAO
	glBindVertexArray(this->ID);

	// Delete VBOs
	for (it = mapVBOs.begin();it != mapVBOs.end(); it++)
	{
		glDisableVertexAttribArray(it->first);
		delete it->second;
		it->second = nullptr;
	}
	mapVBOs.clear();

	// Unbind VAO
	glBindVertexArray(0);

	// Delete VAO
	glDeleteVertexArrays(1, &this->ID);
}
