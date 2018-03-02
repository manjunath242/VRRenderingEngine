#include "Game.h"
#include "BackgroundMusic.h"


Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteRenderbuffers(1, &rbo);
}

void Game::Init()
{
	//create all rendering objects in the scene

	Pos2 = Vector3f(Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y, Camera::instance()->GetPosition().z);

	flock1 = new Flockers();
	centreOfFlock1 = glm::vec3(-22, 38, -108);
	flock1->InitializeFlock(27, centreOfFlock1, 1.0f, false, 0.0f, true, 2);
	modelObjects.push_back(flock1);

	flock2 = new Flockers();
	centreOfFlock2 = glm::vec3(150, -100, 50);
	flock2->InitializeFlock(27, centreOfFlock2, 1.0f, false, 0.0f, true, 2);
	modelObjects.push_back(flock2);

	flock3 = new Flockers();
	centreOfFlock3 = glm::vec3(350, 0, 250);
	flock3->InitializeFlock(27, centreOfFlock3, 1.0f, false, 0.0f, true, 2);
	modelObjects.push_back(flock3);

	planet = new Model();
	planet->InitPath("Models/Planet/planet.obj");
	planet->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	planet->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	modelObjects.push_back(planet);

	lamp = new Model();
	lamp->InitPath("Models/Lamp/lamp.obj");
	lamp->SetPosition(glm::vec3(Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y, (Camera::instance()->GetPosition().z - 10)));
	lamp->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	modelObjects.push_back(lamp);

	maintree= new Model();
	maintree->InitPath("Models/Tree1/tree1.obj");
	maintree->InitPath("Models/Base/base.obj");
	maintree->SetPosition(glm::vec3(-22, 32, -108));
	maintree->SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
	modelObjects.push_back(maintree);

	tree1 = new Model();
	sphere = new Model();
	srand(1);

	dirSpiral1 = glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5);
	dirSpiral2 = glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5);
	alpha = 0.0f;

	dirSpiral1 = glm::normalize(dirSpiral1);
	dirSpiral2 = glm::normalize(dirSpiral2);


	for (int i = 1; i < 4; i++)
	{
		Satellite *s = new Satellite(i);
		s->InitPath("Models/GreenObject/GreenObject.obj");
		s->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		s->SetPosition(glm::vec3(0, 0, 0));
		modelObjects.push_back(s);
	}

	//- objects which do not move

	vector<glm::vec3> coralPositions;
	//coral positions
	glm::vec3 coralPosition1 = glm::vec3(-21.25,-18.75,135.5); coralPositions.push_back(coralPosition1);
	glm::vec3 coralPosition2 = glm::vec3(-27,-16.25,120.5); coralPositions.push_back(coralPosition2); 
	glm::vec3 coralPosition3 = glm::vec3(-27,-11.25,111.25); coralPositions.push_back(coralPosition3);
	glm::vec3 coralPosition4 = glm::vec3(-17.75,-11,111.25); coralPositions.push_back(coralPosition4);
	glm::vec3 coralPosition5 = glm::vec3(-17.25,-10.75,98.75); coralPositions.push_back(coralPosition5); 
	glm::vec3 coralPosition6 = glm::vec3(-20.75,-8.25,89.25); coralPositions.push_back(coralPosition6);
	glm::vec3 coralPosition7 = glm::vec3(-35.5,-4.5,89.25); coralPositions.push_back(coralPosition7);
	glm::vec3 coralPosition8 = glm::vec3(-40.5,-15.75,123.25); coralPositions.push_back(coralPosition8);
	glm::vec3 coralPosition9 = glm::vec3(-28,-17.25,136.5); coralPositions.push_back(coralPosition9);
	glm::vec3 coralPosition10 = glm::vec3(-35.25,-16.25,145.25); coralPositions.push_back(coralPosition10);

	//surface emitter positions
	vector<glm::vec3> surfaceEmitterPositions;
	glm::vec3 surfaceEmitterPosition1 = glm::vec3(-21,13.25,61); surfaceEmitterPositions.push_back(surfaceEmitterPosition1);
	glm::vec3 surfaceEmitterPosition2 = glm::vec3(-21,24.75,60.75); surfaceEmitterPositions.push_back(surfaceEmitterPosition2);
	glm::vec3 surfaceEmitterPosition3 = glm::vec3(-32.5,9.75,60.75); surfaceEmitterPositions.push_back(surfaceEmitterPosition3);
	glm::vec3 surfaceEmitterPosition4 = glm::vec3(-40.25,13.5,61); surfaceEmitterPositions.push_back(surfaceEmitterPosition4);
	glm::vec3 surfaceEmitterPosition5 = glm::vec3(-3.75,13.5,61); surfaceEmitterPositions.push_back(surfaceEmitterPosition5);


	// Load Default Shader
	ResourceManager::LoadShader("Shaders/vertexShader_default.vs", "Shaders/fragmentShader_default.frag", nullptr, "default");

	for (int i = 0; i < 4; i++)
	{
		Model *obj = new Model();
		obj->InitPath("Models/Tree1/tree2.obj");
		obj->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		trees.push_back(obj);
	}

	trees.at(0)->SetPosition(glm::vec3(-22, 30, -100));
	trees.at(1)->SetPosition(glm::vec3(-22, 30, -108));
	trees.at(2)->SetPosition(glm::vec3(-15, 30, -108));
	trees.at(3)->SetPosition(glm::vec3(-30, 30, -108));


	for (int i = 0; i < 8; i++)
	{
		Model *obj = new Model();
		obj->InitPath("Models/Tree1/tree3.obj");
		obj->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		trees.push_back(obj);
	}

	trees.at(4)->SetPosition((glm::vec3(-2,84,100)));
	trees.at(4)->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));

	trees.at(5)->SetPosition((glm::vec3(-22, 118, 62)));
	trees.at(6)->SetPosition((glm::vec3(-2, 134, 36)));
	trees.at(7)->SetPosition((glm::vec3(10, 143, -10)));
	trees.at(8)->SetPosition((glm::vec3(24, 136, -32)));
	trees.at(9)->SetPosition((glm::vec3(60, 102, -96)));
	trees.at(10)->SetPosition((glm::vec3(60, 126, -44)));

	for (int i = 0; i < 4; i++)
	{
		Model *obj = new Model();
		obj->InitPath("Models/Tree1/tree4.obj");
		obj->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		trees.push_back(obj);
	}

	trees.at(12)->SetPosition((glm::vec3(0, 128, 74)));
	trees.at(13)->SetPosition((glm::vec3(16, 143, 8)));
	trees.at(14)->SetPosition((glm::vec3(36, 138, 0)));
	trees.at(15)->SetPosition((glm::vec3(64, 88, -88)));

	for (int i = 0; i < 30; i++)
	{
		int xpos = rand() % 20;
		int ypos = rand() % 20;
		int zpos = rand() % 200;
		Lamp *obj = new Lamp();
		obj->InitPath("Models/Lamp/lamp.obj");
		obj->SetPosition(glm::vec3((lamp->GetPosition().x + xpos), (lamp->GetPosition().y + ypos), (lamp->GetPosition().z - zpos)));
		obj->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		obj->SetInitialMeanPosition();
		lampContainers.push_back(obj);
		modelObjects.push_back(obj);
	}



	
	ResourceManager::LoadShader("Shaders/vertexShader_Coral.vs", "Shaders/fragmentShader_Coral.frag", nullptr, "coralShader");
	
	Coral *masterCoral = new Coral();
	masterCoral->InitPath("Models/Corals/coral0.obj");
	masterCoral->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));


	for (int i = 0;i < coralPositions.size();i++)
	{
		Coral * coral = new Coral();
		*coral = *masterCoral;
		coral->SetPosition(coralPositions[i]);
		coral->InitializeSound();
		corals.push_back(coral);
	
	}

	//sample coral
	coral1 = new Coral();
	coral1->InitPath("Models/Corals/coral0.obj");
	coral1->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	
	for (int i = 0; i < trees.size(); i++)
	{
		modelObjects.push_back(trees[i]);
	}



	// Setup Directional Light
	glm::vec3 directionDir = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 ambientDir = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 diffuseDir = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 specularDir = glm::vec3(0.5f, 0.5f, 0.5f);
	directionalLight = new Lights();
	directionalLight->initializeDirectionalLightParameters(directionDir, ambientDir, diffuseDir, specularDir);

	// Initialize Point Light (Initialize as many point lights as requires and push the lights into the pointLights vector)
	glm::vec3 pointLightPosition1 = glm::vec3(1.0, 150.5, 23.75); pointLightPositions.push_back(pointLightPosition1);
	glm::vec3 pointLightPosition2 = glm::vec3(1.0, 148.0, 44.0); pointLightPositions.push_back(pointLightPosition2);
	glm::vec3 pointLightPosition3 = glm::vec3(-9.5, 148.75, 33.75); pointLightPositions.push_back(pointLightPosition3);
	glm::vec3 pointLightPosition4 = glm::vec3(12.25, 148.75, 33.75); pointLightPositions.push_back(pointLightPosition4);
	glm::vec3 pointLightPosition5 = glm::vec3(-9.0, 143.5, 54.25); pointLightPositions.push_back(pointLightPosition5);
	glm::vec3 pointLightPosition6 = glm::vec3(13.0, 140.25, 54.25); pointLightPositions.push_back(pointLightPosition6);
	glm::vec3 pointLightPosition7 = glm::vec3(0.75, 150.0, 4.25); pointLightPositions.push_back(pointLightPosition7);
	glm::vec3 pointLightPosition8 = glm::vec3(-10.25, 151.5, 14.0); pointLightPositions.push_back(pointLightPosition8);
	glm::vec3 pointLightPosition9 = glm::vec3(12.0, 150.0, 14.0); pointLightPositions.push_back(pointLightPosition9);
	glm::vec3 pointLightPosition10 = glm::vec3(12.0, 147.75, -8.0); pointLightPositions.push_back(pointLightPosition10);
	glm::vec3 pointLightPosition11 = glm::vec3(-11.25, 148.25, -8.0); pointLightPositions.push_back(pointLightPosition11);
	glm::vec3 pointLightPosition12 = glm::vec3(0.2, 149.0, -18.0); pointLightPositions.push_back(pointLightPosition12);
	glm::vec3 pointLightPosition13 = glm::vec3(-11.8, 147.75, -28.75); pointLightPositions.push_back(pointLightPosition13);
	glm::vec3 pointLightPosition14 = glm::vec3(11.2, 147.75, -28.75); pointLightPositions.push_back(pointLightPosition14);
	glm::vec3 pointLightPosition15 = glm::vec3(33.2, 143.5, -28.75); pointLightPositions.push_back(pointLightPosition15);
	glm::vec3 pointLightPosition16 = glm::vec3(33.2, 147.5, -7.75); pointLightPositions.push_back(pointLightPosition16);
	glm::vec3 pointLightPosition17 = glm::vec3(33.2, 149.5, 13.75); pointLightPositions.push_back(pointLightPosition17);
	glm::vec3 pointLightPosition18 = glm::vec3(33.2, 149.0, 33.25); pointLightPositions.push_back(pointLightPosition18);
	glm::vec3 pointLightPosition19 = glm::vec3(33.2, 141.75, 52.75); pointLightPositions.push_back(pointLightPosition19);
	glm::vec3 pointLightPosition20 = glm::vec3(55.95, 137.0, 54.5); pointLightPositions.push_back(pointLightPosition20);
	glm::vec3 pointLightPosition21 = glm::vec3(55.95, 143.75, 34.0); pointLightPositions.push_back(pointLightPosition21);
	glm::vec3 pointLightPosition22 = glm::vec3(55.95, 147.5, 14.25); pointLightPositions.push_back(pointLightPosition22);
	glm::vec3 pointLightPosition23 = glm::vec3(55.95, 148.75, -8.0); pointLightPositions.push_back(pointLightPosition23);
	glm::vec3 pointLightPosition24 = glm::vec3(55.95, 143.5, -28.75); pointLightPositions.push_back(pointLightPosition24);
	glm::vec3 pointLightPosition25 = glm::vec3(79.20, 130.5, -28.75); pointLightPositions.push_back(pointLightPosition25);
	glm::vec3 pointLightPosition26 = glm::vec3(79.20, 137.5, -5.25); pointLightPositions.push_back(pointLightPosition26);
	glm::vec3 pointLightPosition27 = glm::vec3(79.20, 135.0, 17.25); pointLightPositions.push_back(pointLightPosition27);
	glm::vec3 pointLightPosition28 = glm::vec3(79.20, 134.25, 34.25); pointLightPositions.push_back(pointLightPosition28);
	glm::vec3 pointLightPosition29 = glm::vec3(79.30, 131.25, 54.75); pointLightPositions.push_back(pointLightPosition29);
	glm::vec3 pointLightPosition30 = glm::vec3(104.30, 112.0, 56.25); pointLightPositions.push_back(pointLightPosition30);
	glm::vec3 pointLightPosition31 = glm::vec3(102.30, 116.75, 35.0); pointLightPositions.push_back(pointLightPosition31);
	glm::vec3 pointLightPosition32 = glm::vec3(102.05, 121.0, 16.25); pointLightPositions.push_back(pointLightPosition32);
	glm::vec3 pointLightPosition33 = glm::vec3(102.05, 119.0, -7.75); pointLightPositions.push_back(pointLightPosition33);
	glm::vec3 pointLightPosition34 = glm::vec3(102.05, 116.25, -28.75); pointLightPositions.push_back(pointLightPosition34);
	glm::vec3 pointLightPosition35 = glm::vec3(122.55, 96.75, -35.25); pointLightPositions.push_back(pointLightPosition35);
	glm::vec3 pointLightPosition36 = glm::vec3(122.55, 99.25, -10.0); pointLightPositions.push_back(pointLightPosition36);
	glm::vec3 pointLightPosition37 = glm::vec3(122.55, 99.25, 14.75); pointLightPositions.push_back(pointLightPosition37);
	glm::vec3 pointLightPosition38 = glm::vec3(122.55, 98.25, 36.25); pointLightPositions.push_back(pointLightPosition38);
	glm::vec3 pointLightPosition39 = glm::vec3(122.55, 83.25, 59.75); pointLightPositions.push_back(pointLightPosition39);
	glm::vec3 pointLightPosition40 = glm::vec3(129.30, 63.75, 59.75); pointLightPositions.push_back(pointLightPosition40);
	glm::vec3 pointLightPosition41 = glm::vec3(133.80, 76.25, 36.25); pointLightPositions.push_back(pointLightPosition41);
	glm::vec3 pointLightPosition42 = glm::vec3(137.80, 76.25, 16.5); pointLightPositions.push_back(pointLightPosition42);
	glm::vec3 pointLightPosition43 = glm::vec3(135.80, 76.25, -9.5); pointLightPositions.push_back(pointLightPosition43);
	glm::vec3 pointLightPosition44 = glm::vec3(135.80, 76.25, -35.0); pointLightPositions.push_back(pointLightPosition44);
	glm::vec3 pointLightPosition45 = glm::vec3(142.3, 52.5, -35.0); pointLightPositions.push_back(pointLightPosition45);
	glm::vec3 pointLightPosition46 = glm::vec3(144.3, 52.5, -10.0); pointLightPositions.push_back(pointLightPosition46);
	glm::vec3 pointLightPosition47 = glm::vec3(144.8, 52.5, 15.5); pointLightPositions.push_back(pointLightPosition47);
	glm::vec3 pointLightPosition48 = glm::vec3(144.55, 52.5, 36.25); pointLightPositions.push_back(pointLightPosition48);
	glm::vec3 pointLightPosition49 = glm::vec3(144.55, 52.5, 60.5); pointLightPositions.push_back(pointLightPosition49);
	glm::vec3 pointLightPosition50 = glm::vec3(149.05, 25.5, 60.5); pointLightPositions.push_back(pointLightPosition50);
	glm::vec3 pointLightPosition51 = glm::vec3(151.8, 25.5, 35.0); pointLightPositions.push_back(pointLightPosition51);
	glm::vec3 pointLightPosition52 = glm::vec3(153.3, 25.5, 16.0); pointLightPositions.push_back(pointLightPosition52);
	glm::vec3 pointLightPosition53 = glm::vec3(157.55, 25.5, -8.5); pointLightPositions.push_back(pointLightPosition53);
	glm::vec3 pointLightPosition54 = glm::vec3(153.05, 25.5, -33.5); pointLightPositions.push_back(pointLightPosition54);
	glm::vec3 pointLightPosition55 = glm::vec3(136.8, 25.5, 84.75); pointLightPositions.push_back(pointLightPosition55);
	glm::vec3 pointLightPosition56 = glm::vec3(126.55, 47.25, 84.75); pointLightPositions.push_back(pointLightPosition56);
	glm::vec3 pointLightPosition57 = glm::vec3(116.55, 66.0, 84.75); pointLightPositions.push_back(pointLightPosition57);
	glm::vec3 pointLightPosition58 = glm::vec3(98.55, 93.0, 78.25); pointLightPositions.push_back(pointLightPosition58);
	glm::vec3 pointLightPosition59 = glm::vec3(-30.95, 146.5, 51.25); pointLightPositions.push_back(pointLightPosition59);
	glm::vec3 pointLightPosition60 = glm::vec3(-30.95, 149.75, 32.5); pointLightPositions.push_back(pointLightPosition60);
	glm::vec3 pointLightPosition61 = glm::vec3(-30.95, 149.75, 15.0); pointLightPositions.push_back(pointLightPosition61);
	glm::vec3 pointLightPosition62 = glm::vec3(-30.95, 151.75, -5); pointLightPositions.push_back(pointLightPosition62);
	glm::vec3 pointLightPosition63 = glm::vec3(-30.95, 146.0, -28.75); pointLightPositions.push_back(pointLightPosition63);
	glm::vec3 pointLightPosition64 = glm::vec3(-50.95, 143.5, 51.25); pointLightPositions.push_back(pointLightPosition64);
	glm::vec3 pointLightPosition65 = glm::vec3(-50.95, 146.75, 32.5); pointLightPositions.push_back(pointLightPosition65);
	glm::vec3 pointLightPosition66 = glm::vec3(-50.95, 146.75, 15.0); pointLightPositions.push_back(pointLightPosition66);
	glm::vec3 pointLightPosition67 = glm::vec3(-50.95, 148.75, -5); pointLightPositions.push_back(pointLightPosition67);
	glm::vec3 pointLightPosition68 = glm::vec3(-50.95, 143.0, -28.75); pointLightPositions.push_back(pointLightPosition68);
	glm::vec3 pointLightPosition69 = glm::vec3(-70.95, 136.5, 51.25); pointLightPositions.push_back(pointLightPosition69);
	glm::vec3 pointLightPosition70 = glm::vec3(-70.95, 139.75, 32.5); pointLightPositions.push_back(pointLightPosition70);
	glm::vec3 pointLightPosition71 = glm::vec3(-70.95, 139.75, 15.0); pointLightPositions.push_back(pointLightPosition71);
	glm::vec3 pointLightPosition72 = glm::vec3(-70.95, 141.75, -5); pointLightPositions.push_back(pointLightPosition72);
	glm::vec3 pointLightPosition73 = glm::vec3(-70.95, 136.0, -28.75); pointLightPositions.push_back(pointLightPosition73);
	glm::vec3 pointLightPosition74 = glm::vec3(-95.95, 125.5, 51.25); pointLightPositions.push_back(pointLightPosition74);
	glm::vec3 pointLightPosition75 = glm::vec3(-95.95, 128.75, 32.5); pointLightPositions.push_back(pointLightPosition75);
	glm::vec3 pointLightPosition76 = glm::vec3(-95.95, 128.75, 15.0); pointLightPositions.push_back(pointLightPosition76);
	glm::vec3 pointLightPosition77 = glm::vec3(-95.95, 130.75, -5); pointLightPositions.push_back(pointLightPosition77);
	glm::vec3 pointLightPosition78 = glm::vec3(-95.95, 125.0, -28.75); pointLightPositions.push_back(pointLightPosition78);
	glm::vec3 pointLightPosition79 = glm::vec3(-120.95, 110.5, 51.25); pointLightPositions.push_back(pointLightPosition79);
	glm::vec3 pointLightPosition80 = glm::vec3(-120.95, 113.75, 32.5); pointLightPositions.push_back(pointLightPosition80);
	glm::vec3 pointLightPosition81 = glm::vec3(-120.95, 113.75, 15.0); pointLightPositions.push_back(pointLightPosition81);
	glm::vec3 pointLightPosition82 = glm::vec3(-120.95, 115.75, -5); pointLightPositions.push_back(pointLightPosition82);
	glm::vec3 pointLightPosition83 = glm::vec3(-120.95, 110.0, -28.75); pointLightPositions.push_back(pointLightPosition83);

	//for cave1
	glm::vec3 pointLightPosition84 = glm::vec3(-7.95, -13.0, 118.75); pointLightPositions.push_back(pointLightPosition84);
	glm::vec3 pointLightPosition85 = glm::vec3(-41.95, -13.0, 118.75); pointLightPositions.push_back(pointLightPosition85);
	glm::vec3 pointLightPosition86 = glm::vec3(-41.95, -13.0, 90.75); pointLightPositions.push_back(pointLightPosition86);
	glm::vec3 pointLightPosition87 = glm::vec3(-13.95, -13.0, 90.75); pointLightPositions.push_back(pointLightPosition87);
	glm::vec3 pointLightPosition88 = glm::vec3(-23.95, -15.0, 114.75); pointLightPositions.push_back(pointLightPosition88);
	//   vec3(98.550003, 93.000000, 78.250000)

	//path to cave2
	glm::vec3 pointLightPosition89 = glm::vec3(-13.95, 111.0, 98.75); pointLightPositions.push_back(pointLightPosition89);
	glm::vec3 pointLightPosition90 = glm::vec3(-17.95, 125.0, 76.75); pointLightPositions.push_back(pointLightPosition90);
	glm::vec3 pointLightPosition91 = glm::vec3(1.95, 129.0, 68.75); pointLightPositions.push_back(pointLightPosition91);
	glm::vec3 pointLightPosition92 = glm::vec3(53.95, 119.0, -64.75); pointLightPositions.push_back(pointLightPosition92);
	glm::vec3 pointLightPosition93 = glm::vec3(63.95, 113.0, -62.75); pointLightPositions.push_back(pointLightPosition93);
	glm::vec3 pointLightPosition94 = glm::vec3(25.95, 117.0, -62.75); pointLightPositions.push_back(pointLightPosition94);
	glm::vec3 pointLightPosition95 = glm::vec3(55.95, 111.0, -82.75); pointLightPositions.push_back(pointLightPosition95);
	glm::vec3 pointLightPosition96 = glm::vec3(43.95, 87.0, -98.75); pointLightPositions.push_back(pointLightPosition96);
	glm::vec3 pointLightPosition97 = glm::vec3(83.95, 93.0, -88.75); pointLightPositions.push_back(pointLightPosition97);
	glm::vec3 pointLightPosition98 = glm::vec3(21.95, 53.0, -134.75); pointLightPositions.push_back(pointLightPosition98);
	glm::vec3 pointLightPosition99 = glm::vec3(-7.95, 55.0, -150.75); pointLightPositions.push_back(pointLightPosition99);
	glm::vec3 pointLightPosition100 = glm::vec3(-39.95, 49.0, -146.75); pointLightPositions.push_back(pointLightPosition100);
	glm::vec3 pointLightPosition101 = glm::vec3(-75.95, 31.0, -26.75); pointLightPositions.push_back(pointLightPosition101);
	glm::vec3 pointLightPosition102 = glm::vec3(-95.95, 27.0, -110.75); pointLightPositions.push_back(pointLightPosition102);
	glm::vec3 pointLightPosition103 = glm::vec3(-29.95, 27.0, -110.75); pointLightPositions.push_back(pointLightPosition103);

	//ontop of big tree
	glm::vec3 pointLightPosition104 = glm::vec3(-13.95, 101.0, 136.75); pointLightPositions.push_back(pointLightPosition104);
	glm::vec3 pointLightPosition105 = glm::vec3(-7.95, 79.0, 138.75); pointLightPositions.push_back(pointLightPosition105);
	glm::vec3 pointLightPosition106 = glm::vec3(-7.95, 27.0, -134.75); pointLightPositions.push_back(pointLightPosition106);
	glm::vec3 pointLightPosition107 = glm::vec3(-7.95, 27.0, -114.75); pointLightPositions.push_back(pointLightPosition107);
	glm::vec3 pointLightPosition108 = glm::vec3(-7.95, 37.0, -98.75); pointLightPositions.push_back(pointLightPosition108);
	glm::vec3 pointLightPosition109 = glm::vec3(-45.95, 37.0, -100.75); pointLightPositions.push_back(pointLightPosition109);
	glm::vec3 pointLightPosition110 = glm::vec3(-65.95, 25.0, -128.75); pointLightPositions.push_back(pointLightPosition110);

	for (int i = 0; i < lampContainers.size(); i++)
	{
		pointLightPositions.push_back(lampContainers.at(i)->GetPosition());
	}


	// Setup Point Light. Properties of Point Light can be changed over time if required. (In Game loop change the values if required)
	// Get the Point Light whose values need to be changed using the vector pointLights and change the properties as required
	glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < pointLightPositions.size(); i++)
	{
		Lights *pointLight = new Lights();
		int lightDistance;
		// Note that the 4th parameter is the distance the point light should affect (set the distance from the pre initialized pointLightDistance array)
		if (i == 103)
		{
			ambient = glm::vec3(3.0f, 3.0f, 3.0f);
			diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			specular = glm::vec3(1.0f, 1.0f, 1.0f);
			lightDistance = pointLightDistance[6];
		}
		else {

			ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
			specular = glm::vec3(0.0f, 0.0f, 0.0f);
			lightDistance = pointLightDistance[4];
		}

		uniformNames.clear();
		uniformNames.push_back("pointLights[" + to_string(i) + "].position");
		uniformNames.push_back("pointLights[" + to_string(i) + "].ambient");
		uniformNames.push_back("pointLights[" + to_string(i) + "].diffuse");
		uniformNames.push_back("pointLights[" + to_string(i) + "].specular");
		uniformNames.push_back("pointLights[" + to_string(i) + "].constant");
		uniformNames.push_back("pointLights[" + to_string(i) + "].linear");
		uniformNames.push_back("pointLights[" + to_string(i) + "].quadratic");

		pointLight->setPointLightParameters(pointLightPositions[i], ambient, diffuse, specular, lightDistance);
		pointLight->setPointLightUniforms(uniformNames, &ResourceManager::GetShader("default"));
		pointLights.push_back(pointLight);

	}



	// Point Light Container Shader
	ResourceManager::LoadShader("Shaders/vertexShader_LightContainer.vs", "Shaders/fragmentShader_LightContainer.frag", nullptr, "lightContainerShader");

	//load skybox shader
	ResourceManager::LoadShader("Shaders/vertexShader_Skysphere.vs", "Shaders/fragmentShader_Skysphere.frag", nullptr, "skySphere");
	sphere->InitPath("Models/Sphere/perfect_sphere.obj");
	sphere->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	sphere->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	//load particle system
	ResourceManager::LoadShader("Shaders/particle.vs", "Shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadTexture("Textures/whitesnowflake.png", GL_TRUE, "particle");
	ResourceManager::LoadTexture("Textures/star.png", GL_TRUE, "coralParticle");
	particlesystem1 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 5000);
	particlesystem1->age = 10.0f;
	particlesystem1->initialPosition = glm::vec3(0.5, 0.5, 370.0);
	particlesystem1->acceleration = glm::vec3(0.0, 0.0, 0.5);
	particlesystem1->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	particlesystem1->startVelocityMin = 1.f;
	particlesystem1->startVelocityRange = 1.5f;
	particlesystem1->scale = 0.1f;

	//coral particle system
	ParticleSystem *masterCoralParticle = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("coralParticle"), 1000);
	masterCoralParticle->age = 4.0f;
	masterCoralParticle->acceleration = glm::vec3(0.0, 1.5, 0.0);
	masterCoralParticle->color = glm::vec4(0.1f, 0.8f, 0.4f, 1.0f);
	masterCoralParticle->startVelocityMin = 0.1f;
	masterCoralParticle->startVelocityRange = 0.1f;
	masterCoralParticle->scale = 0.1f;

	for (int i = 0;i < coralPositions.size();i++)
	{
		ParticleSystem *coralParticle = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
		*coralParticle = *masterCoralParticle;
		coralParticle->initialPosition = coralPositions[i];
		coralParticles.push_back(coralParticle);
	}

	//surface emitter setup
	ParticleSystem* masterSurfaceEmitter = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 5000);
	masterSurfaceEmitter->age = 15.0f;
	masterSurfaceEmitter->acceleration = glm::vec3(0.0, 0.0, 1.5);
	masterSurfaceEmitter->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	masterSurfaceEmitter->startVelocityMin = 1.1f;
	masterSurfaceEmitter->startVelocityRange = 1.5f;
	masterSurfaceEmitter->scale = 0.8f;

	for (int i = 0;i < surfaceEmitterPositions.size();i++)
	{
		ParticleSystem *surfaceParticle = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
		*surfaceParticle = *masterSurfaceEmitter;
		surfaceParticle->initialPosition = surfaceEmitterPositions[i];
		surfaceEmitter.push_back(surfaceParticle);
	}

	for (int i = 0; i < corals.size(); i++)
	{
		modelObjects.push_back(corals[i]);
	}

	if (!isVR)
	{
		// Initialize framebuffer and setup the screenQuad
		setupScreenQuadAndFrameBuffer();
	}
}

void Game::Update(GLfloat dt)
{
	// background music update
	BackgroundMusic::Switch(Camera::instance()->GetPosition().z <= 200);

}


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		// Camera controls
		if (this->Keys[GLFW_KEY_W])
			Camera::instance()->ProcessKeyboard(FORWARD, dt);
		if (this->Keys[GLFW_KEY_S])
			Camera::instance()->ProcessKeyboard(BACKWARD, dt);
		if (this->Keys[GLFW_KEY_A])
			Camera::instance()->ProcessKeyboard(LEFT, dt);
		if (this->Keys[GLFW_KEY_D])
			Camera::instance()->ProcessKeyboard(RIGHT, dt);

		if (Camera::instance()->GetPosition().z > 61 && Camera::instance()->GetPosition().z < 200 &&
			Camera::instance()->GetPosition().x > -100 && Camera::instance()->GetPosition().x < 10 &&
			Camera::instance()->GetPosition().y > -25 && Camera::instance()->GetPosition().y < 35)
		{
			Camera::instance()->MovePositiveZ(dt);
			Camera::instance()->SetSpeed(8.0f);
		}

		else
		{
			Camera::instance()->SetSpeed(10.0f);
		}


		float movespeed = 2.0f;

		if (this->Keys[GLFW_KEY_I])
		{
			pointLightPositions[pointLightPositions.size()-1].y += movespeed;

		}
		if (this->Keys[GLFW_KEY_K])
		{
			pointLightPositions[pointLightPositions.size() - 1].y -= movespeed;
		}
		if (this->Keys[GLFW_KEY_J])
		{
			pointLightPositions[pointLightPositions.size() - 1].z += movespeed;
		}
		if (this->Keys[GLFW_KEY_L])
		{
			pointLightPositions[pointLightPositions.size() - 1].z -= movespeed;
			coral1Position.z -= .25;

		}
		if (this->Keys[GLFW_KEY_U])
		{
			pointLightPositions[pointLightPositions.size() - 1].x -= movespeed;
		}
		if (this->Keys[GLFW_KEY_O])
		{
			pointLightPositions[pointLightPositions.size() - 1].x += movespeed;
		}
		if (this->Keys[GLFW_KEY_P])
		{
			cout << "Position : " + glm::to_string(pointLightPositions[pointLightPositions.size() - 1]);
		}
	}
}


void Game::Render(GLfloat dt)
{
	if (isVR)
	{
		 for (int i = 0;i < 4;i++)
		 {
			 for (int j = 0;j < 4;j++)
			 {
				 camView[i][j] = viewVR.M[j][i];
			 }
		 }
		 for (int i = 0;i < 4;i++)
		 {
			 for (int j = 0;j < 4;j++)
			 {
				 camProjection[i][j] = projVR.M[j][i];
			 }
		 }

		 Camera::instance()->camView = camView;
		 Camera::instance()->camProjection = camProjection;

	}
	
	else
	{
		// Bind to framebuffer and draw to color texture 
		// as we normally would.

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// Clear all attached buffers        
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer so why bother with clearing?
		glEnable(GL_DEPTH_TEST);
		
		camView = Camera::instance()->GetViewMatrix();
		camProjection = glm::perspective(Camera::instance()->Zoom, static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 5000.0f);
	}

	Frustum::instance()->ConstructFrustum(5, camProjection, camView);

	shader = ResourceManager::GetShader("default");
	shader.Use();
	shader.SetMatrix4("view", camView);
	shader.SetMatrix4("projection", camProjection);

	ResourceManager::GetShader("default").SetFloat("FogDensity", fogDensity);
	
	// Setup Directional Light
	directionalLight->setUpDirectionalLight(&ResourceManager::GetShader("default"), Camera::instance());

	// Properties of Point Light can be changed over time if required here.
	// Get the Point Lights whose values need to be changed using the vector pointLights and change the properties as required
	ResourceManager::GetShader("default").SetInteger("NO_OF_POINT_LIGHTS", pointLights.size());
	glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < pointLightPositions.size(); i++)
	{
		if (i == 103)
		{
			ambient = pointLights[i]->getPointLightAmbient();
			diffuse = pointLights[i]->getPointLightDiffuse();
			specular = pointLights[i]->getPointLightSpecular();

			pointLights[i]->setPointLightParameters(pointLightPositions[i], ambient, diffuse, specular, pointLightDistance[6]);
		}
		else
		{
			pointLights[i]->EmitLight(glm::vec3(Pos2.x,Pos2.y,Pos2.z));
			if (pointLights[i]->getEmitLightValue() == true)
			{

				ambient = pointLights[i]->getPointLightAmbient();
				diffuse = pointLights[i]->getPointLightDiffuse();
				specular = pointLights[i]->getPointLightSpecular();
				if (ambient.x < 1.0f)
					ambient = ambient + 0.1f;
				else
					ambient = glm::vec3(1.0f, 1.0f, 1.0f);

				if (diffuse.x < 0.8f)
					diffuse = diffuse + 0.1f;
				else
					diffuse = glm::vec3(0.8f, 0.8f, 0.8f);

				if (specular.x < 1.0f)
					specular = specular + 0.1f;
				else
					specular = glm::vec3(1.0f, 1.0f, 1.0f);

				pointLights[i]->setPointLightParameters(pointLightPositions[i], ambient, diffuse, specular, pointLightDistance[4]);
			}
		}
		
		pointLights[i]->setUpPointLight(&ResourceManager::GetShader("default"), Camera::instance());
	}

	


	RenderThread();

	ChangeDirectionOfCameraRandomly(false);

	glDepthFunc(GL_LEQUAL);
	Shader skySphereShader = ResourceManager::GetShader("skySphere");

	skySphereShader.Use();

	glm::mat4 camViewVRSkysphere = glm::mat4(glm::mat3(camView));
	skySphereShader.SetMatrix4("view", camViewVRSkysphere);
	skySphereShader.SetMatrix4("projection", camProjection);
	skySphereShader.SetFloat("fog", fogDensity);
	GLfloat timeValue = glfwGetTime();
	skySphereShader.SetFloat("iGlobalTime", timeValue);
	if (noOfCoralsActivated == corals.size()) 
	{
		noOfCoralsActivated = 0;
		skySphereShader.SetFloat("isColor", 1.0f);
		fogDensity = 0.0f;
		skySphereShader.SetFloat("fog", fogDensity);
		ResourceManager::GetShader("default").SetFloat("FogDensity", fogDensity);
	}

	else 
	{
		noOfCoralsActivated = 0;
		skySphereShader.SetFloat("isColor", 0.0f);		
	}

	sphere->Draw(&skySphereShader);
	glDepthFunc(GL_LESS);

	//add particles here

	Shader particleShader = ResourceManager::GetShader("particle");
	particleShader.Use();
	//camView = Camera::instance()->GetViewMatrix();
	particleShader.SetMatrix4("view", camView);
	particleShader.SetMatrix4("projection", camProjection);
	glm::mat4 model;
	//model = glm::translate(model, particle.Position);
	model[0][0] = camView[0][0];
	model[0][1] = camView[1][0];
	model[0][2] = camView[2][0];
	model[1][0] = camView[0][1];
	model[1][1] = camView[1][1];
	model[1][2] = camView[2][1];
	model[2][0] = camView[0][2];
	model[2][1] = camView[1][2];
	model[2][2] = camView[2][2];
	particleShader.SetMatrix4("model", model);


	for (int i = 0; i < corals.size(); i++)
	{
		if (corals[i]->GetLerpColorStatus() == true)
			noOfCoralsActivated++;

		coralParticles[i]->ActivateParticles(corals[i]->ActivateParticles());
		coralParticles[i]->Draw();
		coralParticles[i]->Update(dt, 2);
	}
	
	for (int i = 0;i < surfaceEmitter.size(); i++)
	{
		surfaceEmitter[i]->Draw();
		surfaceEmitter[i]->Update(dt, 2);
	}

	
	if (!isVR)
	{
		// Bind to default framebuffer again and draw the 
		// quad plane with attched screen texture.

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

		// Draw Screen
		Shader screenShader = ResourceManager::GetShader("screenShader");
		screenShader.Use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}


void Game::setupScreenQuadAndFrameBuffer()
{
	ResourceManager::LoadShader("Shaders/VertexShader_Quad.vs", "Shaders/FragmentShader_Quad.frag", nullptr, "screenShader");
	GLfloat quadVertices[] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	// Setup screen VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
	// Framebuffers
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->Width, this->Height); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
																								  // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Generates a texture that is suited for attachments to a framebuffer
GLuint Game::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, this->Width, this->Height, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->Width, this->Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

float Game::GetDeterminant(const glm::vec3 k)
{
	float val = (k.x * k.x) + (k.y * k.y) + (k.z * k.z);
	val = pow(val, 0.5f);
	return val;
}

glm::vec3 Game::MultiplyVector(const glm::vec3 a, float k)
{
	return glm::vec3(a.x*k,a.y*k,a.z*k);
}



float Game::DistanceBetweenVectors(glm::vec3 a, glm::vec3 b)
{
	float x = a.x - b.x;
	float y = a.y - b.y;
	float z = a.z - b.z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

void Game::CleanUp()
{
	delete sphere, planet, lamp, maintree, tree1, directionalLight, particlesystem1, particlesystem2, coral1;

	for (int i = 0; i < lampContainers.size(); ++i) {
		delete lampContainers[i]; // Calls ~object and deallocates *tmp[i]
	}
	lampContainers.clear();

	for (int i = 0; i < trees.size(); ++i) {
		delete trees[i]; // Calls ~object and deallocates *tmp[i]
	}
	trees.clear();

	for (int i = 0; i < pointLights.size(); i++)
		delete pointLights[i];
	pointLights.clear();


	for (int i = 0; i < coralParticles.size(); i++)
	{
		delete coralParticles[i];
	}
	coralParticles.clear();

	for (int i = 0; i < surfaceEmitter.size(); i++)
	{
		delete surfaceEmitter[i];
	}
	surfaceEmitter.clear();

	for (int i = 0; i < modelObjects.size(); i++)
	{
		delete modelObjects[i];
	}
	modelObjects.clear();

}

void Game::ChangeDirectionOfCameraRandomly(const bool k)
{
	if (k)
	{
		glm::vec3 dirSpiralLerp = alpha*dirSpiral1 + (1 - alpha)*dirSpiral2;
		dirSpiralLerp = glm::normalize(dirSpiralLerp);
		
		Camera::instance()->Front = dirSpiralLerp;
		alpha = alpha + 0.01f;

		if (alpha >= 0.96f)
		{
			srand(glfwGetTime());
			dirSpiral2 = dirSpiralLerp;
			dirSpiral1 = glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5);
			dirSpiral1 = glm::normalize(dirSpiral1);
			alpha = 0.0f;
		}
	}
}

void Game::RenderThread()
{
	for (int i = 0; i<modelObjects.size(); i++)
	{
		modelObjects[i]->UpdatePhysics();
		modelObjects[i]->Render(&shader);
	}
}


 ovrGraphicsLuid Game::GetDefaultAdapterLuid()
{
	ovrGraphicsLuid luid = ovrGraphicsLuid();

#if defined(_WIN32)
	IDXGIFactory* factory = nullptr;

	if (SUCCEEDED(CreateDXGIFactory(IID_PPV_ARGS(&factory))))
	{
		IDXGIAdapter* adapter = nullptr;

		if (SUCCEEDED(factory->EnumAdapters(0, &adapter)))
		{
			DXGI_ADAPTER_DESC desc;

			adapter->GetDesc(&desc);
			memcpy(&luid, &desc.AdapterLuid, sizeof(luid));
			adapter->Release();
		}

		factory->Release();
	}
#endif

	return luid;
}

int Game::Compare(const ovrGraphicsLuid& lhs, const ovrGraphicsLuid& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(ovrGraphicsLuid));
}

void Game::VRtoMouse(double xoffset, double yoffset)
{
	Camera::instance()->ProcessMouseMovement(xoffset, yoffset, true);
}

// return true to retry later (e.g. after display lost)
bool Game::RenderOculus()
{
	TextureBuffer * eyeRenderTexture[2] = { nullptr, nullptr };
	DepthBuffer   * eyeDepthBuffer[2] = { nullptr, nullptr };
	ovrMirrorTexture mirrorTexture = nullptr;
	GLuint          mirrorFBO = 0;
	long long frameIndex = 0;

	ovrSession session;
	ovrGraphicsLuid luid;
	ovrResult result = ovr_Create(&session, &luid);
	if (!OVR_SUCCESS(result))
		return retryCreate;

	if (Compare(luid, GetDefaultAdapterLuid())) // If luid that the Rift is on is not the default adapter LUID...
	{
		VALIDATE(false, "OpenGL supports only the default graphics adapter.");
	}

	ovrHmdDesc hmdDesc = ovr_GetHmdDesc(session);

	// Setup Window and Graphics

	ovrSizei windowSize = { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2 };

	if (glfwWindowShouldClose(window))
		goto Done;

	// Make eye render buffers
	for (int eye = 0; eye < 2; ++eye)
	{
		ovrSizei idealTextureSize = ovr_GetFovTextureSize(session, ovrEyeType(eye), hmdDesc.DefaultEyeFov[eye], 1);
		eyeRenderTexture[eye] = new TextureBuffer(session, true, true, idealTextureSize, 1, NULL, 1);
		eyeDepthBuffer[eye] = new DepthBuffer(eyeRenderTexture[eye]->GetSize(), 0);

		if (!eyeRenderTexture[eye]->TextureChain)
		{
			if (retryCreate) goto Done;
			VALIDATE(false, "Failed to create texture.");
		}
	}

	ovrMirrorTextureDesc desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width = windowSize.w;
	desc.Height = windowSize.h;
	desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;

	// Create mirror texture and an FBO used to copy mirror texture to back buffer
	result = ovr_CreateMirrorTextureGL(session, &desc, &mirrorTexture);
	if (!OVR_SUCCESS(result))
	{
		if (retryCreate) goto Done;
		VALIDATE(false, "Failed to create mirror texture.");
	}

	// Configure the mirror read buffer
	GLuint texId;
	ovr_GetMirrorTextureBufferGL(session, mirrorTexture, &texId);

	glGenFramebuffers(1, &mirrorFBO);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
	glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	Init();
	ovr_SetTrackingOriginType(session, ovrTrackingOrigin_FloorLevel);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		ovrSessionStatus sessionStatus;
		ovr_GetSessionStatus(session, &sessionStatus);
		if (sessionStatus.ShouldQuit)
		{
			// Because the application is requested to quit, should not request retry
			retryCreate = false;
			break;
		}
		if (sessionStatus.ShouldRecenter)
			ovr_RecenterTrackingOrigin(session);

		if (sessionStatus.IsVisible)
		{
			// Keyboard inputs to adjust player orientation
			static float Yaw(3.141592f);
			if (Platform.Key[VK_LEFT])  Yaw += 2.0f;
			if (Platform.Key[VK_RIGHT]) Yaw -= 2.0f;

			Camera::instance()->SetPosition(glm::vec3(Pos2.x, Pos2.y, Pos2.z));
			alListener3f(AL_POSITION, Pos2.x, Pos2.y, Pos2.z);

			// Call ovr_GetRenderDesc each frame to get the ovrEyeRenderDesc, as the returned values (e.g. HmdToEyeOffset) may change at runtime.
			ovrEyeRenderDesc eyeRenderDesc[2];
			eyeRenderDesc[0] = ovr_GetRenderDesc(session, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
			eyeRenderDesc[1] = ovr_GetRenderDesc(session, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

			// Get eye poses, feeding in correct IPD offset
			ovrPosef                  EyeRenderPose[2];
			ovrVector3f               HmdToEyeOffset[2] = { eyeRenderDesc[0].HmdToEyeOffset,
				eyeRenderDesc[1].HmdToEyeOffset };

			double sensorSampleTime;    // sensorSampleTime is fed into the layer later
			ovr_GetEyePoses(session, frameIndex, ovrTrue, HmdToEyeOffset, EyeRenderPose, &sensorSampleTime);

			// Render Scene to Eye Buffers
			for (int eye = 0; eye < 2; ++eye)
			{
				// Switch to eye render target
				eyeRenderTexture[eye]->SetAndClearRenderSurface(eyeDepthBuffer[eye]);

				// Get view and projection matrices
				Matrix4f rollPitchYaw = Matrix4f::RotationY(Yaw);
				Matrix4f finalRollPitchYaw = rollPitchYaw * Matrix4f(EyeRenderPose[eye].Orientation);
				Vector3f finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
				Vector3f finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
				Vector3f shiftedEyePos = Pos2 + rollPitchYaw.Transform(EyeRenderPose[eye].Position);



				// Keyboard inputs to adjust player position			
				if (this->Keys[GLFW_KEY_W])      Pos2 += finalForward; //Matrix4f::RotationY(Yaw).Transform(Vector3f(0, 0, +2.f));;// Pos2 += Matrix4f::RotationY(Yaw).Transform(EyeRenderPose[eye].Orientation)*2);
				if (this->Keys[GLFW_KEY_S])      Pos2 -= finalForward;
				if (this->Keys[GLFW_KEY_D])      Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(-2.0f, 0, 0));
				if (this->Keys[GLFW_KEY_A])      Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(+2.f, 0, 0));

				

				//manju_note
				 viewVR = Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + finalForward, finalUp);
				 projVR = ovrMatrix4f_Projection(hmdDesc.DefaultEyeFov[eye], 0.2f, 1000.0f, ovrProjection_None);

				calcFPS(1.0, windowTitle);


				// Check and call events
				glfwPollEvents();

				// Manage user input
				ProcessInput(deltaTime);

				// Clear the colorbuffer
				glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//glm::mat4 viewVR; glm::mat4 projectionVR;
				Render(deltaTime);
				eyeRenderTexture[eye]->UnsetRenderSurface();

				// Commit changes to the textures so they get picked up frame
				eyeRenderTexture[eye]->Commit();
			}

			// Do distortion rendering, Present and flush/sync

			ovrLayerEyeFov ld;
			ld.Header.Type = ovrLayerType_EyeFov;
			ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.

			for (int eye = 0; eye < 2; ++eye)
			{
				ld.ColorTexture[eye] = eyeRenderTexture[eye]->TextureChain;
				ld.Viewport[eye] = Recti(eyeRenderTexture[eye]->GetSize());
				ld.Fov[eye] = hmdDesc.DefaultEyeFov[eye];
				ld.RenderPose[eye] = EyeRenderPose[eye];
				ld.SensorSampleTime = sensorSampleTime;
			}

			ovrLayerHeader* layers = &ld.Header;
			result = ovr_SubmitFrame(session, frameIndex, nullptr, &layers, 1);
			// exit the rendering loop if submit returns an error, will retry on ovrError_DisplayLost
			if (!OVR_SUCCESS(result))
				goto Done;

			frameIndex++;
		}

		// Blit mirror texture to back buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLint w = windowSize.w;
		GLint h = windowSize.h;
		glBlitFramebuffer(0, h, w, 0,
			0, 0, w, h,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		glfwSwapBuffers(window);
	}

Done:
	Running = false;
	if (mirrorFBO) glDeleteFramebuffers(1, &mirrorFBO);
	if (mirrorTexture) ovr_DestroyMirrorTexture(session, mirrorTexture);
	for (int eye = 0; eye < 2; ++eye)
	{
		delete eyeRenderTexture[eye];
		delete eyeDepthBuffer[eye];
	}
	//Platform.ReleaseDevice();
	ovr_Destroy(session);

	// Retry on ovrError_DisplayLost
	return retryCreate || (result == ovrError_DisplayLost);
}

void Game::RunVR()
{
	while (!glfwWindowShouldClose(window))
	{
		//alSource3f(source, AL_POSITION, Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y, Camera::instance()->GetPosition().z);
		alListener3f(AL_POSITION, Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y, Camera::instance()->GetPosition().z);

		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();


		//game.Update(deltaTime);
		BackgroundMusic::Update(deltaTime);

		if(retryCreate)
		RenderOculus();

	}
}


double Game::calcFPS(double theTimeInterval, string theWindowTitle)
{
	// Static values which only get initialised the first time the function runs
	static double t0Value = glfwGetTime(); // Set the initial time to now
	static int    fpsFrameCount = 0;             // Set the initial FPS frame count to 0
	static double fps = 0.0;           // Set the initial FPS value to 0.0

									   // Get the current time in seconds since the program started (non-static, so executed every time)
	double currentTime = glfwGetTime();

	// Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s)
	// Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least.
	if (theTimeInterval < 0.1)
	{
		theTimeInterval = 0.1;
	}
	if (theTimeInterval > 10.0)
	{
		theTimeInterval = 10.0;
	}

	// Calculate and display the FPS every specified time interval
	if ((currentTime - t0Value) > theTimeInterval)
	{
		// Calculate the FPS as the number of frames divided by the interval in seconds
		fps = (double)fpsFrameCount / (currentTime - t0Value);

		// If the user specified a window title to append the FPS value to...
		if (theWindowTitle != "NONE")
		{
			// Convert the fps value into a string using an output stringstream
			std::ostringstream stream;
			stream << fps;
			std::string fpsString = stream.str();

			// Append the FPS value to the window title details
			theWindowTitle += " | FPS: " + fpsString;

			// Convert the new window title to a c_str and set it
			const char* pszConstString = theWindowTitle.c_str();
			glfwSetWindowTitle(window, pszConstString);
		}
		else // If the user didn't specify a window to append the FPS to then output the FPS to the console
		{
			std::cout << "FPS: " << fps << std::endl;
		}

		// Reset the FPS frame counter and set the initial time to be now
		fpsFrameCount = 0;
		t0Value = glfwGetTime();
	}
	else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter
	{
		fpsFrameCount++;
	}

	// Return the current FPS - doesn't have to be used if you don't want it!
	return fps;
}