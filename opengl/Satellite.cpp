#include "Satellite.h"



Satellite::Satellite(int c)
{
	theta = 0;
	config = c;
	radius = 200.0f;
}


Satellite::~Satellite()
{
}

void Satellite::Render(Shader * shader)
{
	Draw(shader);
}

void Satellite::UpdatePhysics()
{
	switch (config)
	{
	case 1:
		this->SetPosition(glm::vec3(radius * sin(theta), radius * cos(theta), this->GetPosition().z));
		break;
	case 2:
		this->SetPosition(glm::vec3(radius * sin(theta), this->GetPosition().y, radius * cos(theta)));
		break;
	case 3:
		this->SetPosition(glm::vec3(this->GetPosition().x, radius * sin(theta), radius * cos(theta)));
		break;
	case 4:
		this->SetPosition(glm::vec3(radius * sin(theta), radius * cos(theta), radius * cos(theta)));
		break;
	case 5:
		this->SetPosition(glm::vec3(radius * sin(theta), radius * cos(theta), radius * sin(theta)));
		break;
	case 6:
		this->SetPosition(glm::vec3(radius * cos(theta), radius * cos(theta), radius * sin(theta)));
		break;

	}

	theta = theta + 0.001f;
}

float Satellite::GetRadius()
{
	return radius;
}
