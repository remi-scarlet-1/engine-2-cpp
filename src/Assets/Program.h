#pragma once

namespace engine::assets {
	class Program {
	public:
		Program(unsigned int program)
			: program(program)
		{}
		~Program() = default;

	private:
		unsigned int program = 0;
	};
}