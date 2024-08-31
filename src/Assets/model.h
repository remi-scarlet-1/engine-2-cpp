#pragma once

#include <vector>
#include <filesystem>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace engine::assets {
    /// <summary>
    /// Represents a single mesh primitive (aka. submesh) of a mesh.
    /// </summary>
    struct MeshPrimitive {
        const std::vector<unsigned int> Indices;
        const std::vector<float> Positions;
        const std::vector<float> Normals;
        const std::vector<float> UVs;

        // Move constructor for memory efficiency (copies are disallowed)
        MeshPrimitive(
            std::vector<unsigned int>&& indices,
            std::vector<float>&& positions,
            std::vector<float>&& normals,
            std::vector<float>&& uvs) noexcept
            : 
                Indices(std::move(indices)),
                Positions(std::move(positions)),
                Normals(std::move(normals)),
                UVs(std::move(uvs))
        {}

        ~MeshPrimitive() = default;
    };

    /// <summary>
    /// Represents a mesh of a model, with all its primitives.
    /// </summary>
    struct Mesh {
        const std::vector<MeshPrimitive> MeshPrimitives;

        // Move constructor for memory efficiency (copies are disallowed)
        Mesh(std::vector<MeshPrimitive>&& meshPrimitives) noexcept : MeshPrimitives(std::move(meshPrimitives)) {}
        ~Mesh() = default;
    };

    /// <summary>
    /// Represents a model.
    /// </summary>
    struct Model {
        const std::vector<Mesh> Meshes;

        // Move constructor for memory efficiency (copies are disallowed)
        Model(std::vector<Mesh>&& meshes) noexcept : Meshes(std::move(meshes)) {}
        ~Model() = default;
    };

	/// <summary>
	/// Load's a GLTF/GLB model from file.
	/// </summary>
	/// <returns>Raw Model pointer.</returns>
	Model* loadModelGLTF(const std::filesystem::path &path);
}