#include "Lamp.h"



Lamp::Lamp()
{
	//srand(time(NULL));
}

void Lamp::Render(Shader * shader)
{
	Draw(shader);

	if (currentYPos - meanPosition.y > 0.5f)
	{
		dirAmplitude = -0.01f;
	}

	if (currentYPos - meanPosition.y < -0.5f)
	{
		dirAmplitude = 0.01f;
	}

	currentYPos = currentYPos + dirAmplitude;
	this->SetPosition(glm::vec3(this->GetPosition().x, currentYPos, this->GetPosition().z));
}

void Lamp::SetInitialMeanPosition()
{
	meanPosition = this->GetPosition();
	currentYPos = meanPosition.y;
	int val = static_cast<int>(glfwGetTime());;
	//float randValue = rand() % 10 + 1;
	if(val%2==0)
		dirAmplitude = 0.1f;
	else
		dirAmplitude = -0.1f;
}


Lamp::~Lamp()
{
}
