#pragma once
#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SOIL.h>
#include <ppl.h>


#include "Camera.h"
#include "ActorFactory.h"
#include <stdlib.h>    
#include <time.h>
#include <map>
#include <ppl.h>
#include <tuple>
#include <thread>
#include "Bullet.h"
#include "Coral.h"
#include "ParticleSystem.h"
#include "Skybox.h"
#include "Lights.h"
#include "BGMovingObjects.h"
#include "ResourceManager.h"
#include "Satellite.h"
#include "Flockers.h"
#include "Frustum.h"
#include "EnvironmentObject.h"
#include "Spiral.h"
#include "Lamp.h"

// Include the Oculus SDK
#include "GL/CAPI_GLE.h"
#include "ThirdParty\OculusSDK\LibOVR\Include\Extras\OVR_Math.h"
#include "ThirdParty\OculusSDK\LibOVR\Include\OVR_CAPI_GL.h"

#if defined(_WIN32)
#include <dxgi.h> // for GetDefaultAdapterLuid
#pragma comment(lib, "dxgi.lib")
#endif


using namespace OVR;
using namespace concurrency;

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:

	// Game state
	GameState              State;
	GLboolean              Keys[1024];
	bool isVR = false;
	GLFWwindow* window;
	string windowTitle;


	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render(GLfloat dt);

	// Other methods specific for the game

	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
	void setupScreenQuadAndFrameBuffer();
	ovrGraphicsLuid GetDefaultAdapterLuid();
	int Compare(const ovrGraphicsLuid & lhs, const ovrGraphicsLuid & rhs);
	void VRtoMouse(double xoffset, double yoffset);
	bool RenderOculus();
	void RunVR();
	//void Run(bool(*MainLoop)(bool retryCreate));
	double calcFPS(double theTimeInterval, string theWindowTitle);
	
	float GetDeterminant(const glm::vec3 k);
	glm::vec3 MultiplyVector(const glm::vec3 a, float k);
	float DistanceBetweenVectors(glm::vec3 a,glm::vec3 b);

	//Delete all Pointers
	void CleanUp();

	void ChangeDirectionOfCameraRandomly(const bool k);

	//Render thread
	void RenderThread();
private:

	GLuint                 Width, Height;
	bool Running = true;
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	bool retryCreate = true;
	GLuint quadVAO, quadVBO;
	GLuint rbo;
	GLuint textureColorbuffer;
	GLuint framebuffer;

	Matrix4f viewVR;
	Matrix4f projVR;
	Vector3f Pos2;

	float VRx = 0;
	float VRy = 0;
	float xoffset, yoffset;
	bool VROrientationSet = false;
	float currentBulletFired = 0;
	float previousBulletFired = 0;
	// Point Light Distance array. Do not add distances manually. The pre initialized array is based on the point light properties.
	int pointLightDistance[9] = { 7,13,20,32,50,65,100,160,200 };

	GLuint texture1;
	//vector<Model*> pointLightContainers;
	Model *sphere;
	Model *planet,  *lamp, *maintree,*tree1; //planet,maintree entered
	vector<Lamp*> lampContainers;		//entered in modelObjects
	vector<Model*> trees;				//Entered in modelObjects

	//Skybox skybox;
	Lights *directionalLight;
	vector<Lights*> pointLights;
	vector<glm::vec3> pointLightPositions;

	vector<Lights*> pointguideLights;
	vector<glm::vec3> pointguideLightPositions;

	ParticleSystem*  particlesystem1;
	ParticleSystem*  particlesystem2;
	//coral particlesystem
	Coral* coral1;
	//coral models
	
	vector<Coral* >corals;
	float mixValue;
	int noOfCoralsActivated = 0;
	vector<ParticleSystem*> coralParticles;
	vector<ParticleSystem*> surfaceEmitter;

	glm::vec3 centreOfFlock1;
	glm::vec3 centreOfFlock3;
	GLfloat flock1CurTime;
	GLfloat flock2CurTime;
	glm::vec3 centreOfFlock2;
	GLfloat currentTime;


	Flockers *flock1;
	Flockers *flock2;
	Flockers *flock3;
	

	glm::vec3 coral1Position = glm::vec3(0.5, 0.5, 380);
	glm::mat4 camView;
	glm::mat4 camProjection;

	Shader shader;

	glm::vec3 dirSpiral1;
	glm::vec3 dirSpiral2;

	float alpha;
	vector<Model*> modelObjects;
	float fogDensity = 0.01f;
	vector<string> uniformNames;
};

#endif
