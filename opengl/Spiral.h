#pragma once
#include "Model.h"
class Spiral : public Model
{
public:
	Spiral();
	~Spiral();
	void SetVelocity(glm::vec3 velocity);
	void Update(Shader *shader);

private:
	glm::vec3 velocity;
};

