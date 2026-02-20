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
	// Get the two physics objects
	PhysicsObject* A = cd.pObjA;
	PhysicsObject * B = cd.pObjB;
	if (!A || !B) return;

	// Get collision normal and penetration
	glm::vec3 normal = cd.collisionNormal;
	float penetration = cd.penetration;

	// Calculate inverse masses
	float invMassA = (A->mass > 0.0f) ? 1.0f / A->mass : 0.0f;
	float invMassB = (B->mass > 0.0f) ? 1.0f / B->mass : 0.0f;
	float invMassSum = invMassA + invMassB;

	// Positional correction to avoid sinking
	const float SLOP = 0.01f;
	const float MAX_CORRECTION = 1.0f;

	// Apply positional correction
	if (invMassSum > 0.0f && penetration > SLOP) {
		float correction = std::min((penetration - SLOP) / invMassSum, MAX_CORRECTION);
		glm::vec3 separation = normal * correction;
		if (invMassA > 0.0f)
			A->position -= separation * invMassA;
		if (invMassB > 0.0f)
			B->position += separation * invMassB;
	}

	// Calculate relative velocity at contact point
	glm::vec3 ra = cd.contactPoint - A->position;
	glm::vec3 rb = cd.contactPoint - B->position;
	glm::vec3 velA = A->velocity + glm::vec3(-ra.y, ra.x, 0.f) * A->angularVelocity;
	glm::vec3 velB = B->velocity + glm::vec3(-rb.y, rb.x, 0.f) * B->angularVelocity;
	glm::vec3 relVel = velB - velA;

	float velAlongNormal = glm::dot(relVel, normal);
	if (velAlongNormal > 0.0f) return;

	// Calculate bounciness coefficient
	float e = (A->bounciness + B->bounciness) * 0.5f;


	// Calculate impulse scalar
	float raCrossN = ra.x * normal.y - ra.y * normal.x;
	float rbCrossN = rb.x * normal.y - rb.y * normal.x;
	float raLengthSquared = glm::length(ra) * glm::length(ra);
	float rbLengthSquared = glm::length(rb) * glm::length(rb);
	float invInertiaA = (A->mass > 0.0f) ? 1.0f / (A->mass * (raLengthSquared + 1e-6f)) : 0.0f;
	float invInertiaB = (B->mass > 0.0f) ? 1.0f / (B->mass * (rbLengthSquared + 1e-6f)) : 0.0f;

	// Denominator for impulse calculation
	// Formula: j = -(1 + e) * velAlongNormal / (invMassA + invMassB + (raCrossN^2) * invInertiaA + (rbCrossN^2) * invInertiaB)
	float denom = invMassSum + (raCrossN * raCrossN) * invInertiaA + (rbCrossN * rbCrossN) * invInertiaB;
	float j = -(1.0f + e) * velAlongNormal;
	// Avoid division by zero
	if (denom > 0.0f) j /= denom;
	else j = 0.0f;

	// Apply impulse along the collision normal * by the impulse scalar
	glm::vec3 impulse = normal * j;

	// Update linear velocities
	if (invMassA > 0.0f)
		A->velocity -= impulse * invMassA;
	if (invMassB > 0.0f)
		B->velocity += impulse * invMassB;

	// Update angular velocities (Yes, this thing can spin when hit off an angle!)
	if (A->mass > 0.0f) {
		float inertiaA = A->mass * (raLengthSquared + 1e-6f);
		float torqueA = ra.x * impulse.y - ra.y * impulse.x;
		A->angularVelocity -= torqueA / inertiaA;
	}
	if (B->mass > 0.0f) {
		float inertiaB = B->mass * (rbLengthSquared + 1e-6f);
		float torqueB = rb.x * impulse.y - rb.y * impulse.x;
		B->angularVelocity += torqueB / inertiaB;
	}
}
