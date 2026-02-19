#include "CollisionDetection.h"

bool CheckCollision(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData)
{
	// Check the types of the bounding boxes of the two objects and call the appropriate collision detection function
	if ((objA.boundingBox.getType() == BoundingBox::Type::OBB && objA.isOBBanAABB()) && (objB.boundingBox.getType() == BoundingBox::Type::OBB && objB.isOBBanAABB())) {
		return OverlapAABBAABB(objA, objB, collisionData);
	}
	else if (objA.boundingBox.getType() == BoundingBox::Type::OBB && objB.boundingBox.getType() == BoundingBox::Type::OBB) {
		return OverlapOBBOBB(objA, objB, collisionData);
	}
	else if (objA.boundingBox.getType() == BoundingBox::Type::OBB && (objB.boundingBox.getType() == BoundingBox::Type::OBB && objB.isOBBanAABB())) {
		return OverlapOBBAABB(objA, objB, collisionData);
	}
	else if (objA.boundingBox.getType() == BoundingBox::Type::SPHERE && objB.boundingBox.getType() == BoundingBox::Type::SPHERE) {
		return OverlapSphereSphere(objA, objB, collisionData);
	}
	else if ((objA.boundingBox.getType() == BoundingBox::Type::OBB && objA.isOBBanAABB()) && objB.boundingBox.getType() == BoundingBox::Type::SPHERE) {
		return OverlapAABBSphere(objA, objB, collisionData);
	}
	else if (objA.boundingBox.getType() == BoundingBox::Type::OBB && objB.boundingBox.getType() == BoundingBox::Type::SPHERE) {
		return OverlapOBBSphere(objA, objB, collisionData);
	}
	return false;
}

bool OverlapAABBAABB(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData)
{
	// Implement AABB vs AABB collision detection
	return false;
}

bool OverlapOBBOBB(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData)
{
	// Implement OBB vs OBB collision detection (To be done IF ABSOLUTELY REQUIRED. idw do quarternions and rotation matrices if not necessary)
	return false;
}

bool OverlapOBBAABB(const PhysicsObject& OBB, const PhysicsObject& AABB, CollisionData& collisionData)
{
	// Implement OBB vs AABB collision detection
	return false;
}

bool OverlapSphereSphere(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData)
{
	// Implement Sphere vs Sphere collision detection 
	float totalRadius = objA.boundingBox.getRadius() + objB.boundingBox.getRadius();
	float dispX;

	return false;
}

bool OverlapAABBSphere(const PhysicsObject& AABB, const PhysicsObject& Sphere, CollisionData& collisionData)
{
	// Implement AABB vs Sphere collision detection
	return false;
}

bool OverlapOBBSphere(const PhysicsObject& OBB, const PhysicsObject& Sphere, CollisionData& collisionData)
{
	// Implement OBB vs Sphere collision detection
	return false;
}

void ResolveCollision(CollisionData& collisionData)
{
	// Implement collision resolution using the collision data
}