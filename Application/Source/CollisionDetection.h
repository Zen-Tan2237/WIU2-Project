#pragma once
#include "PhysicsObject.h"

// Include glm headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

class CollisionData
{
public:
	float penetration{};
	glm::vec3 collisionNormal{};
	glm::vec3 contactPoint{};
	glm::vec3 contactPointB{};
	glm::vec3 tangent{};
	glm::vec3 bitangent{};
	PhysicsObject* pObjA;
	PhysicsObject* pObjB;

	CollisionData() :
		penetration(0.f),
		collisionNormal(glm::vec3(0.f)),
		contactPoint(glm::vec3(0.f)),
		contactPointB(glm::vec3(0.f)),
		tangent(glm::vec3(0.f)),
		pObjA(nullptr),
		pObjB(nullptr)
	{
	}
	~CollisionData() {
		pObjA = nullptr;
		pObjB = nullptr;
	}
};

// Collision detection functions

bool CheckCollision(PhysicsObject& objA, PhysicsObject& objB, CollisionData& collisionData);
void ClearCollisionData(CollisionData& cd);
bool OverlapAABBAABB(PhysicsObject& objA, PhysicsObject& objB, CollisionData& collisionData);
bool OverlapOBBOBB(PhysicsObject& objA, PhysicsObject& objB, CollisionData& collisionData);
bool OverlapOBBAABB(PhysicsObject& OBB, PhysicsObject& AABB, CollisionData& collisionData);

bool OverlapSphereSphere(PhysicsObject& objA, PhysicsObject& objB, CollisionData& collisionData);

bool OverlapAABBSphere(PhysicsObject& AABB, PhysicsObject& Sphere, CollisionData& collisionData);
bool OverlapOBBSphere(PhysicsObject& OBB, PhysicsObject& Sphere, CollisionData& collisionData);

void ResolveCollision(CollisionData& collisionData);
void ResolveCollision(CollisionData& collisionData, float dt);

// Helpers
static float ProjectOBBRadiusOnAxis(const glm::vec3 axes[3], const glm::vec3& halfExtents, const glm::vec3& axis);
glm::vec3 SafeNormalize(const glm::vec3& vec, const glm::vec3& fallback = glm::vec3(0.f, 1.f, 0.f));
void GenerateTangentBitangent(const glm::vec3& normal, glm::vec3& outTangent, glm::vec3& outBitangent);
glm::vec3 GetSupportPoint(const glm::vec3& center, const glm::vec3 axes[3], const glm::vec3& halfExtents, const glm::vec3& direction);
static float ClampFloat(float v, float lo, float hi);
void ApplyPositionCorrection(CollisionData& cd);
