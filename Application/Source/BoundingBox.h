#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

class BoundingBox
{
public: 
	enum class Type {
		NONE,
		OBB,
		SPHERE
	};

private:


	Type type;

	// z forward, y up, x right

	// For OBB (And AABB when rotation == 0 || 180 deg) 
	float width; // "Dev" Input for x-height
	float height; // "Dev" Input for y-width
	float depth; // "Dev" Input for z-depth
	glm::vec3 minPoint; // To be calced during instance creation
	glm::vec3 maxPoint; // To be calced during instance creation
	glm::vec3 center; // To be calced during instance creation
	glm::vec3 halfExtents; // To be calced during instance creation

	// For Sphere
	float radius; // "Dev" Input for radius (STRICTLY USE RADIUS, NOT DIAMETER)

public:
	BoundingBox();
	~BoundingBox();

	void InitBB();
	void UpdateBB(const glm::vec3& newCenter);

	// Getters and setters for the bounding box properties
	void setType(const Type&t) { type = t; }
	void setHeight(const float& h) { height = h; }
	void setWidth(const float& w) { width = w; }
	void setDepth(const float& d) { depth = d; }
	void setMinPoint(const glm::vec3& min) { minPoint = min; }
	void setMaxPoint(const glm::vec3& max) { maxPoint = max; }
	void setCenter(const glm::vec3& c) { center = c; }
	void setHalfExtents(const glm::vec3& he) { halfExtents = he; }
	void setRadius(const float& r) { radius = r; }
	
	float getHeight() const { return height; }
	float getWidth() const { return width; }
	float getDepth() const { return depth; }
	glm::vec3 getMinPoint() const { return minPoint; }
	glm::vec3 getMaxPoint() const { return maxPoint; }
	glm::vec3 getCenter() const { return center; }
	glm::vec3 getHalfExtents() const { return halfExtents; }
	float getRadius() const { return radius; }
	Type getType() const { return type; }
};

