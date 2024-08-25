#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

int main() {
    // Create a window with a specified size
    sf::Window window(sf::VideoMode(800, 600), "SFML Window Test");

    // Print the version of SFML being used
    std::cout << "SFML Version: " << SFML_VERSION_MAJOR << "."
        << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        // Process events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Close the window when the close event is received
            }
        }

        // Here you could do other non-graphics related processing if needed

        // Sleep for a short period to avoid using 100% CPU
        sf::sleep(sf::milliseconds(10));
    }

    return 0;
}