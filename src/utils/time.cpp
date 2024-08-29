#include "utils/time.h"

#include <chrono>

auto previousTime = std::chrono::high_resolution_clock::now();
float deltaTime;

void engine::utils::updateTime()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = currentTime - previousTime;
	previousTime = currentTime;

	deltaTime = duration.count();
}

float engine::utils::getDeltaTime()
{
	return deltaTime;
}