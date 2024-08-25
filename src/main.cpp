#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <glad/glad.h>
#include <iostream>

int main() {
    // Create a window with a specified size
    sf::Window window(sf::VideoMode(800, 600), "Test", sf::Style::Default, sf::ContextSettings(24, 8, 4, 4, 6));

    // Print SFML versopm
    std::cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

    // Load OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Print OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Render & Logic
    }

    return 0;
}