#pragma once

#include <iostream>

namespace engine {
	/// <summary> Compiles a shader from source. Source must be provided in form of a std::string. </summary>
	/// <returns> OpenGL handle to successfuly compiled shader or 0. </returns>
	unsigned int compileShader(unsigned int shaderType, std::string shaderSource);

	/// <summary> Links a standard OpenGL program from a vertex and fragment shader handle. </summary>
	/// <returns> OpenGL handle to successfuly linked program or 0. </returns>
	unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
}
