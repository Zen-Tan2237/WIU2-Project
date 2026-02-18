#pragma once
#include "BoundingBox.h"


// Include glm headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

class PhysicsObject
{
	// Base class for all objects used in this physics engine.
	// Can be used directly, inherit when you want to add more properties or functions.

public:
	PhysicsObject() {}
	virtual ~PhysicsObject() {}

	glm::vec3 pos{ 0.f };
	glm::vec3 vel{ 0.f };
	glm::vec3 acc{ 0.f };
	float mass{ 1.f }; // 0.f is immovable... Units in kg.

	// Angular properties
	glm::vec3 angularVel{ 0.f };
	float angleDeg{ 0.f };

	BoundingBox boundingBox;

	void AddForce(const glm::vec3& force);
	void AddTorque(const glm::vec3& torque);
	void AddImpulse(const glm::vec3& impulse);
	void AddAngularImpulse(const glm::vec3& angularImpulse);
	void UpdatePhysics(double dt);
	void AddBoundingBox(BoundingBox box);

	bool isOBBanAABB() const;


protected:
	glm::vec3 totalForces{ 0.f };
};



