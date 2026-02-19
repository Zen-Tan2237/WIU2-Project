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
	glm::vec3 contactPointB{}; // For cases where we need the contact point on both objects (e.g., OBB vs OBB)
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

bool CheckCollision(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData);
bool OverlapAABBAABB(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData);
bool OverlapOBBOBB(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData);
bool OverlapOBBAABB(const PhysicsObject& OBB, const PhysicsObject& AABB, CollisionData& collisionData);

bool OverlapSphereSphere(const PhysicsObject& objA, const PhysicsObject& objB, CollisionData& collisionData);

bool OverlapAABBSphere(const PhysicsObject& AABB, const PhysicsObject& Sphere, CollisionData& collisionData);
bool OverlapOBBSphere(const PhysicsObject& OBB, const PhysicsObject& Sphere, CollisionData& collisionData);

void ResolveCollision(CollisionData& collisionData);

