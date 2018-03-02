#pragma once
#include <ppl.h>
//#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include<GL\glew.h>
#include<glm\glm.hpp>
#include"Shader.h"
#include"Texture2D.h"
#include<vector>
#include"Camera.h"

struct Particle {
	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec4 Color;
	/*GLfloat Rotation;
	GLfloat AngularVelocity;*/
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};
class ParticleSystem
{
public:
	ParticleSystem(Shader shader, Texture2D texture, GLuint amount);
	~ParticleSystem();
	void Update(GLfloat dt, GLuint newParticles);
	void Draw();
	void DrawVR();
	void    SetMatrix(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
	void	SetModelMatrix();
	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	GLfloat age = 1.0f;
	GLfloat scale = 1.0f;
	glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat startVelocityMin = 0;
	GLfloat startVelocityRange = 100;
	//additional stuff
	glm::vec2 angularVelocityRange = glm::vec2(0, 0);
	glm::vec3 acceleration = glm::vec3(0, 0, 0);
	glm::vec2 particleSize = glm::vec2(100, 100);
	glm::vec2 viewportDimensions = glm::vec2(800, 600);
	glm::vec2 rotationalRange = glm::vec2(0, 0);
	void ActivateParticles(bool Ap);


private:
	std::vector<Particle> particles;
	GLuint amount;
	// Render state
	Shader shader;
	Texture2D texture;
	GLuint Width = 800, Height = 600;
	GLuint VAO;
	// Initializes buffer and vertex attributes
	void init();
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle();
	void respawnParticle(Particle &particle);
	glm::mat4 model;
	glm::mat4 projection = glm::perspective(Camera::instance()->Zoom, static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 100.0f);
	//additional stuff
	/*static const int MAX_PARTICLES = 8000;
	Particle m_particle[MAX_PARTICLES];
	GLuint m_vertexBuffer;*/

	bool emittingParticles;

};
#endif



