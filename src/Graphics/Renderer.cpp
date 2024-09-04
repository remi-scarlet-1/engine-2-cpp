#include "renderer.h"

#include <iostream>
#include <glad/glad.h>

engine::Renderer::Renderer(Window* window): window(window)
{
    if (!gladLoadGLLoader(window->getGLFunctionLoader())) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

void engine::Renderer::initialize()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CW); // This is inverted because of the engines left-handed coord system (GL_CW becomes GL_CCW)
    glCullFace(GL_BACK);

    glDepthFunc(GL_LESS);
}

void engine::Renderer::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void engine::Renderer::drawTriangleMesh(unsigned int program, unsigned int vao, unsigned int indexCount, Matrix4 modelMatrix, Matrix4 viewMatrix, Matrix4 projectionMatrix)
{
    glUseProgram(program);

    GLuint modelLocation, viewLocation, projectionLocation;
    modelLocation = glGetUniformLocation(program, "model");
    viewLocation = glGetUniformLocation(program, "view");
    projectionLocation = glGetUniformLocation(program, "projection");

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void engine::Renderer::present()
{
    window->present();
}
