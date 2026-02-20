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

// Helper function to get the support point of a box in a direction
glm::vec3 GetSupportPoint(const glm::vec3& center, const glm::vec3 axes[3], const glm::vec3& halfExtents, const glm::vec3& direction)
{
	glm::vec3 point = center;
	for (int i = 0; i < 3; ++i) {
		float sign = (glm::dot(axes[i], direction) >= 0.0f) ? 1.0f : -1.0f;
		point += axes[i] * (halfExtents[i] * sign);
	}
	return point;
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
	// Get OBB properties
	glm::vec3 centerA = objA.position;
	glm::vec3 centerB = objB.position;
	glm::vec3 halfExtentsA = objA.boundingBox.getHalfExtents();
	glm::vec3 halfExtentsB = objB.boundingBox.getHalfExtents();

	// Get rotation matrices from orientations
	glm::mat3 rotA = glm::mat3_cast(objA.orientation);
	glm::mat3 rotB = glm::mat3_cast(objB.orientation);

	// Extract axes
	glm::vec3 axesA[3] = { rotA[0], rotA[1], rotA[2] };
	glm::vec3 axesB[3] = { rotB[0], rotB[1], rotB[2] };

	// Vector between centers
	glm::vec3 translation = centerB - centerA;

	// Separating Axis Theorem (SAT) - test 15 axes
	// 3 axes from A, 3 axes from B, 9 cross products

	float minPenetration = FLT_MAX;
	glm::vec3 minAxis = glm::vec3(0.f);
	int minAxisIndex = -1;

	// Test axes from OBB A (3 axes)
	for (int i = 0; i < 3; ++i) {
		glm::vec3 axis = axesA[i];
		float distOnAxis = std::abs(glm::dot(translation, axis));

		// Project extents onto axis
		float projA = halfExtentsA[i];
		float projB = 0.0f;
		for (int j = 0; j < 3; ++j) {
			projB += halfExtentsB[j] * std::abs(glm::dot(axesB[j], axis));
		}

		float penetration = projA + projB - distOnAxis;
		if (penetration < 0.0f) return false;

		if (penetration < minPenetration) {
			minPenetration = penetration;
			minAxis = axis;
			minAxisIndex = i;
		}
	}

	// Test axes from OBB B (3 axes)
	for (int i = 0; i < 3; ++i) {
		glm::vec3 axis = axesB[i];
		float distOnAxis = std::abs(glm::dot(translation, axis));

		// Project extents onto axis
		float projA = 0.0f;
		for (int j = 0; j < 3; ++j) {
			projA += halfExtentsA[j] * std::abs(glm::dot(axesA[j], axis));
		}
		float projB = halfExtentsB[i];

		float penetration = projA + projB - distOnAxis;
		if (penetration < 0.0f) return false;

		if (penetration < minPenetration) {
			minPenetration = penetration;
			minAxis = axis;
			minAxisIndex = 3 + i;
		}
	}

	// Test cross product axes (9 axes)
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			glm::vec3 axis = glm::cross(axesA[i], axesB[j]);
			float lengthSq = glm::dot(axis, axis);

			// Skip near-parallel axes
			if (lengthSq < 1e-6f) continue;

			axis = axis / std::sqrt(lengthSq);
			float distOnAxis = std::abs(glm::dot(translation, axis));

			// Project extents onto axis
			float projA = 0.0f;
			for (int k = 0; k < 3; ++k) {
				projA += halfExtentsA[k] * std::abs(glm::dot(axesA[k], axis));
			}
			float projB = 0.0f;
			for (int k = 0; k < 3; ++k) {
				projB += halfExtentsB[k] * std::abs(glm::dot(axesB[k], axis));
			}

			float penetration = projA + projB - distOnAxis;
			if (penetration < 0.0f) return false;

			if (penetration < minPenetration) {
				minPenetration = penetration;
				minAxis = axis;
				minAxisIndex = 6 + i * 3 + j;
			}
		}
	}

	// Collision detected - fill collision data
	collisionData.penetration = minPenetration;

	// Ensure normal points from A to B
	if (glm::dot(minAxis, translation) < 0.0f) {
		minAxis = -minAxis;
	}
	collisionData.collisionNormal = SafeNormalize(minAxis, glm::vec3(0.f, 1.f, 0.f));

	// Use support points for better contact approximation
	collisionData.contactPoint = GetSupportPoint(centerA, axesA, halfExtentsA, collisionData.collisionNormal);
	collisionData.contactPointB = GetSupportPoint(centerB, axesB, halfExtentsB, -collisionData.collisionNormal);

	collisionData.pObjA = &objA;
	collisionData.pObjB = &objB;

	// Generate tangent and bitangent from normal
	GenerateTangentBitangent(collisionData.collisionNormal, collisionData.tangent, collisionData.bitangent);

	return true;
}

bool OverlapOBBAABB(PhysicsObject& OBB, PhysicsObject& AABB, CollisionData& collisionData)
{
	// Get OBB properties
	glm::vec3 obbCenter = OBB.position;
	glm::vec3 obbHalfExtents = OBB.boundingBox.getHalfExtents();

	// Get OBB axes from orientation
	glm::mat3 rotOBB = glm::mat3_cast(OBB.orientation);
	glm::vec3 obbAxes[3] = { rotOBB[0], rotOBB[1], rotOBB[2] };

	// Get AABB properties
	glm::vec3 aabbCenter = AABB.position;
	glm::vec3 aabbHalfExtents = AABB.boundingBox.getHalfExtents();

	// AABB axes are world-aligned
	glm::vec3 aabbAxes[3] = {
		glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0.f, 0.f, 1.f)
	};

	// Vector between centers
	glm::vec3 translation = aabbCenter - obbCenter;

	// Separating Axis Theorem (SAT) - test 15 axes
	// 3 axes from OBB, 3 axes from AABB (world), 9 cross products

	float minPenetration = FLT_MAX;
	glm::vec3 minAxis = glm::vec3(0.f);

	// Test OBB axes (3 axes)
	for (int i = 0; i < 3; ++i) {
		glm::vec3 axis = obbAxes[i];
		float distOnAxis = std::abs(glm::dot(translation, axis));

		// Project OBB extents onto axis
		float projOBB = obbHalfExtents[i];

		// Project AABB extents onto axis
		float projAABB = 0.0f;
		for (int j = 0; j < 3; ++j) {
			projAABB += aabbHalfExtents[j] * std::abs(glm::dot(aabbAxes[j], axis));
		}

		float penetration = projOBB + projAABB - distOnAxis;
		if (penetration < 0.0f) return false;

		if (penetration < minPenetration) {
			minPenetration = penetration;
			minAxis = axis;
		}
	}

	// Test AABB axes (3 axes - world aligned)
	for (int i = 0; i < 3; ++i) {
		glm::vec3 axis = aabbAxes[i];
		float distOnAxis = std::abs(glm::dot(translation, axis));

		// Project OBB extents onto axis
		float projOBB = 0.0f;
		for (int j = 0; j < 3; ++j) {
			projOBB += obbHalfExtents[j] * std::abs(glm::dot(obbAxes[j], axis));
		}

		// Project AABB extents onto axis
		float projAABB = aabbHalfExtents[i];

		float penetration = projOBB + projAABB - distOnAxis;
		if (penetration < 0.0f) return false;

		if (penetration < minPenetration) {
			minPenetration = penetration;
			minAxis = axis;
		}
	}

	// Test cross product axes (9 axes)
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			glm::vec3 axis = glm::cross(obbAxes[i], aabbAxes[j]);
			float lengthSq = glm::dot(axis, axis);

			// Skip near-parallel axes
			if (lengthSq < 1e-6f) continue;

			axis = axis / std::sqrt(lengthSq);
			float distOnAxis = std::abs(glm::dot(translation, axis));

			// Project OBB extents onto axis
			float projOBB = 0.0f;
			for (int k = 0; k < 3; ++k) {
				projOBB += obbHalfExtents[k] * std::abs(glm::dot(obbAxes[k], axis));
			}

			// Project AABB extents onto axis
			float projAABB = 0.0f;
			for (int k = 0; k < 3; ++k) {
				projAABB += aabbHalfExtents[k] * std::abs(glm::dot(aabbAxes[k], axis));
			}

			float penetration = projOBB + projAABB - distOnAxis;
			if (penetration < 0.0f) return false;

			if (penetration < minPenetration) {
				minPenetration = penetration;
				minAxis = axis;
			}
		}
	}

	// Collision detected - fill collision data
	collisionData.penetration = minPenetration;

	// Ensure normal points from OBB to AABB
	if (glm::dot(minAxis, translation) < 0.0f) {
		minAxis = -minAxis;
	}
	collisionData.collisionNormal = SafeNormalize(minAxis, glm::vec3(0.f, 1.f, 0.f));

	// Use support points for better contact approximation
	collisionData.contactPoint = GetSupportPoint(obbCenter, obbAxes, obbHalfExtents, collisionData.collisionNormal);
	collisionData.contactPointB = GetSupportPoint(aabbCenter, aabbAxes, aabbHalfExtents, -collisionData.collisionNormal);

	collisionData.pObjA = &OBB;
	collisionData.pObjB = &AABB;

	// Generate tangent and bitangent from normal
	GenerateTangentBitangent(collisionData.collisionNormal, collisionData.tangent, collisionData.bitangent);

	return true;
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
	PhysicsObject* B = cd.pObjB;
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
	glm::vec3 rb = cd.contactPointB - B->position;

	// Calculate velocity at contact points including angular velocity contribution
	glm::vec3 velA = A->velocity + glm::cross(A->angularVelocity, ra);
	glm::vec3 velB = B->velocity + glm::cross(B->angularVelocity, rb);
	glm::vec3 relVel = velB - velA;

	float velAlongNormal = glm::dot(relVel, normal);
	if (velAlongNormal > 0.0f) return; // Objects separating

	// Calculate bounciness coefficient
	float e = (A->bounciness + B->bounciness) * 0.5f;

	// Calculate cross products for rotational contribution
	glm::vec3 raCrossN = glm::cross(ra, normal);
	glm::vec3 rbCrossN = glm::cross(rb, normal);

	// Get inverse inertia tensors
	glm::mat3 invInertiaA = A->invInertiaWorld;
	glm::mat3 invInertiaB = B->invInertiaWorld;

	// Calculate rotational contributions to impulse denominator
	// Formula: (r × n)^T * I^-1 * (r × n)
	float rotationalA = glm::dot(raCrossN, invInertiaA * raCrossN);
	float rotationalB = glm::dot(rbCrossN, invInertiaB * rbCrossN);

	// Denominator for impulse calculation
	float denom = invMassSum + rotationalA + rotationalB;

	// Calculate impulse magnitude
	float j = -(1.0f + e) * velAlongNormal;
	if (denom > 0.0f)
		j /= denom;
	else
		j = 0.0f;

	// Apply impulse along the collision normal
	glm::vec3 impulse = normal * j;


	// Update linear velocities
	if (invMassA > 0.0f)
		A->velocity -= impulse * invMassA;
	if (invMassB > 0.0f)
		B->velocity += impulse * invMassB;

	// Calculate and apply angular impulses using the CORRECT contact points
	if (invMassA > 0.0f) {
		glm::vec3 angularImpulseA = glm::cross(ra, impulse);
		A->angularVelocity -= invInertiaA * angularImpulseA;
	}
	if (invMassB > 0.0f) {
		glm::vec3 angularImpulseB = glm::cross(rb, impulse);
		B->angularVelocity += invInertiaB * angularImpulseB;
	}
}
