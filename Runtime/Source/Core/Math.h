#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {
	using Vec1 = glm::vec1;
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;

	using Vec1i = glm::i32vec1;
	using Vec2i = glm::i32vec2;
	using Vec3i = glm::i32vec3;
	using Vec4i = glm::i32vec4;

	using Mat2 = glm::mat2;
	using Mat3 = glm::mat3;
	using Mat4 = glm::mat4;

	template <typename T>
	auto getPointer(const T& value) -> decltype(glm::value_ptr(value)) { return glm::value_ptr(value); }

	template <typename T>
	auto translate(const T& matrix, const glm::vec3& translation) -> decltype(glm::translate(matrix, translation)) { return glm::translate(matrix, translation); }

	template <typename T>
	auto rotate(const T& matrix, float angle, const glm::vec3& axis) -> decltype(glm::rotate(matrix, angle, axis)) { return glm::rotate(matrix, angle, axis); }

	template <typename T>
	auto scale(const T& matrix, const glm::vec3& scale) -> decltype(glm::scale(matrix, scale)) { return glm::scale(matrix, scale); }

	template <typename T>
	auto orthographic(T left, T right, T bottom, T top, T near, T far) -> decltype(glm::orthoLH(left, right, bottom, top, near, far)) { return glm::orthoLH(left, right, bottom, top, near, far); }

	template <typename T>
	auto perspective(T fovY, T aspect, T near, T far) -> decltype(glm::perspectiveLH(fovY, aspect, near, far)) { return glm::perspectiveLH(fovY, aspect, near, far); }
}