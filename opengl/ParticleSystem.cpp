#include "ParticleSystem.h"



ParticleSystem::ParticleSystem(Shader shader, Texture2D texture, GLuint amount)
	:shader(shader), texture(texture), amount(amount)
{
	this->init();

}

void ParticleSystem::ActivateParticles(bool Ap)
{
	emittingParticles = Ap;
}

void ParticleSystem::init()
{
	//glGenBuffers(1, &m_vertexBuffer);
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

GLuint lastUsedParticle = 0;
GLuint ParticleSystem::firstUnusedParticle()
{
	// First search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

void ParticleSystem::respawnParticle(Particle & particle)
{
	glm::vec3 initialVelocity = glm::vec3(rand(), rand(), rand());
	initialVelocity -= glm::vec3(RAND_MAX*0.5f);
	initialVelocity = glm::normalize(initialVelocity);
	GLfloat speed = rand() / (GLfloat)RAND_MAX*startVelocityRange + startVelocityMin;
	initialVelocity = initialVelocity*speed;

	GLfloat initialRotation = (rand() / (GLfloat)RAND_MAX)*rotationalRange.y + rotationalRange.x;
	GLfloat initialAngular = (rand() / (GLfloat)RAND_MAX)*angularVelocityRange.y + angularVelocityRange.x;



	particle.Position = initialPosition;
	particle.Color = color;
	particle.Life = age;
	particle.Velocity = initialVelocity, 0.0f;
	//particle.AngularVelocity = initialAngular;
	//particle.Rotation = initialRotation;





}

void ParticleSystem::DrawVR()
{
	if (emittingParticles)
	{
		glEnable(GL_BLEND);
		// SRC_ALPHA, and ONE give additive blending

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		for (Particle particle : this->particles)
		{

			//this->shader.Use();
			if (particle.Life > 0.0f)
			{

				this->shader.SetInteger("ourTexture", 0);
				this->shader.SetVector3f("position", particle.Position);
				this->shader.SetVector4f("ourColor", color);
				this->shader.SetFloat("scale", scale);

				this->texture.Bind();
				glBindVertexArray(this->VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);

			}
			//this->shader.UnBind();
		}

		glDisable(GL_BLEND);
	}
}

void ParticleSystem::Draw()
{
	if (emittingParticles)
	{
		glEnable(GL_BLEND);
		// SRC_ALPHA, and ONE give additive blending

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//this->shader.Use();
		//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		//glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES*sizeof(Particle), m_particle, GL_STATIC_DRAW);
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)0);
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(GLfloat)*4));
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(GLfloat) * 8));
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(GLfloat) * 12));
		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(GLfloat) * 13));
		//glEnableVertexAttribArray(5);
		//glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(GLfloat) * 14));
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		//this->shader.SetInteger("ourTexture", 0);
		////this->shader.SetVector3f("position",m_particle->Position);
		////this->shader.SetVector4f("ourColor", color);
		//this->shader.SetVector2f("particleSize", particleSize);
		//this->shader.SetVector2f("viewport", viewportDimensions);
		//this->texture.Bind();

		//glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

		for (Particle particle : this->particles)
		{

			//this->shader.Use();
			if (particle.Life > 0.0f)
			{

				this->shader.SetInteger("ourTexture", 0);
				this->shader.SetVector3f("position", particle.Position);
				this->shader.SetVector4f("ourColor", color);
				this->shader.SetFloat("scale", scale);

				this->texture.Bind();
				glBindVertexArray(this->VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);

			}
			//this->shader.UnBind();
		}
		/*for (int i = 0;i < 6;i++)
		{
		glDisableVertexAttribArray(i);
		}*/
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);
	}
}

void ParticleSystem::SetMatrix(const GLchar * name, const glm::mat4 & matrix, GLboolean useShader)
{

	glUniformMatrix4fv(glGetUniformLocation(this->shader.ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ParticleSystem::SetModelMatrix()
{
	glm::mat4 view = Camera::instance()->GetViewMatrix();

	//model = glm::translate(model, this->initialPosition);
	model[0][0] = view[0][0];
	model[0][1] = view[1][0];
	model[0][2] = view[2][0];
	model[1][0] = view[0][1];
	model[1][1] = view[1][1];
	model[1][2] = view[2][1];
	model[2][0] = view[0][2];
	model[2][1] = view[1][2];
	model[2][2] = view[2][2];

	this->shader.SetMatrix4("model", model);
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update(GLfloat dt, GLuint newParticles)
{
	if (emittingParticles)
	{
		for (GLuint i = 0;i < newParticles;++i)
		{
			int unusedParticle = this->firstUnusedParticle();
			this->respawnParticle(this->particles[unusedParticle]);
		}
		concurrency::parallel_for(size_t(0), this->amount, [&](size_t i)
		{
			Particle &p = this->particles[i];
			p.Life -= dt;
			if (p.Life > 0.0f)
			{
				p.Position += p.Velocity*dt;
				p.Velocity += acceleration*dt;
			}
		});
	}
}
