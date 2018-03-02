#include "Spiral.h"



Spiral::Spiral()
{
}


Spiral::~Spiral()
{
}

void Spiral::SetVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
}

void Spiral::Update(Shader * shader)
{
	//this->SetPosition(this->GetPosition() + velocity);
	Draw(shader);
}
