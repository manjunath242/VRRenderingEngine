#pragma once

#include "Model.h"
#include <time.h> 
#include <GLFW/glfw3.h>


class Satellite : public Model
{
public:
	Satellite(int c);
	~Satellite();
	void Render(Shader *shader);
	void UpdatePhysics();
	float GetRadius();
	
private:
	float theta;
	int config;
	float radius;
};

