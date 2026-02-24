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

void PhysicsObject::UpdateInertiaTensors() {
	// Update world inverse inertia tensor based on current orientation
	glm::mat3 rotationMatrix = glm::mat3_cast(orientation);
	invInertiaWorld = rotationMatrix * invInertiaLocal * glm::transpose(rotationMatrix);
}

void PhysicsObject::UpdatePhysics(double dt) {
	float fdt = static_cast<float>(dt);

	// No time has passed, no need to update
	if (fdt <= 0.f) return;

	// update inertia tensors based on current orientation
	UpdateInertiaTensors();

	// Warp orientation
	orientation = glm::normalize(orientation);

	// Important params
	const float Drag = 0.5f; // Drag coefficient (tunable)
	const float AngularDrag = 0.5f; // Angular drag coefficient (tunable)
	const float SpeedThreshold = 0.01f; // Threshold below which we consider the object to be at rest
	const float AngularSpeedThreshold = 0.01f; // Threshold below which we consider the object to be at rest in terms of rotation

	// If gravity is enabled, apply it as a force
	if (GravityEnabled && mass > 0) {
		glm::vec3 gravityForce = glm::vec3(0.f, -9.81f, 0.f);
		totalForces += gravityForce * mass;
	}

	// If drag is enabled, apply it as a force opposite to the velocity
	if (DragEnabled && mass > 0) {
		glm::vec3 dragForce = -Drag * velocity;
		totalForces += dragForce;
		acceleration += dragForce * (1.f / mass);
	}

	// Apply angular drag
	glm::vec3 angularDragTorque = -AngularDrag * angularVelocity;
	totalTorque += angularDragTorque;

	// Apply acceleration from forces
	if (mass > 0) {
		acceleration = totalForces * (1.f / mass);
	}

	// Apply angular acceleration from torque
	glm::vec3 angularAcceleration = invInertiaWorld * totalTorque;
	angularVelocity += angularAcceleration * fdt;


	// apply angular velocity to orientation
	orientation += 0.5f * glm::quat(0.f, angularVelocity) * orientation * fdt;

	// Semi-implicit Euler integration for linear motion
	velocity += acceleration * fdt;
	position += velocity * fdt;


	// Snap to 0 velocity if below threshold to prevent jitter
	if (glm::length(velocity) < SpeedThreshold) {
		velocity = glm::vec3(0.f);
	}

	// Warp orientation again to prevent implicit scaling from numerical errors
	orientation = glm::normalize(orientation);

	// Reset forces and torque for the next frame
	ForcesSetZero();    
}

void PhysicsObject::ForcesSetZero() {
	totalForces = glm::vec3(0.f);
	totalTorque = glm::vec3(0.f);
}

void PhysicsObject::InitPhysicsObject(glm::vec3 pos, float mass, BoundingBox::Type bbType, glm::vec3 bbDimensions, bool* miscSettings) {
	position = pos;
	this->mass = mass;
	// misc settings is always size 2, where [0] is gravity enabled and [1] is drag enabled.
	miscSettings[0] ? GravityEnabled = true : GravityEnabled = false;
	miscSettings[1] ? DragEnabled = true : DragEnabled = false;

	if (bbType == BoundingBox::Type::OBB) {
		boundingBox.setType(bbType);
		boundingBox.setWidth(bbDimensions.x);
		boundingBox.setHeight(bbDimensions.y);
		boundingBox.setDepth(bbDimensions.z);
		boundingBox.InitBB();
		// Calculate local inverse inertia tensor for OBB
		float w = bbDimensions.x;
		float h = bbDimensions.y;
		float d = bbDimensions.z;
		float ix = (1.f / 12.f) * mass * (h * h + d * d);
		float iy = (1.f / 12.f) * mass * (w * w + d * d);
		float iz = (1.f / 12.f) * mass * (w * w + h * h);

		// Avoid division by zero for static objects
		if (ix > 0.f) invInertiaLocal[0][0] = 1.f / ix;
		else invInertiaLocal[0][0] = 0.f;
		if (iy > 0.f) invInertiaLocal[1][1] = 1.f / iy;
		else invInertiaLocal[1][1] = 0.f;
		if (iz > 0.f) invInertiaLocal[2][2] = 1.f / iz;
		else invInertiaLocal[2][2] = 0.f;
	}
	else if (bbType == BoundingBox::Type::SPHERE) {
		boundingBox.setType(bbType);
		boundingBox.setRadius(bbDimensions.x); // Use x component for radius

		// Calculate local inverse inertia tensor for sphere
		float r = bbDimensions.x;
		float i = (2.f / 5.f) * mass * r * r;
		if (i > 0.f) invInertiaLocal[0][0] = invInertiaLocal[1][1] = invInertiaLocal[2][2] = 1.f / i;
		else invInertiaLocal[0][0] = invInertiaLocal[1][1] = invInertiaLocal[2][2] = 0.f;
	}
}


void PhysicsObject::InitPhysicsObject(glm::vec3 pos, float mass, BoundingBox::Type bbType, glm::vec3 bbDimensions, glm::quat orientation, bool* miscSettings) {
	position = pos;
	this->mass = mass;
	this->orientation = orientation;
	miscSettings[0] ? GravityEnabled = true : GravityEnabled = false;
	miscSettings[1] ? DragEnabled = true : DragEnabled = false;

	if (bbType == BoundingBox::Type::OBB) {
		boundingBox.setType(bbType);
		boundingBox.setWidth(bbDimensions.x);
		boundingBox.setHeight(bbDimensions.y);
		boundingBox.setDepth(bbDimensions.z);
		boundingBox.InitBB();
		// Calculate local inverse inertia tensor for OBB
		float w = bbDimensions.x;
		float h = bbDimensions.y;
		float d = bbDimensions.z;
		float ix = (1.f / 12.f) * mass * (h * h + d * d);
		float iy = (1.f / 12.f) * mass * (w * w + d * d);
		float iz = (1.f / 12.f) * mass * (w * w + h * h);

		// Avoid division by zero for static objects
		if (ix > 0.f) invInertiaLocal[0][0] = 1.f / ix;
		else invInertiaLocal[0][0] = 0.f;
		if (iy > 0.f) invInertiaLocal[1][1] = 1.f / iy;
		else invInertiaLocal[1][1] = 0.f;
		if (iz > 0.f) invInertiaLocal[2][2] = 1.f / iz;
		else invInertiaLocal[2][2] = 0.f;
	}
	else if (bbType == BoundingBox::Type::SPHERE) {
		boundingBox.setType(bbType);
		boundingBox.setRadius(bbDimensions.x); // Use x component for radius

		// Calculate local inverse inertia tensor for sphere
		float r = bbDimensions.x;
		float i = (2.f / 5.f) * mass * r * r;
		if (i > 0.f) invInertiaLocal[0][0] = invInertiaLocal[1][1] = invInertiaLocal[2][2] = 1.f / i;
		else invInertiaLocal[0][0] = invInertiaLocal[1][1] = invInertiaLocal[2][2] = 0.f;
	}
}

void PhysicsObject::InitPhysicsObject(glm::vec3 pos, float mass, BoundingBox::Type bbType, glm::vec3 bbDimensions, float angleDeg, glm::vec3 rotAxis, bool* miscSettings) {
	position = pos;
	this->mass = mass;
	// Calculate orientation from angle and rotation axis
	glm::quat newOrientation = glm::angleAxis(glm::radians(angleDeg), glm::normalize(rotAxis));
	orientation = newOrientation;
	miscSettings[0] ? GravityEnabled = true : GravityEnabled = false;
	miscSettings[1] ? DragEnabled = true : DragEnabled = false;

	if (bbType == BoundingBox::Type::OBB) {
		boundingBox.setType(bbType);
		boundingBox.setWidth(bbDimensions.x);
		boundingBox.setHeight(bbDimensions.y);
		boundingBox.setDepth(bbDimensions.z);
		boundingBox.InitBB();
		// Calculate local inverse inertia tensor for OBB
		float w = bbDimensions.x;
		float h = bbDimensions.y;
		float d = bbDimensions.z;
		float ix = (1.f / 12.f) * mass * (h * h + d * d);
		float iy = (1.f / 12.f) * mass * (w * w + d * d);
		float iz = (1.f / 12.f) * mass * (w * w + h * h);

		// Avoid division by zero for static objects
		if (ix > 0.f) invInertiaLocal[0][0] = 1.f / ix;
		else invInertiaLocal[0][0] = 0.f;
		if (iy > 0.f) invInertiaLocal[1][1] = 1.f / iy;
		else invInertiaLocal[1][1] = 0.f;
		if (iz > 0.f) invInertiaLocal[2][2] = 1.f / iz;
		else invInertiaLocal[2][2] = 0.f;
	}
	else if (bbType == BoundingBox::Type::SPHERE) {
		boundingBox.setType(bbType);
		boundingBox.setRadius(bbDimensions.x); // Use x component for radius

		// Calculate local inverse inertia tensor for sphere
		float r = bbDimensions.x;
		float i = (2.f / 5.f) * mass * r * r;
		if (i > 0.f) invInertiaLocal[0][0] = invInertiaLocal[1][1] = invInertiaLocal[2][2] = 1.f / i;
		else invInertiaLocal[0][0] = invInertiaLocal[1][1] = invInertiaLocal[2][2] = 0.f;
	}
}

void PhysicsObject::ResetPhysicsProperties() {
	position = glm::vec3(0.f);
	velocity = glm::vec3(0.f);
	acceleration = glm::vec3(0.f);
	orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
	angularVelocity = glm::vec3(0.f);
	ForcesSetZero();
}

// Rotate the orientation of the object by a given angle (in degrees) around a specified axis
// rotationAxis should be a non-zero vector representing the axis of rotation (e.g., glm::vec3(0, 1, 0) for rotation around the y-axis)
void PhysicsObject::RotateOrientation(float angleDegrees, glm::vec3 rotationAxis) {
	glm::quat rotationQuat = glm::angleAxis(glm::radians(angleDegrees), glm::normalize(rotationAxis));
	orientation = rotationQuat * orientation;
	// Warp orientation to prevent implicit scaling from numerical errors
	orientation = glm::normalize(orientation);
}