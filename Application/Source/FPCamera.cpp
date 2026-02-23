#include "FPCamera.h"
#include "KeyboardController.h"
#include "MouseController.h"

//Include GLFW
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL	// It'll cause error ifndef 
#include <glm/gtx/rotate_vector.hpp>

FPCamera::FPCamera() : isDirty(false)
{
	this->position = glm::vec3(0, 0, 0);
	this->target = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);

	glm::vec3 forward = glm::normalize(target - position);

	yaw = glm::degrees(atan2(forward.z, forward.x));
	pitch = glm::degrees(asin(forward.y));

	targetYaw = yaw;
	targetPitch = pitch;
	roll = 0.f;
	targetRoll = 0.f;
}

FPCamera::~FPCamera()
{
}

void FPCamera::Init(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	this->position = position;
	this->target = target;
	this->up = up;
	this->isDirty = true;

	// Reset rotation state
	glm::vec3 forward = glm::normalize(target - position);

	yaw = glm::degrees(atan2(forward.z, forward.x));
	pitch = glm::degrees(asin(forward.y));

	targetYaw = yaw;
	targetPitch = pitch;
	roll = 0.f;
	targetRoll = 0.f;

	Refresh();
}

void FPCamera::Reset()
{
}

void FPCamera::Update(double dt)
{
	glm::vec3 forwardFlat = glm::normalize(glm::vec3(
		cos(glm::radians(yaw)),
		0.0f,
		sin(glm::radians(yaw))
	));

	glm::vec3 worldUp = glm::vec3(0, 1, 0);
	glm::vec3 right = glm::normalize(glm::cross(forwardFlat, worldUp));
	glm::vec3 moveDir = forwardFlat;
	glm::vec3 input(0, 0, 0);

	// Move Forward/Backward
	float strafeAmount = 0.0f;

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W)) {
		input += moveDir;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S)) {
		input -= moveDir;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {
		input -= right;
		strafeAmount += 1.f;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D)) {
		input += right;
		strafeAmount -= 1.f;
	}

	targetRoll = -strafeAmount * maxTilt;

	//

	if (glm::length(input) > 0.001f) {
		position += glm::normalize(input) * movementSpeed * static_cast<float>(dt);
		target += glm::normalize(input) * movementSpeed * static_cast<float>(dt);
	}

	double deltaX = MouseController::GetInstance()->GetMouseDeltaX();
	double deltaY = MouseController::GetInstance()->GetMouseDeltaY();

	targetYaw += deltaX * rotationSpeed * mouseSensitivity;
	targetPitch += deltaY * rotationSpeed * mouseSensitivity;
	targetPitch = glm::clamp(targetPitch, -89.0f, 89.0f); // clamp

	float t1 = 1.0f - exp(-rotationSmoothness * static_cast<float>(dt));
	float t2 = 1.0f - exp(-tiltSmoothness * static_cast<float>(dt));

	yaw += (targetYaw - yaw) * t1;
	pitch += (targetPitch - pitch) * t1;
	roll += (targetRoll - roll) * t2;

	glm::vec3 forward;
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	forward = glm::normalize(forward);

	glm::vec3 cameraRight = glm::normalize(glm::cross(forward, worldUp));
	glm::vec3 baseUp = glm::normalize(glm::cross(cameraRight, forward)); // true camera up

	// Apply roll around forward axis
	glm::mat4 rollMat = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(roll),
		forward
	);

	glm::vec3 finalUp = glm::normalize(glm::vec3(
		rollMat * glm::vec4(baseUp, 0.0f)
	));

	target = position + forward;
	up = finalUp;

	isDirty = true;
}

void FPCamera::Refresh()
{
	if (!isDirty) return;

	glm::vec3 view = glm::normalize(target - position);
	glm::vec3 right = glm::normalize(glm::cross(up, view));
	// Recalculate the up vector
	this->up = glm::normalize(glm::cross(view, right));
	this->isDirty = false;

}