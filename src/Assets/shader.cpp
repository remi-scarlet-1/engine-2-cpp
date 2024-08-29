#include "shader.h"

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

// NOT THREAD SAFE, translation unit wide success & infoLog buffers
static int success;
static char infoLog[512];

unsigned int engine::compileShader(unsigned int shaderType, std::string shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    const char* source = shaderSource.c_str();
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check for compilation errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << getShaderTypeName(shaderType) << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int engine::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    return program;
}