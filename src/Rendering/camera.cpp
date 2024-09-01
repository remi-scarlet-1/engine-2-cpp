#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

static const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
static const float YAW_FIX_OFFSET = 90;

engine::rendering::Camera::Camera(float moveSpeed, float lookSensitivity,
				float fovy, float aspectRatio, float nearClip, float farClip)
	  : moveSpeed(moveSpeed), lookSensitivity(lookSensitivity),
		fovy(fovy), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip),

		position(0.0f, 0.0f, -2.0f), front(0.0f, 0.0f, -1.0f), up(WORLD_UP),
		right(glm::normalize(glm::cross(front, up))),
		yaw(0.0f), pitch(0.0f),
		projectionDirty(true), viewDirty(true)
{
	updateVectors();
}

const glm::vec3& engine::rendering::Camera::getPosition()
{
	return position;
}

const glm::mat4& engine::rendering::Camera::getViewMatrix()
{
	if (viewDirty) {
		view = glm::lookAtLH(position, position + front, up); // Use LH variant for left-handed coord system
		viewDirty = false;
	}

	return view;
}

const glm::mat4& engine::rendering::Camera::getProjectionMatrix()
{
	if (projectionDirty) {
		projection = glm::perspectiveLH(glm::radians(fovy), aspectRatio, nearClip, farClip); // Use LH variant for left-handed coord system
		projectionDirty = false;
	}

	return projection;
}

void engine::rendering::Camera::updateMovement(const glm::vec3& input, float deltaTime)
{
	if (glm::length(input) == 0.0f) // Fix for multiplying a 0 length vector (glm hates it and gives NaN yapping)
		return;

	glm::vec3 faceDir = front * input.z + right * input.x + up * input.y;
	position += glm::normalize(faceDir) * moveSpeed * deltaTime;
	viewDirty = true;
}

void engine::rendering::Camera::updateMouselook(int xDelta, int yDelta)
{
	yaw += xDelta * lookSensitivity;
	pitch += yDelta * lookSensitivity;

	// Clamp pitch
	const float limit = 89.9f;
	if (pitch > limit) pitch = limit;
	if (pitch < -limit) pitch = -limit;

	updateVectors();
}

void engine::rendering::Camera::updateVectors()
{
	// Yaw offset is needed to make camera front face Z+ in an left-handed coord system
	front.x = cos(glm::radians(-yaw + YAW_FIX_OFFSET)) * cos(glm::radians(-pitch));
	front.y = sin(glm::radians(-pitch));
	front.z = sin(glm::radians(-yaw + YAW_FIX_OFFSET)) * cos(glm::radians(-pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(WORLD_UP, front));
	up = glm::normalize(glm::cross(front, right));

	viewDirty = true;
}
