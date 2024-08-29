#include <iostream>
#include <chrono>
#include <string>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "utils/filesystem.h"
#include "utils/time.h"

GLuint uploadTestBoxToGPU(unsigned int& indexCount)
{
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
        2, 1, 0,
        3, 1, 2,

        // Front
        6, 4, 5,
        5, 7, 6,

        // Left
        4, 0, 5,
        1, 5, 0,

        // Right
        2, 6, 3,
        7, 3, 6,

        // Bottom
        0, 4, 2,
        6, 2, 4,

        // Top
        1, 3, 5,
        7, 5, 3
    };

    GLuint vao, vbo, ebo;

    // Create and bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and bind VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and bind EBO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    indexCount = 36;

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO and VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vao;
}

GLuint uploadTestModeltoGPU(unsigned int& indexCount) {
    engine::Model* model = engine::loadModelGLTF(engine::utils::getAssetsDir().append("sponza/sponza.glb"));
    const engine::MeshPrimitive* primitive = model->meshes[0]->meshPrimitives[0];

    GLuint vao, vbo, ebo;

    // Create and bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and bind VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, primitive->numPositions * sizeof(glm::vec3), primitive->bPositions, GL_STATIC_DRAW);

    // Create and bind EBO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitive->numIndices * sizeof(unsigned int), primitive->bIndices, GL_STATIC_DRAW);
    indexCount = primitive->numIndices;

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO and VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vao;
}

int main() {
    sf::Window window(sf::VideoMode(1920, 1080), "Engine Test Window", (sf::Style::Titlebar | sf::Style::Close), sf::ContextSettings(24, 8, 4, 4, 6));
    window.setFramerateLimit(500);

    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Print OpenGL version
    std::cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Load camera
    engine::Camera camera = engine::Camera(5.0f, 0.25f, 68.0f, (16.0f / 9.0f), 0.001f, 1000.0f);

    // Load mesh
    unsigned int indexCount1;
    unsigned int indexCount2;
    GLuint vao1 = uploadTestBoxToGPU(indexCount1);
    GLuint vao2 = uploadTestModeltoGPU(indexCount2);

    // Load shader
    GLuint vertexShader = engine::compileShader(GL_VERTEX_SHADER, engine::utils::loadFileToString(engine::utils::getAssetsDir().append("default.vert")));
    GLuint fragmentShader = engine::compileShader(GL_FRAGMENT_SHADER, engine::utils::loadFileToString(engine::utils::getAssetsDir().append("default.frag")));
    GLuint shaderProgram = engine::linkProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int lastMousePosX = 0;
    int lastMousePosY = 0;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW); // Seems to be inverted (GL_CW -> GL_CCW) because of the engines use of a left-hand coordinate system
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update camera position & look
        glm::vec3 moveInput = glm::vec3(0.0f, 0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveInput.z = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveInput.z = -1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveInput.x = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveInput.x = -1.0f;

        int xDelta = sf::Mouse::getPosition().x - lastMousePosX;
        int yDelta = sf::Mouse::getPosition().y - lastMousePosY;
        lastMousePosX = sf::Mouse::getPosition().x;
        lastMousePosY = sf::Mouse::getPosition().y;

        camera.updateMovement(moveInput, engine::utils::getDeltaTime());
        camera.updateMouselook(xDelta, yDelta);

        // Clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use program and draw test
        glUseProgram(shaderProgram);
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        // Update matrices and pass them to the shader
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Render
        glBindVertexArray(vao2);
        glDrawElements(GL_TRIANGLES, indexCount2, GL_UNSIGNED_INT, 0);

        // Swap buffers
        window.display();
        engine::utils::updateTime();
    }

    // Cleanup
    glDeleteProgram(shaderProgram);

    return 0;
}