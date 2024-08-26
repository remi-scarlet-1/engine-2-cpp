#pragma once

#include <iostream>
#include <glad/glad.h>

namespace shaderloader {
	GLuint compileShader(GLuint glShaderType, std::string shaderCode);
	GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
}
