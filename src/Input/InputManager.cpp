#include "inputmanager.h"

#include "GLFW/glfw3.h"

engine::InputManager::InputManager(Window* window): 
	window(window), 

	move(Vector3(0.0f)), 

    mouseDelta(Vector2(0)),
    mouseLocation(Vector2(0)),
    lastMouseLocation(Vector2(0))
{

}

engine::InputManager::~InputManager()
{

}

void engine::InputManager::processEvents()
{
	window->pollEvents();

    // Move input
	move = Vector3(0.0f);
    if (glfwGetKey(window->getWindowHandle(), GLFW_KEY_D) == GLFW_PRESS) move.x = 1.0f;
    if (glfwGetKey(window->getWindowHandle(), GLFW_KEY_A) == GLFW_PRESS) move.x = -1.0f;
    if (glfwGetKey(window->getWindowHandle(), GLFW_KEY_W) == GLFW_PRESS) move.y = 1.0f;
    if (glfwGetKey(window->getWindowHandle(), GLFW_KEY_S) == GLFW_PRESS) move.y = -1.0f;

    if (glm::length(move) > 0.0f)
        move = glm::normalize(move);

    // Mouse input
    double mouseX, mouseY;
    glfwGetCursorPos(window->getWindowHandle(), &mouseX, &mouseY);

    mouseLocation = Vector2(mouseX, mouseY);
    mouseDelta = mouseLocation - lastMouseLocation;
    lastMouseLocation = mouseLocation;
}

engine::Vector2 engine::InputManager::getMoveInput() { return move; }
engine::Vector2 engine::InputManager::getLookInput() { return mouseDelta; }
