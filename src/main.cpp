#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <glad/glad.h>
#include <iostream>
#include <string>

#include "file.h"
#include "shader.h"

void createTriangleData(unsigned int* VAO, unsigned int* VBO) {
    // Define vertices of a triangle
    float vertices[] = {
         0.0f,  0.5f, 0.0f,  // Top vertex
        -0.5f, -0.5f, 0.0f,  // Bottom left vertex
         0.5f, -0.5f, 0.0f   // Bottom right vertex
    };

    // Create a Vertex Array Object (VAO)
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);

    // Create a Vertex Buffer Object (VBO)
    glGenBuffers(1, VBO);

    // Bind the VBO and upload the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure the vertex attributes (position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

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
    GLuint VAO;
    GLuint VBO;
    createTriangleData(&VAO, &VBO);

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
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}