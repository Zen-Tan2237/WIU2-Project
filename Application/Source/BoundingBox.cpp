#include "BoundingBox.h"

BoundingBox::BoundingBox() :
	type(Type::NONE),
	height(glm::vec3(0.f)),
	width(glm::vec3(0.f)),
	depth(glm::vec3(0.f)),
	minPoint(glm::vec3(0.f)),
	maxPoint(glm::vec3(0.f)),
	center(glm::vec3(0.f)),
	halfExtents(glm::vec3(0.f)),
	radius(0.f)
{
}

BoundingBox::~BoundingBox() {
	height = glm::vec3(0.f);
	width = glm::vec3(0.f);
	depth = glm::vec3(0.f);
	minPoint = glm::vec3(0.f);
	maxPoint = glm::vec3(0.f);
	center = glm::vec3(0.f);
	halfExtents = glm::vec3(0.f);
	radius = 0.f;
}

void BoundingBox::InitBB() {
	if (type == Type::OBB){
		halfExtents = glm::vec3(width.x * 0.5f, height.y * 0.5f, depth.z * 0.5f);
		center = glm::vec3(halfExtents.x, halfExtents.y, halfExtents.z);
		minPoint = center - halfExtents;
		maxPoint = center + halfExtents;
	}
	else if (type == Type::SPHERE) {
		// nothing
	}
}

