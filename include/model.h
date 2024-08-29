#pragma once

#include <cstddef>
#include <filesystem>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace engine {
    /// <summary>
    /// Represents a single mesh primitive (aka. submesh) of a mesh.
    /// </summary>
    struct MeshPrimitive {
        const std::size_t numIndices;
        const std::size_t numPositions;
        const std::size_t numNormals;
        const std::size_t numUVs;

        const unsigned int* const bIndices;
        const glm::vec3* const bPositions;
        const glm::vec3* const bNormals;
        const glm::vec2* const bUVs;

        const bool hasNormals;
        const bool hasUvs;

        // Constructor to initialize all fields
        MeshPrimitive(
            const std::size_t numIndices, 
            const std::size_t numPositions, 
            const std::size_t numNormals, 
            const std::size_t numUvs, 

            const unsigned int* const bIndices, 
            const glm::vec3* const bPositions, 
            const glm::vec3* const bNormals, 
            const glm::vec2* const bUVs
        )
            :   numIndices(numIndices),
                numPositions(numPositions),
                numNormals(numNormals),
                numUVs(numUvs),

                bIndices(bIndices),
                bPositions(bPositions),
                bNormals(bNormals),
                bUVs(bUVs),

                hasNormals(numNormals > 0),
                hasUvs(numUvs > 0)
        {}

        // Destructor to free allocated memory
        ~MeshPrimitive() {
            delete[] bIndices;
            delete[] bPositions;
            delete[] bNormals;
            delete[] bUVs;
        }

        // Delete copy and move constructors/assignments to prevent copying
        MeshPrimitive(const MeshPrimitive&) = delete; MeshPrimitive& operator=(const MeshPrimitive&) = delete;
        MeshPrimitive(MeshPrimitive&&) = delete; MeshPrimitive& operator=(MeshPrimitive&&) = delete;
    };

    /// <summary>
    /// Represents a mesh of a model, with all its primitives.
    /// </summary>
    struct Mesh {
        const std::size_t numMeshPrimitives;
        const MeshPrimitive* const* const meshPrimitives;

        Mesh(std::size_t numMeshPrimitives, const MeshPrimitive** meshPrimitives)
            : numMeshPrimitives(numMeshPrimitives), meshPrimitives(meshPrimitives) {}

        ~Mesh() {
            for (std::size_t i = 0; i < numMeshPrimitives; ++i)
                delete meshPrimitives[i];
            delete[] meshPrimitives;
        }

        // Delete copy and move constructors/assignments to prevent copying
        Mesh(const Mesh&) = delete; Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&&) = delete; Mesh& operator=(Mesh&&) = delete;
    };

    /// <summary>
    /// Represents a model.
    /// </summary>
    struct Model {
        const std::size_t numMeshes;
        const Mesh* const* const meshes;

        Model(std::size_t numMeshes, const Mesh** meshes)
            : numMeshes(numMeshes), meshes(meshes) {}

        ~Model() {
            for (std::size_t i = 0; i < numMeshes; ++i)
                delete meshes[i];
            delete[] meshes;
        }

        // Delete copy and move constructors/assignments to prevent copying
        Model(const Model&) = delete; Model& operator=(const Model&) = delete;
        Model(Model&&) = delete; Model& operator=(Model&&) = delete;
    };

	/// <summary>
	/// Load's a GLTF/GLB model from file.
	/// </summary>
	/// <returns>Raw Model pointer.</returns>
	Model* loadModelGLTF(const std::filesystem::path &path);
}