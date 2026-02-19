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

	// quaternions gon kill me rn. 
public:
	PhysicsObject() {}
	virtual ~PhysicsObject() {}

	// Transform properties
	glm::vec3 position{ 0.f };
	glm::quat orientation{ 1.f, 0.f, 0.f, 0.f };
	glm::vec3 velocity{ 0.f }; // Linear velocity
	
	// Angular physics properties
	glm::vec3 angularVelocity{ 0.f }; // Angular velocity in radians per second
	glm::mat3 invInertiaLocal{ 1.f }; // Local inverse inertia tensor (3x3 matrix)
	glm::mat3 invInertiaWorld{ 1.f }; // World inverse inertia tensor (3x3 matrix)


	// misc properties for physics calculations
	float mass{ 1.f };

	// misc properties to enable/disable certain physics features
	bool GravityEnabled{ false };
	bool DragEnabled{ false };

	BoundingBox boundingBox;

	void AddForce(const glm::vec3& force);
	void AddImpulse(const glm::vec3& impulse);
	void AddImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& point);
	void UpdatePhysics(double dt);
	void AddBoundingBox(BoundingBox box);

	bool isOBBanAABB() const;


protected:
	void ForcesSetZero();
	glm::vec3 totalForces{ 0.f };
	glm::vec3 totalTorque{ 0.f };
};



