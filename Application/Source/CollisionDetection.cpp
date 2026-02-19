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
	// Get extents of AABB A
	glm::vec3 minA = objA.boundingBox.getMinPoint();
	glm::vec3 maxA = objA.boundingBox.getMaxPoint();

	// Get extents of AABB B
	glm::vec3 minB = objB.boundingBox.getMinPoint();
	glm::vec3 maxB = objB.boundingBox.getMaxPoint();

	// Check for overlap on each axis
	bool overlapX = (minA.x <= maxB.x) && (maxA.x >= minB.x);
	bool overlapY = (minA.y <= maxB.y) && (maxA.y >= minB.y);
	bool overlapZ = (minA.z <= maxB.z) && (maxA.z >= minB.z);

	bool isColliding = overlapX && overlapY && overlapZ;

	if (isColliding) {
		collisionData.collisionNormal = glm::vec3(0.f, 1.f, 0.f); 
		collisionData.contactPoint = (glm::max(minA, minB) + glm::min(maxA, maxB)) * 0.5f; 
		collisionData.penetration = glm::length(glm::min(maxA, maxB) - glm::max(minA, minB));
		collisionData.pObjA = const_cast<PhysicsObject*>(&objA);
		collisionData.pObjB = const_cast<PhysicsObject*>(&objB);
		// Currently Unused
		collisionData.tangent = glm::normalize(glm::cross(collisionData.collisionNormal, glm::vec3(0.f, 1.f, 0.f)));
		collisionData.bitangent = glm::normalize(glm::cross(collisionData.collisionNormal, collisionData.tangent));
	}
	return isColliding;
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
	return false;
}

bool OverlapAABBSphere(const PhysicsObject& AABB, const PhysicsObject& Sphere, CollisionData& collisionData)
{
	// Implement AABB vs Sphere collision detection
	float distanceSquared = 0.f;
	glm::vec3 sphereCenter = Sphere.boundingBox.getSphereCenter();
	glm::vec3 aabbMin = AABB.boundingBox.getMinPoint();
	glm::vec3 aabbMax = AABB.boundingBox.getMaxPoint();

	// Calculate the squared distance from the sphere center to the AABB
	for (int i = 0; i < 3; ++i) {
		if (sphereCenter[i] < aabbMin[i]) {
			distanceSquared += (aabbMin[i] - sphereCenter[i]) * (aabbMin[i] - sphereCenter[i]);
		}
		else if (sphereCenter[i] > aabbMax[i]) {
			distanceSquared += (sphereCenter[i] - aabbMax[i]) * (sphereCenter[i] - aabbMax[i]);
		}
	}
	bool isColliding = distanceSquared <= Sphere.boundingBox.getRadius() * Sphere.boundingBox.getRadius();

	// Check if the squared distance is less than or equal to the squared radius of the sphere
	if (isColliding) {
		// Collision detected, fill in collisionData as needed
		collisionData.collisionNormal = glm::normalize(sphereCenter - glm::clamp(sphereCenter, aabbMin, aabbMax));
		collisionData.contactPoint = sphereCenter - collisionData.collisionNormal * Sphere.boundingBox.getRadius();
		collisionData.penetration = Sphere.boundingBox.getRadius() - sqrt(distanceSquared);
		collisionData.pObjA = const_cast<PhysicsObject*>(&AABB);
		collisionData.pObjB = const_cast<PhysicsObject*>(&Sphere);
		// Currently Unused
		collisionData.tangent = glm::normalize(glm::cross(collisionData.collisionNormal, glm::vec3(0.f, 1.f, 0.f)));
		collisionData.bitangent = glm::normalize(glm::cross(collisionData.collisionNormal, collisionData.tangent));
	}
	return isColliding;
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