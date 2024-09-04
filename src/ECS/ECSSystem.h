#pragma once

#include <entt/entity/registry.hpp>

namespace engine {
	class ISystem {
	public:
		virtual ~ISystem() = default;
		virtual void update(entt::registry& registry, float deltaTime) = 0;
	};
}