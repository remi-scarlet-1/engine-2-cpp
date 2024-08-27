#pragma once

#include <vector>
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
        std::vector<const Primitive*> primitives;

        Mesh() = default;
        ~Mesh() {
            for (const Primitive* primitive : primitives)
                delete primitive;
        }

        // Delete copy and move constructors/assignments to prevent copying
        Mesh(const Mesh&) = delete; Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&&) = delete; Mesh& operator=(Mesh&&) = delete;
    };

    /// <summary>
    /// Represents a model.
    /// </summary>
    struct Model {
        std::vector<const Mesh*> meshes;

        Model() = default;
        ~Model() {
            for (const Mesh* mesh : meshes)
                delete mesh;
        }

        // Delete copy and move constructors/assignments to prevent copying
        Model(const Model&) = delete; Model& operator=(const Model&) = delete;
        Model(Model&&) = delete; Model& operator=(Model&&) = delete;
    };

	/// <summary>
	/// Load's a GLB model from disk. <para></para>
    /// (Quantized models are NOT supported)
	/// </summary>
	/// <returns>Raw Model pointer.</returns>
	Model* loadGLBModel(const std::filesystem::path &path);

    /// <summary>
    /// Load's a GLTF model from disk. <para></para>
    /// (Quantized models are NOT supported)
    /// </summary>
    /// <returns>Raw Model pointer.</returns>
	Model* loadGLTFModel(const std::filesystem::path &path);
}