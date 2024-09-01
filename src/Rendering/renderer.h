#pragma once

#include <vector>
#include <functional>

#include <SFML/Window.hpp>
#include <glm/glm.hpp>

#include "program.h"
#include "camera.h"
#include "mesh.h"

namespace engine::rendering {
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

        // Callbacks
        using OnFrameEnd = std::function<void()>;

        void OnFrameEndSubscribe(OnFrameEnd callback) { onFrameEndCallbacks.push_back(callback); }

    private:

        std::vector<OnFrameEnd> onFrameEndCallbacks;
        sf::Window* window;
    };
}