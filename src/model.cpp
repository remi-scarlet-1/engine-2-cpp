#include "model.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <TinyGLTF/tiny_gltf.h>
#include <string>
#include <iostream>

/// <summary>
/// Copies a TinyGLTF buffer into the provided array. Make sure size matches!
/// </summary>
template<typename T>
void copyTinyGLTFBuffer(const tinygltf::Accessor& accessor, const tinygltf::Buffer& buffer, const tinygltf::BufferView& bufferView, T* targetArray) {
    const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
    memcpy(targetArray, dataPtr, accessor.count * sizeof(T));
}

model::Primitive* loadTinyGLTFPrimitive(const tinygltf::Model& tinyModel, const tinygltf::Primitive& tinyPrimitive)
{
    // Extract sizes of various attributes
    std::size_t numIndices = tinyModel.accessors[tinyPrimitive.indices].count;
    std::size_t numPositions = tinyPrimitive.attributes.count("POSITION") > 0 ? tinyModel.accessors[tinyPrimitive.attributes.at("POSITION")].count : 0;
    std::size_t numNormals = tinyPrimitive.attributes.count("NORMAL") > 0 ? tinyModel.accessors[tinyPrimitive.attributes.at("NORMAL")].count : 0;
    std::size_t numUVs = tinyPrimitive.attributes.count("TEXCOORD_0") > 0 ? tinyModel.accessors[tinyPrimitive.attributes.at("TEXCOORD_0")].count : 0;
    
    // Create raw arrays to store data
    unsigned int* bIndices = new unsigned int[numIndices];
    glm::vec3* bPositions = numPositions > 0 ? new glm::vec3[numPositions] : nullptr;
    glm::vec3* bNormals = numNormals > 0 ? new glm::vec3[numNormals] : nullptr;
    glm::vec2* bUVs = numUVs > 0 ? new glm::vec2[numUVs] : nullptr;

    // Indices
    const tinygltf::Accessor& indexAccessor = tinyModel.accessors[tinyPrimitive.indices];
    const tinygltf::BufferView& indexBufferView = tinyModel.bufferViews[indexAccessor.bufferView];
    const tinygltf::Buffer& indexBuffer = tinyModel.buffers[indexBufferView.buffer];
    const unsigned char* indexDataPtr = indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset;

    switch (indexAccessor.componentType) {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: // In case of ushort (16-bit) format
            for (std::size_t i = 0; i < numIndices; ++i)
                bIndices[i] = reinterpret_cast<const unsigned short*>(indexDataPtr)[i];
            break;

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: // In case of uint (32-bit) format
            for (std::size_t i = 0; i < numIndices; ++i)
                bIndices[i] = reinterpret_cast<const unsigned int*>(indexDataPtr)[i];
            break;

        default: // Fail if neither ushort (16-bit) nor uint (32-bit) format matches
            std::cerr << "Unsupported index component type" << std::endl;
            delete[] bIndices;
            if (bPositions) delete[] bPositions;
            if (bNormals) delete[] bNormals;
            if (bUVs) delete[] bUVs;
            return nullptr;
    }

    // Positions
    if (numPositions > 0) {
        const tinygltf::Accessor& positionAccessor = tinyModel.accessors[tinyPrimitive.attributes.at("POSITION")];
        const tinygltf::BufferView& positionBufferView = tinyModel.bufferViews[positionAccessor.bufferView];
        const tinygltf::Buffer& positionBuffer = tinyModel.buffers[positionBufferView.buffer];
        copyTinyGLTFBuffer(positionAccessor, positionBuffer, positionBufferView, bPositions);
    }

    // Normals
    if (numNormals > 0) {
        const tinygltf::Accessor& normalAccessor = tinyModel.accessors[tinyPrimitive.attributes.at("NORMAL")];
        const tinygltf::BufferView& normalBufferView = tinyModel.bufferViews[normalAccessor.bufferView];
        const tinygltf::Buffer& normalBuffer = tinyModel.buffers[normalBufferView.buffer];
        copyTinyGLTFBuffer(normalAccessor, normalBuffer, normalBufferView, bNormals);
    }

    // UVs
    if (numUVs > 0) {
        const tinygltf::Accessor& uvAccessor = tinyModel.accessors[tinyPrimitive.attributes.at("TEXCOORD_0")];
        const tinygltf::BufferView& uvBufferView = tinyModel.bufferViews[uvAccessor.bufferView];
        const tinygltf::Buffer& uvBuffer = tinyModel.buffers[uvBufferView.buffer];
        copyTinyGLTFBuffer(uvAccessor, uvBuffer, uvBufferView, bUVs);
    }

    // Create and return a new Primitive object
    return new model::Primitive(
        numIndices, 
        numPositions, 
        numNormals, 
        numUVs, 
        bIndices, 
        bPositions, 
        bNormals, 
        bUVs
    );
}

model::Model* model::loadModelGLTF(const std::filesystem::path& path)
{
    model::Model* model = new Model();

    // TinyGLTF
    tinygltf::Model tinyModel;
    tinygltf::TinyGLTF tinyLoader;
    std::string err;
    std::string warn;

    // Load GLTF file
    bool success = tinyLoader.LoadASCIIFromFile(&tinyModel, &err, &warn, path.string());
    if (!success) {
        std::cerr << "Failed to load GLTF file: " << err << std::endl;
        return model;
    }

    // Iterate through meshes
    for (const tinygltf::Mesh& tinyMesh : tinyModel.meshes) {
        model::Mesh* mesh = new Mesh();

        // Iterate through primitives
        for (const tinygltf::Primitive& tinyPrimitive : tinyMesh.primitives) {
            model::Primitive* primitive = loadTinyGLTFPrimitive(tinyModel, tinyPrimitive);

            if (primitive)
                mesh->primitives.push_back(primitive);
        }

        model->meshes.push_back(mesh);
    }

    return model;
}
