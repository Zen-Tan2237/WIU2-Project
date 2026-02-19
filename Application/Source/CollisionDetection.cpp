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
		collisionData.contactPointB = collisionData.contactPoint; // For AABB vs AABB, the contact point on both objects is the same
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
	// Implement OBB vs OBB collision detection (To be done IF ABSOLUTELY REQUIRED.)
	return false;
}

bool OverlapOBBAABB(const PhysicsObject& OBB, const PhysicsObject& AABB, CollisionData& collisionData)
{
	// Implement OBB vs AABB collision detection (To be done IF ABSOLUTELY REQUIRED.)
	return false;
}

bool OverlapSphereSphere(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData)
{
	// Implement Sphere vs Sphere collision detection 
	float totalRadiusSquared = (objA.boundingBox.getRadius() + objB.boundingBox.getRadius()) * (objA.boundingBox.getRadius() + objB.boundingBox.getRadius());
	float dispX = objB.position.x - objA.position.x;
	float dispY = objB.position.y - objA.position.y;
	float dispZ = objB.position.z - objA.position.z;
	float dispSquared = (dispX * dispX) + (dispY * dispY) + (dispZ * dispZ);

	bool isColliding = (totalRadiusSquared > dispSquared);

	if (isColliding) {
		collisionData.collisionNormal = glm::normalize(objB.position - objA.position);
		collisionData.contactPoint = objA.position + collisionData.collisionNormal * objA.boundingBox.getRadius();
		collisionData.contactPointB = objB.position - collisionData.collisionNormal * objB.boundingBox.getRadius();
		collisionData.penetration = (objA.boundingBox.getRadius() + objB.boundingBox.getRadius()) - sqrt(dispSquared);
		collisionData.pObjA = const_cast<PhysicsObject*>(&objA);
		collisionData.pObjB = const_cast<PhysicsObject*>(&objB);
		// Currently Unused
		collisionData.tangent = glm::normalize(glm::cross(collisionData.collisionNormal, glm::vec3(0.f, 1.f, 0.f)));
		collisionData.bitangent = glm::normalize(glm::cross(collisionData.collisionNormal, collisionData.tangent));
	}
}

bool OverlapAABBSphere(const PhysicsObject& AABB, const PhysicsObject& Sphere, CollisionData& collisionData)
{
	// Implement AABB vs Sphere collision detection
	float distanceSquared = 0.f;
	glm::vec3 sphereCenter = Sphere.position;
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
	// 1. Get all axes 


	return false;
}

void ResolveCollision(CollisionData& cd)
{
	// Impulse based collision resolution with restitution and positional correction
	PhysicsObject* objA = cd.pObjA;
	PhysicsObject* objB = cd.pObjB;
	glm::vec3 relativeVelocity = objB->velocity - objA->velocity;
	glm::vec3 velAlongNormal = glm::dot(relativeVelocity, cd.collisionNormal) * cd.collisionNormal;
	if (glm::dot(velAlongNormal, cd.collisionNormal) > 0) {
		return;
	}
	// Calculate restitution (bounciness)
	float e = std::min(objA->bounciness, objB->bounciness);
	float j = -(1 + e) * glm::dot(relativeVelocity, cd.collisionNormal);
	
	// prepare inverse mass for impulse calculation
	float invMassA = (objA->mass > 0.f) ? 1.f / objA->mass : 0.f;
	float invMassB = (objB->mass > 0.f) ? 1.f / objB->mass : 0.f;
	float invMassSum = invMassA + invMassB;

	// Impluse calculation
	glm::vec3 impulse = (j / invMassSum) * cd.collisionNormal;
	objA->AddImpulse(-impulse);
	objB->AddImpulse(impulse);

	// Positional Correction
	const float percent = 0.2f; // Penetration percentage to correct
	const float slop = 0.01f; // Penetration allowance
	glm::vec3 correction = std::max(cd.penetration - slop, 0.0f) / invMassSum * percent * cd.collisionNormal;
	objA->position -= invMassA * correction;
	objB->position += invMassB * correction;

	// Angular impulse resolution

	// Apply Friction here (when needed)

	// End.
}