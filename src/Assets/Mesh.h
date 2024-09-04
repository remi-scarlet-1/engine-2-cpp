#pragma once

namespace engine::assets {
	class Mesh {
	public:
		Mesh(unsigned int vao) 
			: vao(vao) 
		{}
		~Mesh() = default;

	private:
		unsigned int vao = 0;
	};
}