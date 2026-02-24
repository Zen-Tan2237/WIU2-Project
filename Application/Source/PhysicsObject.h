#pragma once
#include "BoundingBox.h"


// Include glm headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>


// Usage Instructions:
// 1. Create an instance of PhysicsObject for any objects to have physics properties and behaviors.
// 1b. Feel free to inherit from this class to add more specific properties (ie. player being the first person cam)
// 2. Call InitPhysicsObject() to init the physics properties of the object, like mass, position and bounding box properties.
// 2b. The only bounding box properties needed to be set is the type and dimensions, the others are calculated during initialisation.
// 2c. note that the dimensions are a glm::vec3, for spheres, x component is for radius, rest are ignored.
// For OBB, x is width, y is height and z is depth.
// 2d. miscSettings is a bool array of size 2. miscSettings[0] is for gravity and miscSettings[1] is for drag. Set to true to enable, false to disable.
// 3. Call UpdatePhysics() at the end of every frame to update the physics properties of the object, like position and velocity, based on the forces applied to it.

class PhysicsObject
{
public:
	PhysicsObject() {}
	virtual ~PhysicsObject() {}

	// Transform properties
	glm::vec3 position{ 0.f };
	glm::quat orientation{ 1.f, 0.f, 0.f, 0.f };
	glm::vec3 velocity{ 0.f }; // Linear velocity
	glm::vec3 acceleration{ 0.f }; // Linear acceleration
	
	// Angular physics properties
	glm::vec3 angularVelocity{ 0.f }; // Angular velocity in radians per second
	glm::mat3 invInertiaLocal{ 0.f }; // Local inverse inertia tensor (3x3 matrix)
	glm::mat3 invInertiaWorld{ 0.f }; // World inverse inertia tensor (3x3 matrix)


	// misc properties for physics calculations
	float mass{ 1.f };
	float bounciness{ 0.5f }; // Coefficient of restitution for collisions
	float friction{ 0.5f }; // Coefficient of friction for collisions (Currently a stub, to be added when needed)

	// misc properties to enable/disable certain physics features
	bool GravityEnabled{ false };
	bool DragEnabled{ true };

	BoundingBox boundingBox;

	void AddForce(const glm::vec3& force);
	void AddImpulse(const glm::vec3& impulse);
	void AddImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& point);
	void UpdatePhysics(double dt);
	void AddBoundingBox(BoundingBox box);
	void InitPhysicsObject(glm::vec3 pos, float mass, BoundingBox::Type bbType, glm::vec3 bbDimensions, bool* miscSettings);
	void InitPhysicsObject(glm::vec3 pos, float mass, BoundingBox::Type bbType, glm::vec3 bbDimensions, glm::quat orientation, bool* miscSettings);
	void InitPhysicsObject(glm::vec3 pos, float mass, BoundingBox::Type bbType, glm::vec3 bbDimensions, float angleDegrees, glm::vec3 rotationAxis, bool* miscSettings);
	void UpdateInertiaTensors();
	void ResetPhysicsProperties();
	void RotateOrientation(float angleDegrees, glm::vec3 rotationAxis);
	void SetOrientation(float angleDegrees, glm::vec3 rotationAxis);

	bool isOBBanAABB() const;


protected:
	void ForcesSetZero();
	glm::vec3 totalForces{ 0.f };
	glm::vec3 totalTorque{ 0.f };
};



