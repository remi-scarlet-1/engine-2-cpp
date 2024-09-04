#include "ECSManager.h"

void engine::ECSManager::updateSystems(float deltaTime)
{
	for (auto& system : systems)
		system->update(registry, deltaTime);
}

void engine::ECSManager::registerSystem(std::unique_ptr<ISystem> system)
{
	systems.push_back(std::move(system));
}