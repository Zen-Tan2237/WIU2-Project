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
	glm::vec3 height; // "Dev" Input for x-width
	glm::vec3 width; // "Dev" Input for y-height
	glm::vec3 depth; // "Dev" Input for z-depth
	glm::vec3 minPoint; // To be calced during instance creation
	glm::vec3 maxPoint; // To be calced during instance creation
	glm::vec3 center; // To be calced during instance creation
	glm::vec3 halfExtents; // To be calced during instance creation
	glm::mat3 orientation; // To be calced during instance creation

	// For Sphere
	float radius; // "Dev" Input for radius (STRICTLY USE RADIUS, NOT DIAMETER)
	glm::vec3 sphereCenter; // To be calced during instance creation

public:
	BoundingBox();
	~BoundingBox();

	// Getters and setters for the bounding box properties
	void setType(Type t) { type = t; }
	void setHeight(const glm::vec3& h) { height = h; }
	void setWidth(const glm::vec3& w) { width = w; }
	void setDepth(const glm::vec3& d) { depth = d; }
	void setMinPoint(const glm::vec3& min) { minPoint = min; }
	void setMaxPoint(const glm::vec3& max) { maxPoint = max; }
	void setCenter(const glm::vec3& c) { center = c; }
	void setHalfExtents(const glm::vec3& he) { halfExtents = he; }
	void setOrientation(const glm::mat3& o) { orientation = o; }
	void setRadius(const glm::vec3& r) { radius = r; }
	void setSphereCenter(const glm::vec3& sc) { sphereCenter = sc; }
	
	glm::vec3 getHeight() const { return height; }
	glm::vec3 getWidth() const { return width; }
	glm::vec3 getDepth() const { return depth; }
	glm::vec3 getMinPoint() const { return minPoint; }
	glm::vec3 getMaxPoint() const { return maxPoint; }
	glm::vec3 getCenter() const { return center; }
	glm::vec3 getHalfExtents() const { return halfExtents; }
	glm::mat3 getOrientation() const { return orientation; }
	float getRadius() const { return radius; }
	glm::vec3 getSphereCenter() const { return sphereCenter; }
	Type getType() const { return type; }
};

