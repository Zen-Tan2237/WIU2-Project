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
	return isColliding;
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
	// 1. Get sphere stuff
	glm::vec3 sphereCenter = Sphere.position;
	glm::vec3 obbCenter = OBB.position;
	glm::vec3 toSphere = sphereCenter - obbCenter;
	
	// 2. Get OBB half extents and orientation
	glm::vec3 halfExtents = OBB.boundingBox.getHalfExtents();
	glm::quat orientation = OBB.orientation;

	// 3. Get OBB axes from orientation
	glm::mat3 rotationMatrix = glm::mat3_cast(orientation);
	glm::vec3 obbAxes[3] = { rotationMatrix[0], rotationMatrix[1], rotationMatrix[2] }; // 0 = x, 1 = y, 2 = z
	
	// 4. Project toSphere onto each OBB axis and clamp to half extents
	glm::vec3 closestPointOnOBB = obbCenter;
	closestPointOnOBB += obbAxes[0] * glm::clamp(glm::dot(toSphere, obbAxes[0]), -halfExtents.x, halfExtents.x);
	closestPointOnOBB += obbAxes[1] * glm::clamp(glm::dot(toSphere, obbAxes[1]), -halfExtents.y, halfExtents.y);
	closestPointOnOBB += obbAxes[2] * glm::clamp(glm::dot(toSphere, obbAxes[2]), -halfExtents.z, halfExtents.z);

	// 5. Calculate distance from closest point on OBB to sphere center
	glm::vec3 closestToSphere = sphereCenter - closestPointOnOBB;
	float distanceSquared = glm::dot(closestToSphere, closestToSphere);
	float radiusSquared = Sphere.boundingBox.getRadius() * Sphere.boundingBox.getRadius();
	bool isColliding = distanceSquared <= radiusSquared;

	if (isColliding) {
		collisionData.collisionNormal = glm::normalize(closestToSphere);
		collisionData.contactPoint = closestPointOnOBB;
		collisionData.contactPointB = sphereCenter - collisionData.collisionNormal * Sphere.boundingBox.getRadius();
		collisionData.penetration = Sphere.boundingBox.getRadius() - sqrt(distanceSquared);
		collisionData.pObjA = const_cast<PhysicsObject*>(&OBB);
		collisionData.pObjB = const_cast<PhysicsObject*>(&Sphere);
		// Currently Unused
		collisionData.tangent = glm::normalize(glm::cross(collisionData.collisionNormal, glm::vec3(0.f, 1.f, 0.f)));
		collisionData.bitangent = glm::normalize(glm::cross(collisionData.collisionNormal, collisionData.tangent));
	}



	return false;
}

void ResolveCollision(CollisionData& cd)
{
	// Impulse based collision resolution with restitution and positional correction
	PhysicsObject* objA = cd.pObjA;
	PhysicsObject* objB = cd.pObjB;

	// Guard against null pointers
	if (!objA || !objB) {
		return;
	}

	// Calculate relative position vectors from object centers to contact points
	glm::vec3 rA = cd.contactPoint - objA->position;
	glm::vec3 rB = cd.contactPointB - objB->position;

	// Calculate velocity at contact points (linear + rotational components)
	glm::vec3 velA_contact = objA->velocity + glm::cross(objA->angularVelocity, rA);
	glm::vec3 velB_contact = objB->velocity + glm::cross(objB->angularVelocity, rB);
	glm::vec3 relativeVelocity = velB_contact - velA_contact;

	// Check if objects are approaching (moving towards each other along collision normal)
	float velocityAlongNormal = glm::dot(relativeVelocity, cd.collisionNormal);
	if (velocityAlongNormal >= 0.0f) {
		return; // Objects are separating or moving parallel
	}

	// Calculate restitution (bounciness)
	float e = std::min(objA->bounciness, objB->bounciness);
	float j = -(1.0f + e) * velocityAlongNormal;

	// Prepare inverse mass for impulse calculation
	float invMassA = (objA->mass > 0.0f) ? 1.0f / objA->mass : 0.0f;
	float invMassB = (objB->mass > 0.0f) ? 1.0f / objB->mass : 0.0f;

	// Calculate rotational effect on impulse denominator
	// This accounts for how the contact point offset affects rotational response
	glm::vec3 rACrossN = glm::cross(rA, cd.collisionNormal);
	glm::vec3 rBCrossN = glm::cross(rB, cd.collisionNormal);
	float rotEffectA = glm::dot(rACrossN, objA->invInertiaWorld * rACrossN);
	float rotEffectB = glm::dot(rBCrossN, objB->invInertiaWorld * rBCrossN);

	// Full inverse mass sum including rotational effects
	float invMassSum = invMassA + invMassB + rotEffectA + rotEffectB;

	// Guard against zero inverse mass sum (both objects are static)
	if (invMassSum < 1e-6f) {
		return;
	}

	// Impulse calculation (scalar magnitude)
	float impulseMagnitude = j / invMassSum;
	glm::vec3 impulse = impulseMagnitude * cd.collisionNormal;

	// Apply linear impulses
	objA->AddImpulse(-impulse);
	objB->AddImpulse(impulse);

	// Apply angular impulses (torque from contact point offset)
	glm::vec3 torqueA = glm::cross(rA, -impulse);
	glm::vec3 torqueB = glm::cross(rB, impulse);
	objA->angularVelocity += objA->invInertiaWorld * torqueA;
	objB->angularVelocity += objB->invInertiaWorld * torqueB;

	// Positional Correction (prevent penetration drift)
	const float percent = 0.2f; // Penetration percentage to correct (0-1 range)
	const float slop = 0.01f; // Penetration allowance before correcting
	float penetrationCorrection = std::max(cd.penetration - slop, 0.0f);
	glm::vec3 correction = (penetrationCorrection / invMassSum) * percent * cd.collisionNormal;
	objA->position -= invMassA * correction;
	objB->position += invMassB * correction;
}