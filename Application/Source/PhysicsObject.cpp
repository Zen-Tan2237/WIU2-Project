#include "PhysicsObject.h"

void PhysicsObject::AddForce(const glm::vec3& force)
{
	totalForces += force;
}

void PhysicsObject::AddBoundingBox(BoundingBox box) {
	boundingBox = box;
}

bool PhysicsObject::isOBBanAABB() const {
	if (boundingBox.getType() == BoundingBox::Type::SPHERE) {
		return false;
	}
	else {
		glm::quat identityQuat(1.f, 0.f, 0.f, 0.f);
		glm::quat negIdentityQuat(-1.f, 0.f, 0.f, 0.f);
		if (orientation == identityQuat || orientation == negIdentityQuat) {
			return true;
		}
	}
	return false;
}

void PhysicsObject::AddImpulse(const glm::vec3& impulse)
{
	velocity += impulse / mass;
}

void PhysicsObject::AddImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& point)
{
	// Linear impulse
	AddImpulse(impulse);
	// Angular impulse
	glm::vec3 r = point - position; // Vector from center of mass to point of application
	glm::vec3 angularImpulse = glm::cross(r, impulse);
	angularVelocity += invInertiaWorld * angularImpulse;
}

void PhysicsObject::UpdatePhysics(double dt) {
	float fdt = static_cast<float>(dt);

	// No time has passed, no need to update
	if (fdt <= 0.f) return;

	// Warp orientation
	orientation = glm::normalize(orientation);

	// If gravity is enabled, apply it as a force
	if (GravityEnabled) {
		glm::vec3 gravityForce = glm::vec3(0.f, -9.81f, 0.f);
		totalForces += gravityForce * mass;
	}

	// Important params
	const float Drag = 1.5f; // Drag coefficient (tunable)
	const float SpeedThreshold = 0.01f; // Threshold below which we consider the object to be at rest

	// If drag is enabled, apply it as a force opposite to velocity
	if (mass > 0) {
		acceleration = totalForces * (1.f / mass);
	}

	// If drag is enabled, apply it as a force opposite to velocity
	if (DragEnabled) {
		glm::vec3 dragForce = -Drag * velocity;
		totalForces += dragForce;
		acceleration += dragForce * (1.f / mass);
	}

	// apply angular velocity to orientation
	orientation += 0.5f * glm::quat(0.f, angularVelocity) * orientation * fdt;

	// Snap to 0 velocity if below threshold to prevent jitter
	if (glm::length(velocity) < SpeedThreshold) {
		velocity = glm::vec3(0.f);
	}

	// Semi-implicit Euler integration for linear motion
	velocity += acceleration * fdt;
	position += velocity * fdt;

	// Reset forces and torque for the next frame
	ForcesSetZero();    
}

void PhysicsObject::ForcesSetZero() {
	totalForces = glm::vec3(0.f);
	totalTorque = glm::vec3(0.f);
}