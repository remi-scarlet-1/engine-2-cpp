#pragma once

namespace engine::rendering {
	class Mesh {
	public:
		Mesh(
			const unsigned int* indexData, int indexCount,
			const float* vertexData, int vertexCount
		);
		Mesh(
			const unsigned int* indexData, int indexCount,
			const float* vertexData, int vertexCount,
			const float* normalData, int normalCount
		);
		~Mesh();

		unsigned int getHandle() const { return vao; }
		int getIndexCount() const { return indexCount; }

	private:
		unsigned int vao = 0;
		int indexCount;

		void setupVBO(unsigned int& vbo, const float* data, int count, int attributeIndex, int componentsPerVertex, int stride);
		void setupEBO(unsigned int& ebo, const unsigned int* indexData, int indexCount);
	};
}