#pragma once
#include "ECS/ECSSystem.h"

#include <iostream>

namespace engine::systems {
	class TransformSystem : public ISystem {
	public:
		inline void update(entt::registry& registry, float deltaTime) {
			// Update code
		}
	};
}