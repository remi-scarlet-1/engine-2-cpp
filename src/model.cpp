#include "model.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE // Replaced with mock load
#define TINYGLTF_NO_STB_IMAGE_WRITE // Replaced with mock read
#include <TinyGLTF/tiny_gltf.h>
#include <string>
#include <iostream>

bool MockLoadImageData(tinygltf::Image* image, const int image_idx, std::string* err, std::string* warn, int req_width, int req_height, const unsigned char* bytes, int size, void* user_data) { return true;}
bool MockWriteImageData(const std::string* basepath, const std::string* filename, const tinygltf::Image* image, bool embedImages, const tinygltf::FsCallbacks* fs_cb, const tinygltf::URICallbacks* uri_cb, std::string* out_uri, void* user_pointer) { return true;}

/// <summary>
/// Copies a TinyGLTF buffer into the provided array. Make sure size matches!
/// </summary>
template<typename T>
void copyVertexBuffer(const tinygltf::Accessor& accessor, const tinygltf::Buffer& buffer, const tinygltf::BufferView& bufferView, T* targetArray) {
    const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
    memcpy(targetArray, dataPtr, accessor.count * sizeof(T));
}

engine::Primitive* loadPrimitive(const tinygltf::Model& tinyModel, const tinygltf::Primitive& tinyPrimitive)
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
        copyVertexBuffer(positionAccessor, positionBuffer, positionBufferView, bPositions);
    }

    // Normals
    if (numNormals > 0) {
        const tinygltf::Accessor& normalAccessor = tinyModel.accessors[tinyPrimitive.attributes.at("NORMAL")];
        const tinygltf::BufferView& normalBufferView = tinyModel.bufferViews[normalAccessor.bufferView];
        const tinygltf::Buffer& normalBuffer = tinyModel.buffers[normalBufferView.buffer];
        copyVertexBuffer(normalAccessor, normalBuffer, normalBufferView, bNormals);
    }

    // UVs
    if (numUVs > 0) {
        const tinygltf::Accessor& uvAccessor = tinyModel.accessors[tinyPrimitive.attributes.at("TEXCOORD_0")];
        const tinygltf::BufferView& uvBufferView = tinyModel.bufferViews[uvAccessor.bufferView];
        const tinygltf::Buffer& uvBuffer = tinyModel.buffers[uvBufferView.buffer];
        copyVertexBuffer(uvAccessor, uvBuffer, uvBufferView, bUVs);
    }

    // Create and return a new Primitive object
    return new engine::Primitive(
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

engine::Model* engine::loadModelGLTF(const std::filesystem::path& path)
{
    // Setup
    tinygltf::Model tinyModel;
    tinygltf::TinyGLTF tinyLoader;
    tinyLoader.SetImageLoader(MockLoadImageData, nullptr);
    tinyLoader.SetImageWriter(MockWriteImageData, nullptr);

    // Attempt load
    std::string extension = path.extension().string();
    bool success = false;
    std::string err;
    std::string warn;
    
    if (extension == ".gltf") success = tinyLoader.LoadASCIIFromFile(&tinyModel, &err, &warn, path.string());
    if (extension == ".glb") success = tinyLoader.LoadBinaryFromFile(&tinyModel, &err, &warn, path.string());

    if (!success) {
        std::cerr << "Failed to load GLTF file: " << err << std::endl;
        return nullptr;
    }

    // Collect meshes
    const std::size_t numMeshes = tinyModel.meshes.size();
    const Mesh** meshes = new const Mesh*[numMeshes];

    for (size_t i = 0; i < numMeshes; i++) {
        // Collect primitives
        const std::size_t numPrimitives = tinyModel.meshes[i].primitives.size();
        const Primitive** primitives = new const Primitive*[numPrimitives];

        for (size_t j = 0; j < numPrimitives; j++)
        {
            const engine::Primitive* primitive = loadPrimitive(tinyModel, tinyModel.meshes[i].primitives[j]);
            if (primitive)
                primitives[j] = primitive; // Save constructed primitive
        }

        // Save constructed mesh
        meshes[i] = new Mesh(numPrimitives, primitives);
    }

    return new Model(numMeshes, meshes);
}
