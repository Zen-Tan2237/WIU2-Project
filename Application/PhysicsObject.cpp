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

bool PhysicsObject::isOBBanAABB() const {
	if (boundingBox.getType() == BoundingBox::Type::SPHERE) {
		return false;
	}
	else {
		if (angleDeg == 0.f || angleDeg == 180.f) {
			return true;
		}
	}
	return false;
}