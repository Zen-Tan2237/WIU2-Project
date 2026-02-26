#include "PhysicsObject.h"

void PhysicsObject::AddForce(const glm::vec3& force)
{
	totalForces += force;
}

void PhysicsObject::AddBoundingBox(BoundingBox box) {
	boundingBox = box;
}

bool PhysicsObject::isOBBanAABB() const {
	if (boundingBox.getType() == BoundingBox::Type::SPHERE) return false;

	// If orientation is "close enough" to identity (or -identity), treat as AABB
	const glm::quat id(1.f, 0.f, 0.f, 0.f);
	float d = std::abs(glm::dot(glm::normalize(orientation), id)); // abs handles -identity too
	return d > 0.9999f; // tweak if needed
}

void PhysicsObject::AddImpulse(const glm::vec3& impulse)
{
	if (mass <= 0.f) return; // Static objects don't respond to impulses
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
	if (fdt <= 0.f) return;

	// Normalise orientation
	orientation = glm::normalize(orientation);

	// Reset accumulators you actually integrate
	acceleration = glm::vec3(0.f);

	// Gravity
	if (GravityEnabled && mass > 0.f) {
		totalForces += glm::vec3(0.f, -9.81f, 0.f) * mass;
	}

	// Linear drag
	const float Drag = 0.5f;
	if (DragEnabled && mass > 0.f) {
		totalForces += -Drag * velocity;
	}

	// Angular drag
	const float AngularDrag = 0.5f;
	totalTorque += -AngularDrag * angularVelocity;

	// Integrate angular
	UpdateInertiaTensors();
	glm::vec3 angularAcceleration = invInertiaWorld * totalTorque;
	angularVelocity += angularAcceleration * fdt;

	glm::quat omega(0.f, angularVelocity.x, angularVelocity.y, angularVelocity.z);
	orientation += 0.5f * omega * orientation * fdt;
	orientation = glm::normalize(orientation);

	// Integrate linear (semi-implicit euler)
	if (mass > 0.f) acceleration = totalForces / mass;
	velocity += acceleration * fdt;
	position += velocity * fdt;

	// Sleep thresholds (consider lowering)
	const float SpeedThreshold = 0.01f;
	const float AngularSpeedThreshold = 0.01f;
	if (glm::length(velocity) < SpeedThreshold) velocity = glm::vec3(0.f);
	if (glm::length(angularVelocity) < AngularSpeedThreshold) angularVelocity = glm::vec3(0.f);

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

	// set orientation to identity by default, can be overridden by other overloads of InitPhysicsObject
	orientation = glm::quat(1.f, 0.f, 0.f, 0.f);

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

void PhysicsObject::RotateOrientation(float xDegrees, float yDegrees, float zDegrees) {
	glm::quat rotX = glm::angleAxis(glm::radians(xDegrees), glm::vec3(1.f, 0.f, 0.f));
	glm::quat rotY = glm::angleAxis(glm::radians(yDegrees), glm::vec3(0.f, 1.f, 0.f));
	glm::quat rotZ = glm::angleAxis(glm::radians(zDegrees), glm::vec3(0.f, 0.f, 1.f));
	glm::quat rotationQuat = rotZ * rotY * rotX; // Note: order of multiplication matters
	orientation = rotationQuat * orientation;
	// Warp orientation to prevent implicit scaling from numerical errors
	orientation = glm::normalize(orientation);
}

// Set orientation to a specific angle (in degrees) around a specified axis, replacing the current orientation
void PhysicsObject::SetOrientation(float angleDegrees, glm::vec3 rotationAxis) {
	orientation = glm::angleAxis(glm::radians(angleDegrees), glm::normalize(rotationAxis));
	// Warp orientation to prevent implicit scaling from numerical errors
	orientation = glm::normalize(orientation);
}

void PhysicsObject::SetOrientation(float xDegrees, float yDegrees, float zDegrees) {
	glm::quat rotX = glm::angleAxis(glm::radians(xDegrees), glm::vec3(1.f, 0.f, 0.f));
	glm::quat rotY = glm::angleAxis(glm::radians(yDegrees), glm::vec3(0.f, 1.f, 0.f));
	glm::quat rotZ = glm::angleAxis(glm::radians(zDegrees), glm::vec3(0.f, 0.f, 1.f));
	orientation = rotZ * rotY * rotX; // Note: order of multiplication matters
	// Warp orientation to prevent implicit scaling from numerical errors
	orientation = glm::normalize(orientation);
}
