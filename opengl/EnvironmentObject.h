#pragma once
#include "Model.h"
#include <time.h> 
class EnvironmentObject : public Model
{
public:
	EnvironmentObject();
	~EnvironmentObject();
	void SetVelocity(glm::vec3 velocity);
	void Render(Shader *shader);
	float DistanceFromPlayer();
	glm::vec3 ReturnPerpendicularVector(glm::vec3 inputVector);
	bool IsVelocityZero();
	void UpdatePhysics();

private:
	glm::vec3 velocity;
	glm::vec3 playerPos;
};

