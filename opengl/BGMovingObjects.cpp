#include "BGMovingObjects.h"



BGMovingObjects::BGMovingObjects()
{
	currentTime = 0.0f;
	initialPosition = this->GetPosition();
	
}


BGMovingObjects::~BGMovingObjects()
{
	
}

void BGMovingObjects::Render(Shader * shader)
{
	this->SetPosition(this->GetPosition() + velocity);
	Draw(shader);
}

void BGMovingObjects::SetVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
}

void BGMovingObjects::ResetPosition()
{
	this->SetPosition(initialPosition);
}

void BGMovingObjects::ChangeVelocity(float ct)
{
	//srand(time(NULL));
	float a =    ((rand() % 10 ) / 1000.0f); // this->velocity.x;
	float b =   ((rand() % 10 ) / 2000.0f);     //   this->velocity.y;   
	float c =    this->velocity.z; //((rand() % 5 - 10) / 100.0f);   
	this->velocity = glm::vec3(a, b, c);

	//this->SetPosition ( glm::vec3( 30 * sin(x) , this->velocity.y,  30 * cos(x)));
	
}

float BGMovingObjects::DistanceFrom(glm::vec3 k)
{
	//float k = glm::distance(k, this->GetPosition());
	float x = k.x - this->GetPosition().x;
	float y = k.y - this->GetPosition().y;
	float z = k.z - this->GetPosition().z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

glm::vec3 BGMovingObjects::GetVelocity()
{
	return velocity;
}

float BGMovingObjects::GetDeterminant(glm::vec3 k)
{
	float val = (k.x * k.x) + (k.y * k.y) + (k.z * k.z);
	val = pow(val, 0.5f);
	return val;
}

glm::vec3 BGMovingObjects::MultiplyVector(glm::vec3 a, float k)
{
	return glm::vec3(a.x*k, a.y*k, a.z*k);
}
