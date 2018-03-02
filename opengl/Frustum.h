#pragma once
#include "glm/gtc/matrix_access.hpp"
#include "PlaneClass.h"
#include<iostream>
#include <vector>
class Frustum
{
public:
	Frustum();
	~Frustum();
	void ConstructFrustum(float screenDepth, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	bool CheckSphere(glm::vec3 centre, float radius);

	static Frustum *instance()
	{
		if (!frustum_instance)
			frustum_instance = new Frustum;

		return frustum_instance;
	}
	
private:
	std::vector<PlaneClass> frustum;
	static Frustum *frustum_instance;
};

