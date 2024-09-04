#pragma once

#include <memory>
#include <vector>
#include <entt/entt.hpp>

#include "ECSSystem.h"

namespace engine {
	class ECSManager
	{
	public:
		ECSManager() = default;
		~ECSManager() = default;

		void updateSystems(float deltaTime);
		void registerSystem(std::unique_ptr<ISystem> system);

	private:
		entt::registry registry;
		std::vector<std::unique_ptr<ISystem>> systems;
	};
}