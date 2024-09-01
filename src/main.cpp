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

#include "Rendering/mesh.h"
#include "Rendering/program.h"
#include "Rendering/camera.h"
#include "Rendering/renderer.h"

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

rendering::Mesh** loadModelMeshes(size_t& meshCount) {
    // Load model
    assets::Model* model = assets::loadModelGLTF(utils::getAssetsDir().append("sponza/sponza.glb"));

    if (!model) {
        std::cerr << "Failed to load model." << std::endl;
        meshCount = 0;
        return nullptr;
    }

    // Count total mesh primitives
    meshCount = 0;
    for (const auto& mesh : model->Meshes)
        meshCount += mesh.MeshPrimitives.size();

    // Allocate array for graphics::Mesh pointers
    rendering::Mesh** meshes = new rendering::Mesh*[meshCount];

    size_t meshCurrent = 0;
    for (const auto& mesh : model->Meshes) {
        for (const auto& primitive : mesh.MeshPrimitives) {
            if (meshCurrent >= meshCount) {
                std::cerr << "Buffer overrun detected" << std::endl;
                break;
            }

            meshes[meshCurrent] = new rendering::Mesh(
                primitive.Indices.data(), static_cast<int>(primitive.Indices.size()),
                primitive.Positions.data(), static_cast<int>(primitive.Positions.size()),
                primitive.Normals.data(), static_cast<int>(primitive.Normals.size())
            );

            ++meshCurrent;
        }
    }

    // Clean up
    delete model;
    return meshes;
}

int main() {
    // Initialize systems:
    // - window
    // - renderer
    // - resource manager

    // Initialize ecs:
    // - register components
    // - register systems

    // Initialize rest...


    // Main loop

    // - calculate delta time
    // - handle input
    // - update game logic (ecs)
    // - render (present frame)

    // Check if game should close

    auto renderer = rendering::Renderer("Test Engine", glm::i16vec2(1920, 1080), false, 500);
    auto camera = rendering::Camera(5.0f, 0.25f, 68.0f, (16.0f / 9.0f), 0.001f, 1000.0f);
    auto program = rendering::Program(
        utils::loadFileToString(utils::getAssetsDir().append("default.vert")), 
        utils::loadFileToString(utils::getAssetsDir().append("default.frag"))
    );

    size_t meshCount = 0;
    rendering::Mesh** meshes = loadModelMeshes(meshCount);

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

        for (size_t i = 0; i < meshCount; i++)
            renderer.draw(*meshes[i], program, camera);

        renderer.present();
    }

    for (size_t i = 0; i < meshCount; ++i)
        delete meshes[i];
    delete[] meshes;

    return 0;
}