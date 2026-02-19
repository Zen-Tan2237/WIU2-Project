#include "BoundingBox.h"

BoundingBox::BoundingBox() :
	type(Type::NONE),
	height(0.f),
	width(0.f),
	depth(0.f),
	minPoint(glm::vec3(0.f)),
	maxPoint(glm::vec3(0.f)),
	center(glm::vec3(0.f)),
	halfExtents(glm::vec3(0.f)),
	radius(0.f)
{
}

BoundingBox::~BoundingBox() {

}

void BoundingBox::InitBB() {
	if (type == Type::OBB){
		halfExtents = glm::vec3(width * 0.5f, height * 0.5f, depth * 0.5f);
		center = glm::vec3(halfExtents.x, halfExtents.y, halfExtents.z);
		minPoint = center - halfExtents;
		maxPoint = center + halfExtents;
	}
	else if (type == Type::SPHERE) {
		
	}
}

void BoundingBox::UpdateBB(const glm::vec3& newCenter) {
	if (type == Type::OBB) {
	}
	else if (type == Type::SPHERE) {
	}
}