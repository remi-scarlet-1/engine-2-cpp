#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <glad/glad.h>
#include <iostream>
#include <string>

#include "file.h"
#include "model.h"
#include "shader.h"

int main() {
    // Create a window
    sf::Window window(sf::VideoMode(1920, 1080), "Engine Test Window", sf::Style::Default, sf::ContextSettings(24, 8, 4, 4, 6));

    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Print OpenGL version
    std::cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Create triangle data
    std::string cubePath = file::getExecutablePath().parent_path().append("assets/cube/cube.gltf").string();
    const char* cubePathC = cubePath.c_str();
    GLuint VAO = model::LoadMeshVAO(cubePathC);

    // Create and compile the vertex shader
    GLuint vertexShader = shader::compileShader(GL_VERTEX_SHADER, file::readFileToString("assets/default.vert"));
    GLuint fragmentShader = shader::compileShader(GL_FRAGMENT_SHADER, file::readFileToString("assets/default.frag"));

    GLuint shaderProgram = shader::linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program and draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers
        window.display();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    return 0;
}