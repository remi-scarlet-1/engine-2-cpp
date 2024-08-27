#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <glad/glad.h>
#include <iostream>
#include <chrono>
#include <string>

#include "file.h"
#include "model.h"
#include "shader.h"

void testGLTFModelLoading() {
    // Create testModel path
    std::filesystem::path testModelPath = file::getExecutablePath().parent_path().append("assets/sponza/sponza_texcompress_75jpeg.glb");
    
    // Keep time & load testModel
    auto start = std::chrono::high_resolution_clock::now();
    model::Model* testModel = model::loadGLBModel(testModelPath);
    auto end = std::chrono::high_resolution_clock::now();

    // Print testModel mesh & sub-meshes data
    std::size_t totalMeshes = 0;
    std::size_t totalPrimitives = 0;

    for (std::size_t meshIdx = 0; meshIdx < testModel->meshes.size(); ++meshIdx) {
        const model::Mesh* mesh = testModel->meshes[meshIdx];
        if (!mesh) {
            std::cerr << "Mesh at index " << meshIdx << " is null." << std::endl;
            continue;
        }

        std::size_t numPrimitives = mesh->primitives.size();
        totalMeshes++;
        totalPrimitives += numPrimitives;

        std::cout << "Mesh " << meshIdx << " has " << numPrimitives << " primitives." << std::endl;

        for (std::size_t primIdx = 0; primIdx < numPrimitives; ++primIdx) {
            const model::Primitive* primitive = mesh->primitives[primIdx];
            if (!primitive) {
                std::cerr << "Primitive at index " << primIdx << " in Mesh " << meshIdx << " is null." << std::endl;
                continue;
            }

            std::cout << "  Primitive " << primIdx << ":" << std::endl;
            std::cout << "    Indices count: " << primitive->numIndices << std::endl;
            std::cout << "    Positions count: " << primitive->numPositions << std::endl;
            std::cout << "    Normals count: " << primitive->numNormals << std::endl;
            std::cout << "    UVs count: " << primitive->numUVs << std::endl;
            std::cout << "    Has Normals: " << (primitive->hasNormals ? "Yes" : "No") << std::endl;
            std::cout << "    Has UVs: " << (primitive->hasUvs ? "Yes" : "No") << std::endl;
        }
    }

    // Print total mesh & sub-mesh counts
    std::cout << "Total Meshes: " << totalMeshes << std::endl;
    std::cout << "Total Primitives: " << totalPrimitives << std::endl;

    // Print total time
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    delete testModel;
}

int main() {
    // Test GLTF loading
    testGLTFModelLoading();
    return 0;

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

    // Create and compile shader program
    GLuint vertexShader = shader::compileShader(GL_VERTEX_SHADER, file::readFileToString("assets/default.vert"));
    GLuint fragmentShader = shader::compileShader(GL_FRAGMENT_SHADER, file::readFileToString("assets/default.frag"));
    GLuint shaderProgram = shader::linkProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use program and draw test
        glUseProgram(shaderProgram);
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers
        window.display();
    }

    // Cleanup
    glDeleteProgram(shaderProgram);;

    return 0;
}