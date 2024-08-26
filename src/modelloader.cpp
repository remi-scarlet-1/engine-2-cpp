#include "modelloader.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <TinyGLTF/tiny_gltf.h>

#include <glad/glad.h>
#include <iostream>

using namespace tinygltf;

unsigned int modelloader::LoadMeshVAO(const char* path)
{
    // Load the GLTF model
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err, warn;

    bool success = loader.LoadASCIIFromFile(&model, &err, &warn, path);  // Or LoadBinaryFromFile for GLB
    if (!warn.empty()) {
        std::cout << "Warning: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << "Error: " << err << std::endl;
    }
    if (!success) {
        std::cerr << "Failed to load GLTF/GLB file" << std::endl;
        return 0;
    }

    // Assuming the first mesh and primitive for simplicity
    const tinygltf::Mesh& mesh = model.meshes[0];
    const tinygltf::Primitive& primitive = mesh.primitives[0];

    // Prepare OpenGL buffers
    GLuint VAO, VBO_position, VBO_normal, VBO_uv, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Extract position, normal, and UV attributes if available
    if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
        const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("POSITION")->second];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        // Create VBO for positions
        glGenBuffers(1, &VBO_position);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
        glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);

        // Set up vertex attribute pointer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Assuming vec3 for positions
        glEnableVertexAttribArray(0);
    }

    if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
        const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("NORMAL")->second];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        // Create VBO for normals
        glGenBuffers(1, &VBO_normal);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
        glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);

        // Set up vertex attribute pointer
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Assuming vec3 for normals
        glEnableVertexAttribArray(1);
    }

    if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
        const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        // Create VBO for UVs
        glGenBuffers(1, &VBO_uv);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
        glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);

        // Set up vertex attribute pointer
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // Assuming vec2 for UVs
        glEnableVertexAttribArray(2);
    }

    // Extract and upload index data if present
    if (primitive.indices >= 0) {
        const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);
    }

    // Unbind VAO
    glBindVertexArray(0);

    return VAO;
}
