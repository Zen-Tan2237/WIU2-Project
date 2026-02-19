#include "CollisionDetection.h"
#include <iostream>
#include <algorithm>
#include <cmath>

// Helper function to safely normalize vectors and provide fallback
glm::vec3 SafeNormalize(const glm::vec3& vec, const glm::vec3& fallback = glm::vec3(0.f, 1.f, 0.f))
{
	float length = glm::length(vec);
	if (length < 1e-6f) {
		return fallback;
	}
	return vec / length;
}

// Helper function to generate perpendicular vectors
void GenerateTangentBitangent(const glm::vec3& normal, glm::vec3& outTangent, glm::vec3& outBitangent)
{
	// Choose a vector that is not parallel to the normal
	glm::vec3 fallback = glm::abs(normal.y) < 0.9f ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(1.f, 0.f, 0.f);
	outTangent = SafeNormalize(glm::cross(normal, fallback), fallback);
	outBitangent = SafeNormalize(glm::cross(normal, outTangent), glm::cross(fallback, normal));
}

bool CheckCollision(PhysicsObject& objA, PhysicsObject& objB, CollisionData& collisionData)
{
	bool aIsAABB = objA.boundingBox.getType() == BoundingBox::Type::OBB && objA.isOBBanAABB();
	bool bIsAABB = objB.boundingBox.getType() == BoundingBox::Type::OBB && objB.isOBBanAABB();
	bool aIsOBB = objA.boundingBox.getType() == BoundingBox::Type::OBB && !objA.isOBBanAABB();
	bool bIsOBB = objB.boundingBox.getType() == BoundingBox::Type::OBB && !objB.isOBBanAABB();
	bool aIsSphere = objA.boundingBox.getType() == BoundingBox::Type::SPHERE;
	bool bIsSphere = objB.boundingBox.getType() == BoundingBox::Type::SPHERE;

	// AABB vs AABB
	if (aIsAABB && bIsAABB) {
		return OverlapAABBAABB(objA, objB, collisionData);
	}
	// OBB vs OBB (both are true OBBs, not axis-aligned)
	else if (aIsOBB && bIsOBB) {
		return OverlapOBBOBB(objA, objB, collisionData);
	}
	// OBB vs AABB (either order)
	else if ((aIsOBB && bIsAABB) || (aIsAABB && bIsOBB)) {
		return aIsOBB ? OverlapOBBAABB(objA, objB, collisionData)
			: OverlapOBBAABB(objB, objA, collisionData);
	}
	// Sphere vs Sphere
	else if (aIsSphere && bIsSphere) {
		return OverlapSphereSphere(objA, objB, collisionData);
	}
	// AABB vs Sphere (either order)
	else if ((aIsAABB && bIsSphere) || (aIsSphere && bIsAABB)) {
		if (aIsAABB) {
			return OverlapAABBSphere(objA, objB, collisionData);
		}
		else {
			// Sphere is A, AABB is B - swap and flip normal
			bool result = OverlapAABBSphere(objB, objA, collisionData);
			if (result) {
				// Swap the objects back
				std::swap(collisionData.pObjA, collisionData.pObjB);
				std::swap(collisionData.contactPoint, collisionData.contactPointB);
				// Flip the normal to point from A to B
				collisionData.collisionNormal = -collisionData.collisionNormal;
			}
			return result;
		}
	}
	// OBB vs Sphere (either order)
	else if ((aIsOBB && bIsSphere) || (aIsSphere && bIsOBB)) {
		if (aIsOBB) {
			return OverlapOBBSphere(objA, objB, collisionData);
		}
		else {
			// Sphere is A, OBB is B - swap and flip normal
			bool result = OverlapOBBSphere(objB, objA, collisionData);
			if (result) {
				// Swap the objects back
				std::swap(collisionData.pObjA, collisionData.pObjB);
				std::swap(collisionData.contactPoint, collisionData.contactPointB);
				// Flip the normal to point from A to B
				collisionData.collisionNormal = -collisionData.collisionNormal;
			}
			return result;
		}
	}
	return false;
}

bool OverlapAABBAABB(PhysicsObject& objA, PhysicsObject& objB, CollisionData& collisionData)
{
	// Get extents of AABB A and B
	glm::vec3 minA = objA.position - objA.boundingBox.getHalfExtents();
	glm::vec3 maxA = objA.position + objA.boundingBox.getHalfExtents();
	glm::vec3 minB = objB.position - objB.boundingBox.getHalfExtents();
	glm::vec3 maxB = objB.position + objB.boundingBox.getHalfExtents();

	// Check for overlap on each axis
	bool overlapX = (minA.x <= maxB.x) && (maxA.x >= minB.x);
	bool overlapY = (minA.y <= maxB.y) && (maxA.y >= minB.y);
	bool overlapZ = (minA.z <= maxB.z) && (maxA.z >= minB.z);

	bool isColliding = overlapX && overlapY && overlapZ;

	if (isColliding) {
		// Calculate overlap on each axis
		float overlapX = std::min(maxA.x, maxB.x) - std::max(minA.x, minB.x);
		float overlapY = std::min(maxA.y, maxB.y) - std::max(minA.y, minB.y);
		float overlapZ = std::min(maxA.z, maxB.z) - std::max(minA.z, minB.z);

		// Find axis with minimum overlap (collision axis)
		float minOverlap = std::min({ overlapX, overlapY, overlapZ });

		// Determine collision normal based on minimum overlap axis
		if (minOverlap == overlapX) {
			collisionData.collisionNormal = glm::vec3(minA.x < minB.x ? -1.f : 1.f, 0.f, 0.f);
		}
		else if (minOverlap == overlapY) {
			collisionData.collisionNormal = glm::vec3(0.f, minA.y < minB.y ? -1.f : 1.f, 0.f);
		}
		else {
			collisionData.collisionNormal = glm::vec3(0.f, 0.f, minA.z < minB.z ? -1.f : 1.f);
		}

		// Contact point is at the center of overlap region
		glm::vec3 overlapMin = glm::max(minA, minB);
		glm::vec3 overlapMax = glm::min(maxA, maxB);
		collisionData.contactPoint = (overlapMin + overlapMax) * 0.5f;
		collisionData.contactPointB = collisionData.contactPoint;

		// Penetration is the minimum overlap distance
		collisionData.penetration = minOverlap;

		collisionData.pObjA = &objA;
		collisionData.pObjB = &objB;

		// Generate tangent and bitangent from normal
		GenerateTangentBitangent(collisionData.collisionNormal, collisionData.tangent, collisionData.bitangent);
	}
	return isColliding;
}

bool OverlapOBBOBB(PhysicsObject& objA, PhysicsObject& objB, CollisionData& collisionData)
{
	// Implement OBB vs OBB collision detection (To be done IF ABSOLUTELY REQUIRED.)
	return false;
}

bool OverlapOBBAABB(PhysicsObject& OBB, PhysicsObject& AABB, CollisionData& collisionData)
{
	// Implement OBB vs AABB collision detection (To be done IF ABSOLUTELY REQUIRED.)
	return false;
}

bool OverlapSphereSphere(PhysicsObject& objA, PhysicsObject& objB, CollisionData& collisionData)
{
	// Calculate distance squared between sphere centers
	glm::vec3 diff = objB.position - objA.position;
	float distSquared = glm::dot(diff, diff);
	float radiiSum = objA.boundingBox.getRadius() + objB.boundingBox.getRadius();
	float radiiSumSquared = radiiSum * radiiSum;

	bool isColliding = distSquared <= radiiSumSquared;

	if (isColliding) {
		// Safely calculate collision normal
		collisionData.collisionNormal = SafeNormalize(diff, glm::vec3(0.f, 1.f, 0.f));

		// Contact points on each sphere surface
		float distance = std::sqrt(distSquared);
		collisionData.contactPoint = objA.position + collisionData.collisionNormal * objA.boundingBox.getRadius();
		collisionData.contactPointB = objB.position - collisionData.collisionNormal * objB.boundingBox.getRadius();

		// Penetration depth
		collisionData.penetration = radiiSum - distance;

		collisionData.pObjA = &objA;
		collisionData.pObjB = &objB;

		// Generate tangent and bitangent from normal
		GenerateTangentBitangent(collisionData.collisionNormal, collisionData.tangent, collisionData.bitangent);
	}
	return isColliding;
}

bool OverlapAABBSphere(PhysicsObject& AABB, PhysicsObject& Sphere, CollisionData& collisionData)
{
	// Get AABB bounds
	glm::vec3 aabbMin = AABB.position - AABB.boundingBox.getHalfExtents();
	glm::vec3 aabbMax = AABB.position + AABB.boundingBox.getHalfExtents();
	// Get sphere properties
	glm::vec3 sphereCenter = Sphere.position;
	float sphereRadius = Sphere.boundingBox.getRadius();

	// Find closest point on AABB to sphere center
	glm::vec3 closestPoint = glm::clamp(sphereCenter, aabbMin, aabbMax);
	glm::vec3 diff = sphereCenter - closestPoint;
	float distSquared = glm::dot(diff, diff);
	float radiusSquared = sphereRadius * sphereRadius;

	bool isColliding = distSquared <= radiusSquared;

	if (isColliding) {
		// Safely calculate collision normal pointing from AABB to sphere
		collisionData.collisionNormal = SafeNormalize(diff, glm::vec3(0.f, 1.f, 0.f));

		// Contact point on AABB surface (closest point)
		collisionData.contactPoint = closestPoint;

		// Contact point on sphere surface
		float distance = std::sqrt(distSquared);
		collisionData.contactPointB = sphereCenter - collisionData.collisionNormal * sphereRadius;

		// Penetration depth
		collisionData.penetration = sphereRadius - distance;

		collisionData.pObjA = &AABB;
		collisionData.pObjB = &Sphere;

		// Generate tangent and bitangent from normal
		GenerateTangentBitangent(collisionData.collisionNormal, collisionData.tangent, collisionData.bitangent);
	}
	return isColliding;
}

bool OverlapOBBSphere(PhysicsObject& OBB, PhysicsObject& Sphere, CollisionData& collisionData)
{
	// Get OBB properties
	glm::vec3 obbCenter = OBB.position;
	glm::vec3 sphereCenter = Sphere.position;
	glm::vec3 toSphere = sphereCenter - obbCenter;
	glm::vec3 halfExtents = OBB.boundingBox.getHalfExtents();
	float sphereRadius = Sphere.boundingBox.getRadius();

	// Get OBB axes from orientation
	glm::mat3 rotationMatrix = glm::mat3_cast(OBB.orientation);
	glm::vec3 obbAxes[3] = { rotationMatrix[0], rotationMatrix[1], rotationMatrix[2] };

	// Find closest point on OBB to sphere center
	glm::vec3 closestPointOnOBB = obbCenter;
	for (int i = 0; i < 3; ++i) {
		float distance = glm::dot(toSphere, obbAxes[i]);
		distance = glm::clamp(distance, -halfExtents[i], halfExtents[i]);
		closestPointOnOBB += obbAxes[i] * distance;
	}

	// Calculate collision
	glm::vec3 diff = sphereCenter - closestPointOnOBB;
	float distSquared = glm::dot(diff, diff);
	float radiusSquared = sphereRadius * sphereRadius;

	bool isColliding = distSquared <= radiusSquared;

	if (isColliding) {
		// Safely calculate collision normal
		collisionData.collisionNormal = SafeNormalize(diff, glm::vec3(0.f, 1.f, 0.f));

		// Contact point on OBB surface (closest point)
		collisionData.contactPoint = closestPointOnOBB;

		// Contact point on sphere surface
		float distance = std::sqrt(distSquared);
		collisionData.contactPointB = sphereCenter - collisionData.collisionNormal * sphereRadius;

		// Penetration depth
		collisionData.penetration = sphereRadius - distance;

		collisionData.pObjA = const_cast<PhysicsObject*>(&OBB);
		collisionData.pObjB = const_cast<PhysicsObject*>(&Sphere);

		// Generate tangent and bitangent from normal
		GenerateTangentBitangent(collisionData.collisionNormal, collisionData.tangent, collisionData.bitangent);
	}

	return isColliding;
}


void ApplyPositionalCorrection(PhysicsObject* objA, PhysicsObject* objB, const CollisionData& cd)
{
	if (!objA || !objB) return;

	float invMassA = (objA->mass > 0.0f) ? 1.0f / objA->mass : 0.0f;
	float invMassB = (objB->mass > 0.0f) ? 1.0f / objB->mass : 0.0f;
	float invMassSum = invMassA + invMassB;

	if (invMassSum < 1e-6f) return; // Both static

	const float percent = 0.2f;
	const float slop = 0.01f;
	float penetrationCorrection = std::max(cd.penetration - slop, 0.0f);
	glm::vec3 correction = (penetrationCorrection / invMassSum) * percent * cd.collisionNormal;

	if (invMassA > 0.0f) {
		objA->position -= invMassA * correction;
	}
	if (invMassB > 0.0f) {
		objB->position += invMassB * correction;
	}
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
	if (velocityAlongNormal > -1e-6f) {
		// Still apply positional correction even if not approaching
		ApplyPositionalCorrection(objA, objB, cd);
		return;
	}

	// Calculate restitution (bounciness)
	float e = std::min(objA->bounciness, objB->bounciness);

	// Prepare inverse mass for impulse calculation
	float invMassA = (objA->mass > 0.0f) ? 1.0f / objA->mass : 0.0f;
	float invMassB = (objB->mass > 0.0f) ? 1.0f / objB->mass : 0.0f;

	// If both objects are static, no resolution needed
	if (invMassA < 1e-6f && invMassB < 1e-6f) {
		return;
	}

	// Calculate rotational effect on impulse denominator
	glm::vec3 rACrossN = glm::cross(rA, cd.collisionNormal);
	glm::vec3 rBCrossN = glm::cross(rB, cd.collisionNormal);
	float rotEffectA = glm::dot(rACrossN, objA->invInertiaWorld * rACrossN);
	float rotEffectB = glm::dot(rBCrossN, objB->invInertiaWorld * rBCrossN);

	// Full inverse mass sum including rotational effects
	float invMassSum = invMassA + invMassB + rotEffectA + rotEffectB;

	// Guard against zero inverse mass sum
	if (invMassSum < 1e-6f) {
		ApplyPositionalCorrection(objA, objB, cd);
		return;
	}

	// Impulse calculation for normal force
	float j = -(1.0f + e) * velocityAlongNormal / invMassSum;
	glm::vec3 impulse = j * cd.collisionNormal;

	// Apply linear impulses
	if (invMassA > 0.0f) {
		objA->velocity -= invMassA * impulse;
	}
	if (invMassB > 0.0f) {
		objB->velocity += invMassB * impulse;
	}

	// Apply angular impulses (torque from contact point offset)
	glm::vec3 torqueA = glm::cross(rA, -impulse);
	glm::vec3 torqueB = glm::cross(rB, impulse);
	objA->angularVelocity += objA->invInertiaWorld * torqueA;
	objB->angularVelocity += objB->invInertiaWorld * torqueB;

	// Recalculate contact velocities after normal impulse
	velA_contact = objA->velocity + glm::cross(objA->angularVelocity, rA);
	velB_contact = objB->velocity + glm::cross(objB->angularVelocity, rB);
	relativeVelocity = velB_contact - velA_contact;

	// Calculate tangential velocity (velocity perpendicular to collision normal)
	glm::vec3 tangentVelocity = relativeVelocity - glm::dot(relativeVelocity, cd.collisionNormal) * cd.collisionNormal;
	float tangentSpeed = glm::length(tangentVelocity);

	// Apply friction impulse if there's tangential motion
	if (tangentSpeed > 1e-6f) {
		glm::vec3 tangentDir = tangentVelocity / tangentSpeed;

		// Use average friction coefficient
		float mu = std::min(objA->friction, objB->friction);

		// Calculate friction impulse magnitude
		// Coulomb friction: friction cannot exceed j * mu
		float frictionMagnitude = -tangentSpeed / invMassSum;

		// Clamp friction to Coulomb limit
		frictionMagnitude = std::max(frictionMagnitude, -j * mu);

		glm::vec3 frictionImpulse = frictionMagnitude * tangentDir;

		// Apply friction impulses
		if (invMassA > 0.0f) {
			objA->velocity -= invMassA * frictionImpulse;
		}
		if (invMassB > 0.0f) {
			objB->velocity += invMassB * frictionImpulse;
		}

		// Apply friction torque
		glm::vec3 frictionTorqueA = glm::cross(rA, -frictionImpulse);
		glm::vec3 frictionTorqueB = glm::cross(rB, frictionImpulse);
		objA->angularVelocity += objA->invInertiaWorld * frictionTorqueA;
		objB->angularVelocity += objB->invInertiaWorld * frictionTorqueB;
	}

	// Positional Correction (prevent penetration drift)
	ApplyPositionalCorrection(objA, objB, cd);
}
