#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Model.h"
#include "SphereCollider.h"
#include "Bullet.h"

class NormalEnemy :public Model
{
public:

	float DistanceFromPlayer(glm::vec3 playerPos);
	void ProcessAI(glm::vec3 playerPos);
	NormalEnemy();
	~NormalEnemy();
	enum FSMState { Patrol, Chase, Attack };
	void Patrolling();
	void Chasing();
	void Attacking();
	void AddSphereCollider(float radius,glm::vec3 position);
	void UpdateCollider(float deltaTime);
	void SetVelocity(glm::vec3 velocity);
	SphereCollider* GetCollider();
	void SetHealth(int h);
	int GetHealth();
	void ReduceHealth(int h);
	void Shoot();
	vector<Bullet*> enemyBullets;
	void DestroyBullets();
	void ChasingBezier();

	struct State
	{
		glm::vec3 pos;
		glm::vec3 vel;
	}previous, current;

	void Integrate(double dt);

	struct Derivative
	{
		glm::vec3 dPos;
		glm::vec3 dVel;
	};

	Derivative Evaluate(State &initial, double t, double dt, Derivative &d);
	Derivative Evaluate(State &initial, double t);

	void SimdAddition(double *pArray1, double *pArray2);

	glm::vec3 Acceleration(State &state, double t);

	void SetValues(glm::vec3 pos, glm::vec3 vel);
	void SetType(int t);
	glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float t);
	int GetType();
	double Clamp(double a, int b, int c);

private:
	FSMState currentState = Patrol;
	FSMState previousState = Patrol;
	SphereCollider *sphereCollider;
	glm::vec3 velocityOfEnemy;
	glm::vec3 positionOfPlayer;
	int health;
	float currentBulletFired = 0;
	float previousBulletFired = 0;
	int type;
	float t = 0.0f;
	float radius = 0.0f;
	glm::vec3 center;
	glm::vec3 startPosition;
	glm::vec3 endPosition;
};

