#include "Flockers.h"



glm::vec3 Flockers::ComputeAlignment(BGMovingObjects * obj)
{
	glm::vec3 point;
	concurrency::parallel_for(size_t(0), objs.size(), [&](size_t i)
	{
		if (objs[i] != obj)
		{
			{
				point += objs[i]->GetVelocity();
			}
		}
	});


	point.x /= objs.size();	point.y /= objs.size();	point.z /= objs.size();

	point = glm::vec3((point.x - obj->GetVelocity().x) / 8.0f,
		(point.y - obj->GetVelocity().y) / 8.0f,
		(point.z - obj->GetVelocity().z) / 8.0f);

	return point;
}

glm::vec3 Flockers::ComputeCohesion(BGMovingObjects * obj, glm::vec3 centreOfFlock)
{
	glm::vec3 centre = centreOfFlock;

	concurrency::parallel_for(size_t(0), objs.size(), [&](size_t i)
	{
		if (objs[i] != obj)
		{
			{
				centre += objs[i]->GetPosition();

			}
		}
	});

	centre.x /= objs.size();	centre.y /= objs.size();	centre.z /= objs.size();
	centre = glm::vec3((centre.x - obj->GetPosition().x) / 100,
		(centre.y - obj->GetPosition().y) / 100,
		(centre.z - obj->GetPosition().z) / 100);

	return centre;
}

glm::vec3 Flockers::ComputeSeperation(BGMovingObjects * obj)
{
	glm::vec3 point;

	concurrency::parallel_for(size_t(0), objs.size(), [&](size_t i)
	{
		if (objs[i] != obj)
		{
			if (obj->DistanceFrom(objs[i]->GetPosition()) < 100)
			{
				point = point - (objs[i]->GetPosition() - obj->GetPosition());
			}
		}
	});

	return point;
}

glm::vec3 Flockers::LimitFlockVelocity(glm::vec3 speed, float s)
{
	float vLim = s;
	glm::vec3 v = speed;

	if (GetDeterminant(speed) > vLim)
	{
		v /= GetDeterminant(v);
		v *= vLim;
	}

	return v;
}

void Flockers::ChangeCentreOfFlock(glm::vec3 centre)
{
	
}

void Flockers::ChangeCentreOfFlockLinearPositiveX()
{
	//centreOfFlock.x += 0.01f;
}

void Flockers::ChangeCentreOfFlockCircular()
{
	centreOfFlock = glm::vec3(50 * sin(theta), 50 * cos(theta), centreOfFlock.z);
	theta = theta + 0.01f;
	if (theta > 360.0f)
		theta = 0.0f;
}

void * Flockers::load(char * fname, long * bufsize)
{
	FILE* fp = fopen(fname, "rb");
	fseek(fp, 0L, SEEK_END);
	long len = ftell(fp);
	rewind(fp);
	void *buf = malloc(len);
	fread(buf, 1, len, fp);
	fclose(fp);
	*bufsize = len;
	return buf;
}



void Flockers::InitializeFlock(float flockSize, glm::vec3 centre, float delay,  bool s, float sDelay, bool patternMovement, int number)
{
	for (int i = 0; i < flockSize; i++)
	{
		BGMovingObjects *temp = new BGMovingObjects();
		temp->InitPath("Models/BlueObject/BlueObject.obj");
		temp->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
		temp->SetPosition(centre);
		temp->SetVelocity(glm::vec3(0.0f, 0.1f, -0.1f));
		alGenSources(1, &soundSources[i]);
		alSourcef(soundSources[i], AL_PITCH, 1.);
		alSourcef(soundSources[i], AL_GAIN, 5);
		alSource3f(soundSources[i], AL_POSITION, temp->GetPosition().x, temp->GetPosition().y, temp->GetPosition().z);
		alSource3f(soundSources[i], AL_VELOCITY, 0., 0., 0.);
		alSourcei(soundSources[i], AL_LOOPING, AL_TRUE);

		alGenBuffers(1, &buffer[i]);
		{
			long dataSize;
			const ALvoid* data = load("footsteps.raw", &dataSize);
			/* for simplicity, assume raw file is signed-16b at 44.1kHz */
			alBufferData(buffer[i], AL_FORMAT_MONO16, data, dataSize, 44100);
			free((void*)data);
		}

		alSourcei(soundSources[i], AL_BUFFER, buffer[i]);
		//alSourcePlay(soundSources[i]);

		fflush(stderr); /* in case OpenAL reported an error earlier */


		objs.push_back(temp);		
	}

	centreOfFlock = centre;
	seperator = 1;
	this->updateDelay = delay;
	seperatorDelay = sDelay;
	seperatorTime = 0.0f;
	flockTime = 0.0f;
	patternTrue = patternMovement;
	patternNumber = number;
	theta = 0.0f;
	seperate = s;

}

Flockers::Flockers()
{
}


Flockers::~Flockers()
{
}

void Flockers::Render(Shader * shader)
{
	playerPos = Camera::instance()->GetPosition();
	for (int i = 0; i < objs.size(); i++)
	{
		//if (Frustum::instance()->CheckSphere(objs[i]->GetPosition(), 5))
			objs[i]->Render(shader);

		//alSource3f(soundSources[i], AL_POSITION, objs[i]->GetPosition().x, objs[i]->GetPosition().y, objs[i]->GetPosition().z);
	}

}

void Flockers::setSeperationDelay(float sDelay)
{
	seperatorDelay = sDelay;
}

void Flockers::setSeperate(bool s)
{
	seperate = s;
}

void Flockers::SetPatternMovement(bool p)
{
	patternTrue = p;
}

void Flockers::SetPatternNumber(int p)
{
	patternNumber = p;
}

void Flockers::SetSeperatorOne()
{
	seperator = 1.0f;
}

glm::vec3 Flockers::ReturnPerpendicularVector(glm::vec3 inputVector)
{
	glm::vec3 perpendicularVector;
	srand(++x);
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

	perpendicularVector /= GetDeterminant(perpendicularVector);
	perpendicularVector = perpendicularVector * 0.5f;
	return perpendicularVector;
}

bool Flockers::IsVelocityZero(glm::vec3 velocity)
{
	if (velocity.x == 0 && velocity.y == 0 && velocity.z == 0)
		return true;
	else
		return false;
}

float Flockers::DistanceFromPlayer()
{
	float x = playerPos.x - centreOfFlock.x;
	float y = playerPos.y - centreOfFlock.y;
	float z = playerPos.z - centreOfFlock.z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

glm::vec3 Flockers::ReturnCentreOfFlock()
{
	return centreOfFlock;
}

glm::vec3 Flockers::ReturnScaleOfElementOfFlock()
{
	return objs[0]->GetScale();
}

void Flockers::UpdatePhysics()
{
	currentTime = glfwGetTime();

	if (currentTime - flockTime > updateDelay)
	{
		UpdateVelocity();
		flockTime = currentTime;
	}

	if (currentTime - seperatorTime > seperatorDelay && seperate == true)
	{
		seperator *= -1;
		seperatorTime = currentTime;
	}

	if (patternTrue)
	{
		switch (patternNumber)
		{
		case 1:
			ChangeCentreOfFlockCircular();
			break;
		case 2:
			ChangeCentreOfFlockLinearPositiveX();
			break;
		}
	}
}

void Flockers::UpdateVelocity()
{
	if (DistanceFromPlayer() < 5000.0f)
	{
		for (int i = 0; i < objs.size(); i++)
		{
			glm::vec3 diffVector = centreOfFlock - playerPos;
			diffVector = glm::normalize(diffVector);
			objs[i] ->SetVelocity(ReturnPerpendicularVector(diffVector));
			centreOfFlock = objs[objs.size() / 2]->GetPosition();
		}
	}

	else
	{
		for (int i = 0; i < objs.size(); i++)
		{

			glm::vec3 temp = objs[i]->GetVelocity() +
				ComputeAlignment(objs[i]) +
				MultiplyVector(ComputeCohesion(objs[i], centreOfFlock), seperator) +
				ComputeSeperation(objs[i]);

			temp = LimitFlockVelocity(temp, 0.1f);
			objs[i]->SetVelocity(temp);
		}
	}
}


