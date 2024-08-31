#include "model.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE // Replaced with mock load
#define TINYGLTF_NO_STB_IMAGE_WRITE // Replaced with mock read
#include <TinyGLTF/tiny_gltf.h>
#include <string>
#include <iostream>
#include <stdexcept>

bool MockLoadImageData(tinygltf::Image* image, const int image_idx, std::string* err, std::string* warn, int req_width, int req_height, const unsigned char* bytes, int size, void* user_data) { return true; }
bool MockWriteImageData(const std::string* basepath, const std::string* filename, const tinygltf::Image* image, bool embedImages, const tinygltf::FsCallbacks* fs_cb, const tinygltf::URICallbacks* uri_cb, std::string* out_uri, void* user_pointer) { return true; }

template<typename T>
std::vector<T> copyVertexBuffer(const tinygltf::Accessor& accessor, const tinygltf::Buffer& buffer, const tinygltf::BufferView& bufferView) {
    const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

    std::vector<T> targetVector(accessor.count);
    memcpy(targetVector.data(), dataPtr, accessor.count * sizeof(T));

    return targetVector;
}

engine::assets::MeshPrimitive loadPrimitive(const tinygltf::Model& tinyModel, const tinygltf::Primitive& tinyPrimitive) {
    std::vector<unsigned int> indices;
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;

    // Indices
    const tinygltf::Accessor& indexAccessor = tinyModel.accessors[tinyPrimitive.indices];
    const tinygltf::BufferView& indexBufferView = tinyModel.bufferViews[indexAccessor.bufferView];
    const tinygltf::Buffer& indexBuffer = tinyModel.buffers[indexBufferView.buffer];

    switch (indexAccessor.componentType) {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
            auto indexData = copyVertexBuffer<unsigned short>(indexAccessor, indexBuffer, indexBufferView);
            indices.resize(indexAccessor.count);
            for (size_t i = 0; i < indexAccessor.count; ++i)
                indices[i] = static_cast<unsigned int>(indexData[i]);
            break;
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
            indices = copyVertexBuffer<unsigned int>(indexAccessor, indexBuffer, indexBufferView);
            break;
        }
        default:
            throw std::runtime_error("Unsupported index component type");
    }

    // Positions
    if (tinyPrimitive.attributes.count("POSITION") > 0) {
        const tinygltf::Accessor& positionAccessor = tinyModel.accessors[tinyPrimitive.attributes.at("POSITION")];
        const tinygltf::BufferView& positionBufferView = tinyModel.bufferViews[positionAccessor.bufferView];
        const tinygltf::Buffer& positionBuffer = tinyModel.buffers[positionBufferView.buffer];
        positions = copyVertexBuffer<float>(positionAccessor, positionBuffer, positionBufferView);
    }

    // Normals
    if (tinyPrimitive.attributes.count("NORMAL") > 0) {
        const tinygltf::Accessor& normalAccessor = tinyModel.accessors[tinyPrimitive.attributes.at("NORMAL")];
        const tinygltf::BufferView& normalBufferView = tinyModel.bufferViews[normalAccessor.bufferView];
        const tinygltf::Buffer& normalBuffer = tinyModel.buffers[normalBufferView.buffer];
        normals = copyVertexBuffer<float>(normalAccessor, normalBuffer, normalBufferView);
    }

    // UVs
    if (tinyPrimitive.attributes.count("TEXCOORD_0") > 0) {
        const tinygltf::Accessor& uvAccessor = tinyModel.accessors[tinyPrimitive.attributes.at("TEXCOORD_0")];
        const tinygltf::BufferView& uvBufferView = tinyModel.bufferViews[uvAccessor.bufferView];
        const tinygltf::Buffer& uvBuffer = tinyModel.buffers[uvBufferView.buffer];
        uvs = copyVertexBuffer<float>(uvAccessor, uvBuffer, uvBufferView);
    }

    return engine::assets::MeshPrimitive(std::move(indices), std::move(positions), std::move(normals), std::move(uvs));
}

engine::assets::Model* engine::assets::loadModelGLTF(const std::filesystem::path& path) {
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
    std::vector<engine::assets::Mesh> meshes;
    meshes.reserve(tinyModel.meshes.size());

    for (const auto& tinyMesh : tinyModel.meshes) {
        std::vector<engine::assets::MeshPrimitive> primitives;
        primitives.reserve(tinyMesh.primitives.size());

        for (const auto& tinyPrimitive : tinyMesh.primitives)
            primitives.emplace_back(loadPrimitive(tinyModel, tinyPrimitive));

        meshes.emplace_back(std::move(primitives));
    }

    return new engine::assets::Model(std::move(meshes));

}
