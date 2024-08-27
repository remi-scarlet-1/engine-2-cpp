#pragma once

#include <cstddef>
#include <filesystem>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace model {
    /// <summary>
    /// Represents a single mesh primitive (aka. submesh) of a mesh.
    /// </summary>
    struct Primitive {
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
        Primitive(
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
        ~Primitive() {
            delete[] bIndices;
            delete[] bPositions;
            delete[] bNormals;
            delete[] bUVs;
        }

        // Delete copy and move constructors/assignments to prevent copying
        Primitive(const Primitive&) = delete; Primitive& operator=(const Primitive&) = delete;
        Primitive(Primitive&&) = delete; Primitive& operator=(Primitive&&) = delete;
    };

    /// <summary>
    /// Represents a mesh of a model, with all its primitives.
    /// </summary>
    struct Mesh {
        const std::size_t numPrimitives;
        const Primitive* const* const primitives;

        Mesh(std::size_t numPrimitives, const Primitive** primitives)
            : numPrimitives(numPrimitives), primitives(primitives) {}

        ~Mesh() {
            for (std::size_t i = 0; i < numPrimitives; ++i)
                delete primitives[i];
            delete[] primitives;
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