#include "AssetManager.h"

#include <GLAD/glad.h>
#include <stdexcept>
#include <iostream>

#include "Util/Filesystem.h"

const char* getShaderTypeName(GLenum shaderType) {
	switch (shaderType) {
	case GL_VERTEX_SHADER:          return "GL_VERTEX_SHADER";
	case GL_FRAGMENT_SHADER:        return "GL_FRAGMENT_SHADER";
	case GL_GEOMETRY_SHADER:        return "GL_GEOMETRY_SHADER";
	case GL_COMPUTE_SHADER:         return "GL_COMPUTE_SHADER";
	case GL_TESS_CONTROL_SHADER:    return "GL_TESS_CONTROL_SHADER";
	case GL_TESS_EVALUATION_SHADER: return "GL_TESS_EVALUATION_SHADER";
	default:                        return "UNKNOWN_SHADER_TYPE";
	}
}

unsigned int compileShader(unsigned int shaderType, const char* source) {
	int success;
	char infoLog[512];

	GLuint handle = glCreateShader(shaderType);
	glShaderSource(handle, 1, &source, NULL);
	glCompileShader(handle);

	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(handle, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::" << getShaderTypeName(shaderType) << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return handle;
}

unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
	int success;
	char infoLog[512];

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	return program;
}

template<>
std::shared_ptr<engine::assets::Program> engine::AssetLoader::loadFromFile(const std::filesystem::path& path) {
	std::string vertexShaderSource = engine::loadFileToString(path);
	std::string fragmentShaderSource = engine::loadFileToString(std::filesystem::path(path).replace_extension(".frag"));

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

	auto program = std::make_shared<engine::assets::Program>(linkProgram(vertexShader, fragmentShader));

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

template<>
std::shared_ptr<engine::assets::Mesh> engine::AssetLoader::loadFromFile(const std::filesystem::path& path) {
	// TODO: Load mesh
	throw std::runtime_error("Loading assets of type Mesh is not implemented yet.");
	return std::make_shared<engine::assets::Mesh>(0);
}