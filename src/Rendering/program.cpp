#include "program.h"

#include <iostream>
#include <glad/glad.h>

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

engine::rendering::Program::Program(std::string vertexShaderSource, std::string fragmentShaderSource)
{
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());
    
    program = linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    locationModelMatrix = glGetUniformLocation(program, "model");
    locationViewMatrix = glGetUniformLocation(program, "view");
    locationProjectionMatrix = glGetUniformLocation(program, "projection");
}

engine::rendering::Program::~Program()
{
    glDeleteProgram(program);
}

const unsigned int engine::rendering::Program::getHandle()
{
    return program;
}

void engine::rendering::Program::getLocations(
    unsigned int& modelMatrixLoc, 
    unsigned int& modelViewLoc, 
    unsigned int& modelProjectionLoc)
{
    modelMatrixLoc = locationModelMatrix;
    modelViewLoc = locationViewMatrix;
    modelProjectionLoc = locationProjectionMatrix;
}
