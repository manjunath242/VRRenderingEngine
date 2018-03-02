#include "NormalEnemy.h"


float NormalEnemy::DistanceFromPlayer(glm::vec3 playerPos)
{
	float x = playerPos.x - this->GetPosition().x;
	float y = playerPos.y - this->GetPosition().y;
	float z = playerPos.z - this->GetPosition().z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

void NormalEnemy::ProcessAI(glm::vec3 playerPos)
{
	if (type == 1)
	{
		positionOfPlayer = playerPos;
		if (DistanceFromPlayer(playerPos) < 200.0f && DistanceFromPlayer(playerPos) > 50.0f && (currentState == Patrol || currentState == Attack))
		{
			currentState = Chase;
		}

		else if (DistanceFromPlayer(playerPos) < 50.0f && currentState == Chase)
		{
			currentState = Attack;
		}

		else if (DistanceFromPlayer(playerPos) > 200.0f)
		{
			currentState = Patrol;
		}

		switch (currentState)
		{
		case Attack:
			Attacking();
			break;
		case Chase:
			startPosition = GetPosition();
			endPosition = positionOfPlayer;
			ChasingBezier();
			break;
		case Patrol:
			Patrolling();
			break;
		}
	}
}

NormalEnemy::NormalEnemy()
{
	velocityOfEnemy = glm::vec3(0, 0, 0);
}


NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Patrolling()
{
	SetVelocity(glm::vec3(0, 0, 0));
}

void NormalEnemy::Chasing()
{
	glm::vec3 speedDirection = glm::normalize(positionOfPlayer - this->GetPosition());
	float speed = 2.0f;
	speedDirection = glm::vec3(speedDirection * speed);
	SetVelocity(speedDirection);
}

void NormalEnemy::Attacking()
{
	SetVelocity(glm::vec3(0, 0, 0));
	currentBulletFired = glfwGetTime();
	float shootInterval = currentBulletFired - previousBulletFired;
	if (shootInterval > 0.5f)
	{
		Shoot();
		previousBulletFired = currentBulletFired;
	}
	
}

void NormalEnemy::AddSphereCollider(float radius, glm::vec3 position)
{
	sphereCollider = new SphereCollider();
	sphereCollider->InitParameters(radius, position);
}

void NormalEnemy::UpdateCollider(float deltaTime)
{
	previous.pos = this->GetPosition();
	Integrate(deltaTime);
	double alpha = 1.0f;
	State state;
	state.pos = previous.pos*(float)(1 - alpha) + current.pos*(float)alpha;
	sphereCollider->SetPosition(state.pos);
	this->SetPosition(sphereCollider->GetPosition());
	DestroyBullets();
}

void NormalEnemy::SetVelocity(glm::vec3 velocity)
{
	velocityOfEnemy = velocity;
}

SphereCollider * NormalEnemy::GetCollider()
{
	return sphereCollider;
}

void NormalEnemy::SetHealth(int h)
{
	health = h;
}

int NormalEnemy::GetHealth()
{
	return health;
}

void NormalEnemy::ReduceHealth(int h)
{
	health = health - h;
}

void NormalEnemy::Shoot()
{
	Bullet *enemy = new Bullet();
	enemy->InitPath("Models/Bullet/Bullet.obj");
	glm::vec3 shootPosition = glm::vec3(this->GetPosition());
	enemy->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	enemy->AddSphereCollider(2.0f, enemy->GetPosition());
	glm::vec3 bulletDirection = glm::normalize(positionOfPlayer - shootPosition);
	float bulletSpeed = 4.0f;
	bulletDirection = glm::vec3(bulletDirection * bulletSpeed);
	enemy->SetValues(shootPosition, bulletDirection);
	enemyBullets.push_back(enemy);
}

void NormalEnemy::DestroyBullets()
{
	for (int i = 0; i < enemyBullets.size();)
	{
		if (enemyBullets[i]->GetPosition().x > 100 || enemyBullets[i]->GetPosition().x < -100 ||
			enemyBullets[i]->GetPosition().y > 100 || enemyBullets[i]->GetPosition().y < -100 ||
			enemyBullets[i]->GetPosition().z > 100 || enemyBullets[i]->GetPosition().z < -100)
			{
				Bullet *bullet = enemyBullets[i];
				enemyBullets.erase(enemyBullets.begin() + i);
				delete bullet;
			}

		else
		{
			i++;
		}
	}
}

void NormalEnemy::ChasingBezier()
{
	cout << "Bezier " << GetPosition().x<<" "<<GetPosition().y<<" "<<GetPosition().z<<endl;
	float x = center.x + radius * cos(t);
	float y = center.y + radius * sin(t);
	float currentTime = glfwGetTime();
	float timeToTravel = 5.0f;
	glm::vec3 bending = glm::vec3(0, 1, 0);
	//while (glfwGetTime() < currentTime + timeToTravel)
	{
		glm::vec3 currentPos = Lerp(startPosition, endPosition, (float)(glfwGetTime() - currentTime) / timeToTravel);
		currentPos.x += bending.x * sin(Clamp(((glfwGetTime() - currentTime)/timeToTravel) * 3.14f,0,1));
		currentPos.y += bending.y * sin(Clamp(((glfwGetTime() - currentTime) / timeToTravel) * 3.14f, 0, 1));
		currentPos.z += bending.z * sin(Clamp(((glfwGetTime() - currentTime) / timeToTravel) * 3.14f, 0, 1));
		SetValues(currentPos,glm::vec3(0,0,0));
	}
}

void NormalEnemy::Integrate(double dt)
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
	velocityOfEnemy = velocityOfEnemy + dVeldt*(float)dt;
}

NormalEnemy :: Derivative NormalEnemy::Evaluate(State & initial, double t, double dt, Derivative & d)
{
	State state;
	double pArray1[3] = { initial.pos.x,initial.pos.y,initial.pos.z };
	double pArray2[3] = { d.dPos.x * dt,d.dPos.y * dt,d.dPos.z *dt };

	state.pos.x = pArray1[0]; state.pos.y = pArray1[1]; state.pos.z = pArray1[2];
	double pArray3[3] = { velocityOfEnemy.x,velocityOfEnemy.y,velocityOfEnemy.z };
	double pArray4[3] = { d.dVel.x * dt,d.dVel.y * dt,d.dVel.z *dt };
	velocityOfEnemy.x = pArray3[0]; velocityOfEnemy.y = pArray3[1]; velocityOfEnemy.z = pArray3[2];


	Derivative output;
	output.dPos = velocityOfEnemy;//state.vel;
	output.dVel = Acceleration(state, dt);
	return output;
}

NormalEnemy :: Derivative NormalEnemy::Evaluate(State & initial, double t)
{
	Derivative output;
	output.dPos = velocityOfEnemy;
	return output;
}

void NormalEnemy::SimdAddition(double * pArray1, double * pArray2)
{
	__m128d X = _mm_loadu_pd(pArray1);
	__m128d Y = _mm_loadu_pd(pArray2);
	__m128d result = _mm_add_pd(Y, X);
	_mm_storeu_pd(pArray1, result);
}

glm::vec3 NormalEnemy::Acceleration(State & state, double t)
{
	const double k = 0.01;
	const double b = 0;
	glm::vec3 gravity;
	gravity.x = gravity.z = 0;
	gravity.y = 0.00f;
	state.vel = velocityOfEnemy;
	return gravity;
}

void NormalEnemy::SetValues(glm::vec3 pos, glm::vec3 vel)
{
	previous.pos = pos;
	previous.vel = vel;
	velocityOfEnemy = vel;
	current = previous;
}

void NormalEnemy::SetType(int t)
{
	type = t;
}

glm::vec3 NormalEnemy::Lerp(glm::vec3 a, glm::vec3 b, float t)
{
	return glm::vec3(a * (1 - t))  + glm::vec3(t *b);
}

int NormalEnemy::GetType()
{
	return type;
}

double NormalEnemy::Clamp(double a, int b, int c)
{ 
	a = a < b ? b : a;
	a = a > c ? c : a;
	return a;
}
