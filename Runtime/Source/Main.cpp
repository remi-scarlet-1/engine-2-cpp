#include "Core/Window.h"

#include <iostream>

int main() {
	auto window = engine::Window("Test Window", engine::Vec2i(1920, 1080), false, true);

	while (!window.getShouldClose())
	{
		window.pollEvents();
		window.present();
	}

	window.close();

	return 0;
}