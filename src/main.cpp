#include <iostream>
#include <chrono>
#include <string>

#include <SFML/Window.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utilities/filesystem.h"
#include "Utilities/time.h"
#include "Assets/model.h"

#include "Graphics/mesh.h"
#include "Graphics/program.h"
#include "Graphics/camera.h"
#include "Graphics/renderer.h"

using namespace engine;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
};

unsigned int indices[] = {
    // Back
    0, 1, 2,
    2, 1, 3,

    // Front
    6, 5, 4,
    5, 6, 7,

    // Left
    4, 5, 0,
    1, 0, 5,

    // Right
    2, 3, 6,
    7, 6, 3,

    // Bottom
    0, 2, 4,
    6, 4, 2,

    // Top
    1, 5, 3,
    7, 3, 5
};

#pragma region Input
int lastMousePosX = 0;
int lastMousePosY = 0;

glm::i32vec2 getMouselookInput() {
    glm::i32vec2 input = glm::i32vec2(sf::Mouse::getPosition().x - lastMousePosX, sf::Mouse::getPosition().y - lastMousePosY);
    lastMousePosX = sf::Mouse::getPosition().x;
    lastMousePosY = sf::Mouse::getPosition().y;
    return input;
}

glm::vec3 getMoveInput() {
    glm::vec3 moveInput = glm::vec3(0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveInput.z = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveInput.z = -1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveInput.x = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveInput.x = -1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) moveInput.y = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) moveInput.y = -1.0f;
    return moveInput;
}
#pragma endregion

int main() {
    auto renderer = graphics::Renderer("Test Engine", glm::i16vec2(1920, 1080), false, 500);
    auto camera = graphics::Camera(5.0f, 0.25f, 68.0f, (16.0f / 9.0f), 0.001f, 1000.0f);
    auto mesh = graphics::Mesh(indices, 36, vertices, 24);
    auto program = graphics::Program(
        utils::loadFileToString(utils::getAssetsDir().append("default.vert")), 
        utils::loadFileToString(utils::getAssetsDir().append("default.frag"))
    );

    renderer.initialize();
    renderer.OnFrameEndSubscribe(utils::updateTime);

    while (renderer.windowIsOpen()) {
        // Handle events
        sf::Event event;
        while (renderer.windowPollEvent(event))
            if (event.type == sf::Event::Closed)
                renderer.windowClose();

        // Update camera movement
        glm::i32vec2 mousedelta = getMouselookInput();
        camera.updateMovement(getMoveInput(), utils::deltaTime());
        camera.updateMouselook(mousedelta.x, mousedelta.y);

        // Render
        renderer.clear();
        renderer.draw(mesh, program, camera);
        renderer.present();
    }

    return 0;
}