#include "FPCamera.h"
#include "KeyboardController.h"

//Include GLFW
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL	// It'll cause error ifndef 
#include <glm/gtx/rotate_vector.hpp>

FPCamera::FPCamera() : isDirty(false)
{
	this->position = glm::vec3(0, 0, 0);
	this->target = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);
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

	Refresh();
}

void FPCamera::Reset()
{
}

void FPCamera::Update(double dt)
{
	static const float ROTATE_SPEED = 100.0f;
	static const float ZOOM_SPEED = 10.0f;

	glm::vec3 view = glm::normalize(target - position);
	glm::vec3 right = glm::normalize(glm::cross(view, up));

	// Rotate Left/Right
	float yawDelta = 0.0f;
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT)) {
		yawDelta = glm::radians(ROTATE_SPEED * static_cast<float>(dt));
	} 
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_RIGHT)) {
		yawDelta = -glm::radians(ROTATE_SPEED * static_cast<float>(dt));
	}

	// Move Forward/Backward
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W)) {
		position += view * ZOOM_SPEED * static_cast<float>(dt);
		target += view * ZOOM_SPEED * static_cast<float>(dt);
		isDirty = true;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S)) {
		position -= view * ZOOM_SPEED * static_cast<float>(dt);
		target -= view * ZOOM_SPEED * static_cast<float>(dt);
		isDirty = true;
	}

	// Rotate up/down (Disabled)
	//if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_UP)) {
	//	// Rotate the view vector around the right axis (pitch)
	//	view = glm::rotate(view, glm::radians(ROTATE_SPEED * static_cast<float>(dt)), right);
	//	target = position + view;
	//	isDirty = true;
	//}
	//else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_DOWN)) {
	//	// Rotate the view vector around the right axis (pitch)
	//	view = glm::rotate(view, -glm::radians(ROTATE_SPEED * static_cast<float>(dt)), right);
	//	target = position + view;
	//	isDirty = true;
	//}

	if (yawDelta != 0.0f) {
		// Rotate the view vector around the world up axis (yaw)
		view = glm::rotate(view, yawDelta, up);
		target = position + view;
		isDirty = true;
	}

	this->Refresh();
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