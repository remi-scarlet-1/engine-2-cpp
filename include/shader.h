#pragma once

#include <iostream>
#include <glad/glad.h>

namespace shader {
	GLuint compileShader(GLuint glShaderType, std::string shaderCode);
	GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
}
