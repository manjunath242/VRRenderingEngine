#include <glm/glm.hpp>
#pragma once

class SphereCollider
{
public:
	SphereCollider();
	~SphereCollider();

	void InitParameters(float radius, glm::vec3 position);
	int GetRadius();
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 position);

private:
	float radiusOfCollider;
	glm::vec3 positionOfCollider;
};

