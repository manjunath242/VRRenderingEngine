#include "ActorFactory.h"



ActorFactory::ActorFactory()
{
}

void ActorFactory::InitActor(GLchar * path, int typeActor, int typeModel)
{
	typeOfActor = typeActor;
	typeOfModel = typeModel;

	if (typeActor == 0 && typeModel==0)
	{
		ourModel = new EnvironmentObject();
		ourModel->InitPath(path);
		nonPhysicsObjects.push_back(ourModel);
	}

	if (typeActor == 0 && typeModel == 1)
	{
		ourModel = new NormalEnemy();
		ourModel->InitPath(path);
	}
}

void ActorFactory::UpdateActor(Shader *shader)
{
	//ourModel->Draw(shader);
	for (int i = 0; i < nonPhysicsObjects.size(); i++)
	{
		nonPhysicsObjects[i]->Draw(shader);
	}
}

void ActorFactory::SetPosition(glm::vec3 pos)
{
	if (typeOfActor == 0)
	{
		ourModel->SetPosition(pos);
	}
}

glm::vec3 ActorFactory::GetPosition()
{
	if (typeOfActor == 0)
	{
		return ourModel->GetPosition();
	}

	return glm::vec3(0, 0, 0);
}

void ActorFactory::SetScale(glm::vec3 sc)
{
	if (typeOfActor == 0)
	{
		ourModel->SetScale(sc);
	}
}


glm::vec3 ActorFactory::GetScale()
{
	if (typeOfActor == 0)
	{
		return ourModel->GetScale();
	}

	return glm::vec3(1, 1, 1);
}

void ActorFactory::AddAI()
{
	typeOfModel = 1;
}

void ActorFactory::ProcessAI(glm::vec3 playerPos)
{
	if (this->typeOfModel == 1)
	{
		dynamic_cast<NormalEnemy*>(ourModel)->ProcessAI(playerPos);
	}
}

ActorFactory::~ActorFactory()
{
}
