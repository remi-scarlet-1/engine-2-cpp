#include "mesh.h"

#include <glad/glad.h>

engine::graphics::Mesh::Mesh(
	const unsigned int* indexData, int indexCount,
	const float* vertexData, int vertexCount)
	: indexCount(indexCount)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Release buffers (while vao isnt deleted data is kept anyways)
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

engine::graphics::Mesh::~Mesh() 
{
	glDeleteVertexArrays(1, &vao);
}