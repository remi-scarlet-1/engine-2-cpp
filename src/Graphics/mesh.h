#pragma once

namespace engine::graphics {
	class Mesh {
	public:
		Mesh(
			const unsigned int* indexData, int indexCount,
			const float* vertexData, int vertexCount
		);
		~Mesh();

		unsigned int getHandle() const { return vao; }
		int getIndexCount() const { return indexCount; }

	private:
		unsigned int vao = 0;
		int indexCount;
	};
}