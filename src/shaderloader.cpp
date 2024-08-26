#include "shaderloader.h"

// NOT THREAD SAFE, defining translation wide buffer and status int to not reallocate every time
static int success;
static char infoLog[512];

GLuint shaderloader::compileShader(GLuint glShaderType, std::string shaderCode) {
    // Convert shader code to c style string
    const char* shaderCodeC = shaderCode.c_str();

    // Create shader & try compile
    GLuint shader = glCreateShader(glShaderType);
    glShaderSource(shader, 1, &shaderCodeC, NULL);
    glCompileShader(shader);

    // Check for compilation errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::UNKNOWN::COMPILATION_FAILED\n" << infoLog << std::endl; // TODO: fix error message
    }

    return shader;
}

GLuint shaderloader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    // Create program & try link
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