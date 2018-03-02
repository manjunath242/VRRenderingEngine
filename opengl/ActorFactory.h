#pragma once
#include "Model.h"
#include "NormalEnemy.h"
#include "EnvironmentObject.h"
#include <vector>
class ActorFactory
{
public:
	ActorFactory();
	//Inititalize the actor, if typeActor=0, then model, 1 = sound. typeModel= 0 denotes a non-movable Object, 1 denotes an enemy, 2 denotes a random moving object
	void InitActor(GLchar* path, int typeActor,int typeModel);

	//Update the Actor per frame if necesssary
	void UpdateActor(Shader *shader);

	//Set position of Actor
	void SetPosition(glm::vec3 pos);

	//Get Position of Actor
	glm::vec3 GetPosition();

	//Set scale of Actor
	void SetScale(glm::vec3 sc);

	//Get scale of Actor
	glm::vec3 GetScale();

	//Add AI to an Actor
	void AddAI();

	//Process AI of an Actor
	void ProcessAI(glm::vec3);
	~ActorFactory();

private:
	unsigned int actorID;
	int typeOfActor;
	int typeOfModel;
	glm::vec3 position;
	glm::vec3 scale;
	Model *ourModel;
	vector<Model*> nonPhysicsObjects;
};

