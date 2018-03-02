#include "Frustum.h"


Frustum* Frustum::frustum_instance = 0;

Frustum::Frustum()
{
	PlaneClass a(0, 0, 0, 0);
	for (int i = 0; i < 6; i++)
	{
		frustum.push_back(a);
	}
}


Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum(float screenDepth, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	float zMinimum, r;
	glm::mat4 matrix = projectionMatrix * viewMatrix;

	glm::vec4 rowX = glm::row(matrix, 0);
	glm::vec4 rowY = glm::row(matrix, 1);
	glm::vec4 rowZ = glm::row(matrix, 2);
	glm::vec4 rowW = glm::row(matrix, 3);

	glm::vec4 temp = normalize(rowW + rowX);
	PlaneClass temp1(temp.x, temp.y, temp.z, temp.w);
	frustum[0] = temp1;

	temp = normalize(rowW - rowX);
	PlaneClass temp2(temp.x, temp.y, temp.z, temp.w);
	frustum[1] = temp2;

	temp = normalize(rowW + rowY);
	PlaneClass temp3(temp.x, temp.y, temp.z, temp.w);
	frustum[2] = temp1;

	temp = normalize(rowW - rowY);
	PlaneClass temp4(temp.x, temp.y, temp.z, temp.w);
	frustum[3] = temp4;

	temp = normalize(rowW + rowZ);
	PlaneClass temp5(temp.x, temp.y, temp.z, temp.w);
	frustum[4] = temp5;

	temp = normalize(rowW - rowZ);
	PlaneClass temp6(temp.x, temp.y, temp.z, temp.w);
	frustum[5] = temp6;

	for (int i = 0; i < 6; i++)
	{
		glm::vec3 normal(frustum[i].a, frustum[i].b, frustum[i].c);
		float length = glm::length(normal);
		frustum[i] = frustum[i]/ -length;
	}

	
}

bool Frustum::CheckSphere(glm::vec3 center, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		float dist = frustum[i].a * center.x + frustum[i].b * center.y + frustum[i].c * center.z + frustum[i].d - radius;
		if (dist > 0)
		{
			
			return false;
		}
	}

	
	return true;
}
