#include "Core/Window.h"

int main() {
	auto window = engine::Window("Test Window", engine::Vec2i(1920, 1080), false, true);

	while (true)
	{
		window.present();
	}

	return 0;
}