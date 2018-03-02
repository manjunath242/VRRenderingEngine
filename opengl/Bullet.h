#pragma once
#include "Model.h"
#include "SphereCollider.h"
class Bullet:public Model
{
public:
	Bullet();
	~Bullet();
	void AddSphereCollider(float radius, glm::vec3 position);
	void UpdateCollider(float deltaTime);
	void SetVelocity(glm::vec3 velocity);
	SphereCollider* GetCollider();

	struct State
	{
		glm::vec3 pos;
		glm::vec3 vel;
	}previous,current;

	void Integrate(double dt);

	struct Derivative
	{
		glm::vec3 dPos;
		glm::vec3 dVel;
	};

	Derivative Evaluate(State &initial, double t, double dt, Derivative &d);
	Derivative Evaluate(State &initial, double t);

	void SimdAddition(double *pArray1, double *pArray2);

	glm::vec3 acceleration(State &state, double t);

	void SetValues(glm::vec3 pos, glm::vec3 vel);

private:
	SphereCollider *sphereCollider;
	glm::vec3 velocityOfBullet;
	
};

