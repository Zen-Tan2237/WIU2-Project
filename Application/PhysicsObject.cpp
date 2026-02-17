#include "PhysicsObject.h"

void PhysicsObject::AddForce(const glm::vec3& force)
{
	totalForces += force;
}

void PhysicsObject::AddBoundingBox(BoundingBox box) {
	boundingBox = box;
}

void PhysicsObject::AddTorque(const glm::vec3& torque)
{
}