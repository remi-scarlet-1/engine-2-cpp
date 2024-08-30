#include "renderer.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

engine::graphics::Renderer::Renderer(const char* windowTitle, glm::i16vec2 windowRes, bool windowResizable, unsigned int frameLimit)
{
    window = new sf::Window(
        sf::VideoMode(windowRes.x, windowRes.y), windowTitle, windowResizable ? sf::Style::Default : (sf::Style::Titlebar | sf::Style::Close),
        sf::ContextSettings(24, 8, 4, 4, 6)
    );

    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    std::cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

void engine::graphics::Renderer::initialize()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // This is inverted because of the engines left-handed coord system (GL_CW becomes GL_CCW)
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glDepthFunc(GL_LESS);
}

void engine::graphics::Renderer::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void engine::graphics::Renderer::draw(Mesh& mesh, Program& program, Camera& camera)
{
    glUseProgram(program.getHandle());

    GLuint modelLoc, viewLoc, projectionLoc;
    program.getLocations(modelLoc, viewLoc, projectionLoc);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(mesh.getHandle());
    glDrawElements(GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_INT, 0);
}

void engine::graphics::Renderer::present()
{
    for (const auto& callback : onFrameEndCallbacks)
        callback();

    window->display();
}
