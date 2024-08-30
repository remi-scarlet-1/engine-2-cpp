#pragma once

#include <SFML/Window.hpp>
#include <glm/glm.hpp>

#include "program.h"
#include "camera.h"
#include "mesh.h"

namespace engine::graphics {
    class Renderer {
    public:
        Renderer(const char* windowTitle, glm::i16vec2 windowRes, bool windowResizable, unsigned int frameLimit);

        void initialize();
        void clear();
        void draw(Mesh& mesh, Program& program, Camera& camera);
        void present();

        // Window management
        bool windowIsOpen() { return window->isOpen(); }
        bool windowPollEvent(sf::Event& event) { return window->pollEvent(event); }
        void windowClose() { window->close(); }

    private:
        sf::Window* window;
    };
}