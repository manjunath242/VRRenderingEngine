#pragma once
#include <time.h> 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Model.h"

class Lamp : public Model
{
public:
	Lamp();
	void Render(Shader *shader);
	void SetInitialMeanPosition();
	~Lamp();

private:
	glm::vec3 meanPosition;
	float currentYPos;
	float dirAmplitude;
};

