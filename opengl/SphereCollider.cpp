#include "SphereCollider.h"



SphereCollider::SphereCollider()
{
}


SphereCollider::~SphereCollider()
{
}

void SphereCollider::InitParameters(float radius, glm::vec3 position)
{
	radiusOfCollider = radius;
	positionOfCollider = position;
}

int SphereCollider::GetRadius()
{
	return radiusOfCollider;
}

glm::vec3 SphereCollider::GetPosition()
{
	return positionOfCollider;
}

void SphereCollider::SetPosition(glm::vec3 position)
{
	positionOfCollider = position;
}


