#include "Bullet.h"



Bullet::Bullet()
{
	previous = current;
}


Bullet::~Bullet()
{
}

void Bullet::AddSphereCollider(const float radius, const glm::vec3 position)
{
	sphereCollider = new SphereCollider();
	sphereCollider->InitParameters(radius, position);
}

void Bullet::UpdateCollider(const float deltaTime)
{
	/*glm::vec3 vel = glm::vec3(velocityOfBullet * deltaTime);
	sphereCollider->SetPosition(sphereCollider->GetPosition() + vel);
	this->SetPosition(sphereCollider->GetPosition());*/

	previous.pos = this->GetPosition();
	Integrate(deltaTime);
	double alpha = 1.0f;
	State state;
	state.pos = previous.pos*(float)(1 - alpha) + current.pos*(float)alpha;
	sphereCollider->SetPosition(state.pos);
	this->SetPosition(sphereCollider->GetPosition());
	
}

void Bullet::SetVelocity(const glm::vec3 velocity)
{
	velocityOfBullet = velocity;
}

SphereCollider * Bullet::GetCollider()
{
	return sphereCollider;
}

void Bullet::Integrate(double dt)
{
	previous = current;
	Derivative a, b, c, d;
	int t = 0;
	a = Evaluate(current, t);
	b = Evaluate(current, t, dt*0.5f, a);
	c = Evaluate(current, t, dt*0.5f, b);
	d = Evaluate(current, t, dt, c);

	double temp = 1.0f / 6.0f;

	glm::vec3 dPosdt = glm::vec3((glm::vec3((a.dPos + (b.dPos + c.dPos)) * 2.0f) + d.dPos) * (float)temp);

	glm::vec3 dVeldt = (a.dVel + (b.dVel + c.dVel) * 2.0f + d.dVel)* (float)temp;

	current.pos = current.pos + dPosdt * (float)dt;
	// current.vel = current.vel + dVeldt * dt;
	velocityOfBullet = velocityOfBullet + dVeldt*(float)dt;
}

Bullet :: Derivative Bullet::Evaluate(State & initial, double t, double dt, Derivative & d)
{
	State state;
	double pArray1[3] = { initial.pos.x,initial.pos.y,initial.pos.z };
	double pArray2[3] = { d.dPos.x * dt,d.dPos.y * dt,d.dPos.z *dt };
	//using simd instead of the below addition
	//state.pos = initial.pos + d.dPos*dt;
	// simdAddition(pArray1,pArray2);
	state.pos.x = pArray1[0]; state.pos.y = pArray1[1]; state.pos.z = pArray1[2];
	double pArray3[3] = { velocityOfBullet.x,velocityOfBullet.y,velocityOfBullet.z };
	double pArray4[3] = { d.dVel.x * dt,d.dVel.y * dt,d.dVel.z *dt };
	velocityOfBullet.x = pArray3[0]; velocityOfBullet.y = pArray3[1]; velocityOfBullet.z = pArray3[2];


	Derivative output;
	output.dPos = velocityOfBullet;//state.vel;
	output.dVel = acceleration(state, dt);
	return output;
}

Bullet :: Derivative Bullet::Evaluate(State & initial, double t)
{
	Derivative output;
	output.dPos = velocityOfBullet;
	return output;
}

void Bullet::SimdAddition(double *pArray1, double *pArray2)
{
	__m128d X = _mm_loadu_pd(pArray1);
	__m128d Y = _mm_loadu_pd(pArray2);
	__m128d result = _mm_add_pd(Y, X);
	_mm_storeu_pd(pArray1, result);
}

glm::vec3 Bullet::acceleration(State & state, double t)
{
	const double k = 0.01;
	const double b = 0;
	glm::vec3 gravity;
	gravity.x = gravity.z = 0;
	gravity.y = 0.00f;
	state.vel = velocityOfBullet;
	return gravity;
}

void Bullet::SetValues(glm::vec3 pos, glm::vec3 vel)
{
	previous.pos = pos;
	previous.vel = vel;
	velocityOfBullet = vel;
	current = previous;
}
