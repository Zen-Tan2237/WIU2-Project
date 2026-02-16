#include "CollisionDetection.h"

bool checkCollision(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData)
{
	// Check the types of the bounding boxes of the two objects and call the appropriate collision detection function
	if (objA.getBoundingBox().getType() == BoundingBox::Type::AABB && objB.getBoundingBox().getType() == BoundingBox::Type::AABB) {
		return OverlapAABBAABB(objA, objB, collisionData);
	}
	else if (objA.getBoundingBox().getType() == BoundingBox::Type::OBB && objB.getBoundingBox().getType() == BoundingBox::Type::OBB) {
		return OverlapOBBOBB(objA, objB, collisionData);
	}
	else if (objA.getBoundingBox().getType() == BoundingBox::Type::OBB && objB.getBoundingBox().getType() == BoundingBox::Type::AABB) {
		return OverlapOBBAABB(objA, objB, collisionData);
	}
	else if (objA.getBoundingBox().getType() == BoundingBox::Type::SPHERE && objB.getBoundingBox().getType() == BoundingBox::Type::SPHERE) {
		return OverlapSphereSphere(objA, objB, collisionData);
	}
	else if (objA.getBoundingBox().getType() == BoundingBox::Type::AABB && objB.getBoundingBox().getType() == BoundingBox::Type::SPHERE) {
		return OverlapAABBSphere(objA, objB, collisionData);
	}
	else if (objA.getBoundingBox().getType() == BoundingBox::Type::OBB && objB.getBoundingBox().getType() == BoundingBox::Type::SPHERE) {
		return OverlapOBBSphere(objA, objB, collisionData);
	}
	return false;
}