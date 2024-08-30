#pragma once

#include <string>

namespace engine::graphics {
	class Program
	{
	public:
		Program(
			std::string vertexShader,
			std::string	fragmentShader
		);
		~Program();

		const unsigned int getHandle();
		void getLocations(
			unsigned int& modelMatrixLoc, 
			unsigned int& modelViewLoc, 
			unsigned int& modelProjectionLoc
		);

	private:
		unsigned int program;

		unsigned int locationModelMatrix;
		unsigned int locationViewMatrix;
		unsigned int locationProjectionMatrix;
	};
}