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
	static const float ZOOM_SPEED = 5.0f;
	static const float MOUSE_SENS = 0.1f;

	glm::vec3 view = glm::normalize(target - position);
	glm::vec3 right = glm::normalize(glm::cross(view, up));

	glm::vec3 moveDir = glm::normalize(glm::vec3(view.x, 0, view.z));

	glm::vec3 input(0, 0, 0);

	// Move Forward/Backward
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W)) {
		//position += moveDir * ZOOM_SPEED * static_cast<float>(dt);
		//target += moveDir * ZOOM_SPEED * static_cast<float>(dt);
		input += moveDir;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S)) {
		//position -= moveDir * ZOOM_SPEED * static_cast<float>(dt);
		//target -= moveDir * ZOOM_SPEED * static_cast<float>(dt);
		input -= moveDir;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {
		//position -= right * ZOOM_SPEED * static_cast<float>(dt);
		//target -= right * ZOOM_SPEED * static_cast<float>(dt);
		input -= right;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D)) {
		//position += right * ZOOM_SPEED * static_cast<float>(dt);
		//target += right * ZOOM_SPEED * static_cast<float>(dt);
		input += right;
	}

	if (glm::length(input) > 0.001f) {
		position += glm::normalize(input) * ZOOM_SPEED * static_cast<float>(dt);
		target += glm::normalize(input) * ZOOM_SPEED * static_cast<float>(dt);
	}

	double deltaX = MouseController::GetInstance()->GetMouseDeltaX();
	double deltaY = MouseController::GetInstance()->GetMouseDeltaY();

	float angle = -deltaX * ROTATE_SPEED * MOUSE_SENS * static_cast<float>(dt);
	float angleY = deltaY * ROTATE_SPEED * MOUSE_SENS * static_cast<float>(dt);

	glm::mat4 yaw = glm::rotate(
		glm::mat4(1.f),// matrix to modify
		glm::radians(angle),// rotation angle in degree and converted to radians
		glm::vec3(up.x, up.y, up.z)// the axis to rotate along
	);

	glm::mat4 pitch = glm::rotate(glm::mat4(1.f), glm::radians(angleY), right);

	// calculate the rotated view vector
	view = pitch * glm::vec4(view, 0.f);
	glm::vec3 yawView = yaw * glm::vec4(view, 0.f);
	target = position + yawView;
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