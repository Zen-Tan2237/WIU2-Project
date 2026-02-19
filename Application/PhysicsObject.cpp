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
	// keep quaternions normalized to prevent floating point drift
	orientation = glm::normalize(orientation);

	// convert dt to float for physics calculations
	float fdt = static_cast<float>(dt);

	// Linear physics update
	if (GravityEnabled) {
		totalForces += glm::vec3(0.f, -9.81f * mass, 0.f); // Apply gravity force
	}
	if (DragEnabled) {
		totalForces += -velocity * 0.1f; // Simple drag force proportional to velocity
	}
	if (mass > 0.f) {
		// Simple Euler integration for linear motion
		glm::vec3 acceleration = totalForces / mass;
		velocity += acceleration * static_cast<float>(dt);
		position += velocity * static_cast<float>(dt);
	}
	
	// Angular physics update
	// Update inv inertia in world space from current orientation
	glm::mat3 R = glm::mat3_cast(orientation);
	invInertiaWorld = R * invInertiaLocal * glm::transpose(R);

	glm::vec3 angularAcc = invInertiaWorld * totalTorque;
	angularVelocity += angularAcc * fdt;

	// Integrate orientation using angular velocity
	glm::quat wq(0.f, angularVelocity.x, angularVelocity.y, angularVelocity.z);
	glm::quat dq = 0.5f * wq * orientation;
	orientation += dq * fdt;
	orientation = glm::normalize(orientation);

	// Clear accumulators
	ForcesSetZero();
}

void PhysicsObject::ForcesSetZero() {
	totalForces = glm::vec3(0.f);
	totalTorque = glm::vec3(0.f);
}