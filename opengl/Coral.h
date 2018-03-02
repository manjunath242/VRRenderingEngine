#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <al.h>
#include <alc.h>
#include "Model.h"
#include "ResourceManager.h"

class Coral : public Model
{
public:
	Coral();
	~Coral();

	float DistanceFromPlayer(glm::vec3 playerPos);

	void IsParticleActivated();

	bool ActivateParticles();

	bool GetLerpColorStatus();

	void SetLerpColorStatus(bool status);

	void InitializeSound();

	void UpdatePhysics();

	void Render(Shader *shader);

private:
	bool emitParticles;
	bool lerpColor;
	glm::vec3 playerPos;

	ALuint source;
	ALuint buffer;
	void *load(char *fname, long *bufsize);
	float mixValue = 0.0f;
};

