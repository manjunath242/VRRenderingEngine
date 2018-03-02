#include "EnvironmentObject.h"



EnvironmentObject::EnvironmentObject()
{
}

float EnvironmentObject::DistanceFromPlayer()
{
	float x = playerPos.x - this->GetPosition().x;
	float y = playerPos.y - this->GetPosition().y;
	float z = playerPos.z - this->GetPosition().z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

glm::vec3 EnvironmentObject::ReturnPerpendicularVector(glm::vec3 inputVector)
{
	glm::vec3 perpendicularVector;
	srand(time(NULL));
	int choice = rand() % 8 + 1;

	if (choice == 1)
	{
		float x = inputVector.x;
		float y = inputVector.y;
		float z = inputVector.z;
		float k = (-(x + z)) / y;
		perpendicularVector = glm::vec3(1, k, 1);
	}

	else if (choice == 2)
	{
		float x = 0;
		float y = inputVector.y;
		float z = inputVector.z;
		float k = (-z) / y;
		perpendicularVector = glm::vec3(0, k, 1);
	}

	else if (choice == 4)
	{
		float x = 0;
		float y = inputVector.y;
		float z = inputVector.z;
		float k = (-y) / z;
		perpendicularVector = glm::vec3(0, 1, k);
	}

	else if (choice == 5)
	{
		float x = inputVector.x;
		float y = inputVector.y;
		float z = 0;
		float k = (-x) / y;
		perpendicularVector = glm::vec3(1, k, 0);
	}

	else if (choice == 6)
	{
		float x = inputVector.x;
		float y = inputVector.y;
		float z = 0;
		float k = (-y) / x;
		perpendicularVector = glm::vec3(k, 1, 0);
	}

	else if (choice == 7)
	{
		float x = inputVector.x;
		float y = 0;
		float z = inputVector.z;
		float k = (-z) / x;
		perpendicularVector = glm::vec3(k, 0, 1);
	}

	else 
	{
		float x = inputVector.x;
		float y = 0;
		float z = inputVector.z;
		float k = (-x) / z;
		perpendicularVector = glm::vec3(1, 0, k);
	}

	glm::normalize(perpendicularVector);
	perpendicularVector = perpendicularVector * 0.2f;
	return perpendicularVector;
}

bool EnvironmentObject::IsVelocityZero()
{
	if (velocity.x == 0 && velocity.y == 0 && velocity.z == 0)
		return true;
	else
		return false;
}

void EnvironmentObject::UpdatePhysics()
{
}


EnvironmentObject::~EnvironmentObject()
{
}

void EnvironmentObject::SetVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
}

void EnvironmentObject::Render(Shader * shader)
{
	this->SetPosition(this->GetPosition() + velocity);
	this->playerPos = Camera::instance()->GetPosition();

	if (DistanceFromPlayer() < 500.0f && IsVelocityZero()==true)
	{
		glm::vec3 diffVector = this->GetPosition() - playerPos;
		diffVector = glm::normalize(diffVector);
		SetVelocity(ReturnPerpendicularVector(diffVector));
	}

	else if (DistanceFromPlayer() > 1500.0f)
	{
		SetVelocity(glm::vec3(0, 0, 0));
	}

	Draw(shader);
}

