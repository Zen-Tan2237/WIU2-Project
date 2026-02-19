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
	orientation(glm::mat3(1.f)),
	radius(glm::vec3(0.f)),
	sphereCenter(glm::vec3(0.f))
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
	orientation = glm::mat3(1.f);
	radius = glm::vec3(0.f);
	sphereCenter = glm::vec3(0.f);
}

