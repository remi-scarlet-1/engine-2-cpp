#include "time.h"

#include <chrono>
#include <iostream>

auto previousTime = std::chrono::high_resolution_clock::now();
float deltaT = 0.0f;

void engine::utils::updateTime()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = currentTime - previousTime;
	previousTime = currentTime;

	deltaT = duration.count();
}

float engine::utils::deltaTime()
{
	return deltaT;
}
