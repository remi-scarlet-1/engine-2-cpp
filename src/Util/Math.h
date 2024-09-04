#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

namespace engine {
    // Basic vector types
    using Vector2 = glm::vec2;
    using Vector2i = glm::i32vec2;
    using Vector3 = glm::vec3;
    using Vector3i = glm::i32vec3;
    using Vector4 = glm::vec4;

    // Matrix types
    using Matrix3 = glm::mat3;
    using Matrix4 = glm::mat4;

    // Quaternion type
    using Quaternion = glm::quat;

    // Math constants
    constexpr float PI = glm::pi<float>();
}