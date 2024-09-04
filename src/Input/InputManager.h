#pragma once

#include "Graphics/Window.h"
#include "Util/Math.h"

namespace engine {
	class InputManager
	{
	public:
		InputManager(Window* window);
		~InputManager();

		void processEvents();

		Vector2 getMoveInput();
		Vector2 getLookInput();

	private:
		Window* window;

		Vector2 move;

		Vector2 mouseDelta;
		Vector2 mouseLocation;
		Vector2 lastMouseLocation;
	};
}