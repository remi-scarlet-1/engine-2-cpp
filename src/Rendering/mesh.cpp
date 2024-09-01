#include "mesh.h"

#include <glad/glad.h>

engine::rendering::Mesh::Mesh(const unsigned int* indexData, int indexCount, const float* vertexData, int vertexCount) : indexCount(indexCount)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint ebo, vbo;

	setupEBO(ebo, indexData, indexCount);
	setupVBO(vbo, vertexData, vertexCount, 0, 3, 3 * sizeof(float));

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Release buffers (while vao isnt deleted data is kept anyways)
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

engine::rendering::Mesh::Mesh(const unsigned int* indexData, int indexCount, const float* vertexData, int vertexCount, const float* normalData, int normalCount) : indexCount(indexCount)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint ebo, vbo, nbo;

	setupEBO(ebo, indexData, indexCount);
	setupVBO(vbo, vertexData, vertexCount, 0, 3, 3 * sizeof(float));
	setupVBO(nbo, normalData, normalCount, 1, 3, 3 * sizeof(float));

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Release buffers (while vao isn't deleted, data is kept anyways)
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &nbo);
}

engine::rendering::Mesh::~Mesh() 
{
	glDeleteVertexArrays(1, &vao);
}

void engine::rendering::Mesh::setupVBO(unsigned int& vbo, const float* data, int count, int attributeIndex, int componentsPerVertex, int stride)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeIndex, componentsPerVertex, GL_FLOAT, false, stride, (GLvoid*)0);
	glEnableVertexAttribArray(attributeIndex);
}

void engine::rendering::Mesh::setupEBO(unsigned int& ebo, const unsigned int* indexData, int indexCount) {
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
}
