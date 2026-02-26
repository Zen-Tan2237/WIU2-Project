// GLM Headers
#include "GL\glew.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>

#include "SceneRiseTop.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"

#include "KeyboardController.h"
#include "MouseController.h"
#include "CollisionDetection.h"

SceneRiseTop::SceneRiseTop()
{
}

SceneRiseTop::~SceneRiseTop()
{
}

void SceneRiseTop::Init()
{
	BaseScene::Init();

	// LIGHTS
	{
		light[0].position = glm::vec3(0, 5, 0);
		light[0].color = glm::vec3(1, 1, 1);
		light[0].type = Light::DIRECTIONAL;
		light[0].power = 0.f;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = 45.f;
		light[0].cosInner = 30.f;
		light[0].exponent = 3.f;
		light[0].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[1].position = glm::vec3(0, 5, 0);
		light[1].color = glm::vec3(1, 1, 1);
		light[1].type = Light::POINT;
		light[1].power = 2;
		light[1].kC = 1.f;
		light[1].kL = 0.01f;
		light[1].kQ = 0.001f;
		light[1].cosCutoff = 45.f;
		light[1].cosInner = 30.f;
		light[1].exponent = 3.f;
		light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[2].position = glm::vec3(0, 5, 0);
		light[2].color = glm::vec3(1, 1, 1);
		light[2].type = Light::POINT;
		light[2].power = 0;
		light[2].kC = 1.f;
		light[2].kL = 0.01f;
		light[2].kQ = 0.001f;
		light[2].cosCutoff = 45.f;
		light[2].cosInner = 30.f;
		light[2].exponent = 3.f;
		light[2].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[3].position = glm::vec3(0, 5, 0);
		light[3].color = glm::vec3(1, 1, 1);
		light[3].type = Light::POINT;
		light[3].power = 0;
		light[3].kC = 1.f;
		light[3].kL = 0.01f;
		light[3].kQ = 0.001f;
		light[3].cosCutoff = 45.f;
		light[3].cosInner = 30.f;
		light[3].exponent = 3.f;
		light[3].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[4].position = glm::vec3(0, 5, 0);
		light[4].color = glm::vec3(1, 1, 1);
		light[4].type = Light::POINT;
		light[4].power = 0;
		light[4].kC = 1.f;
		light[4].kL = 0.01f;
		light[4].kQ = 0.001f;
		light[4].cosCutoff = 45.f;
		light[4].cosInner = 30.f;
		light[4].exponent = 3.f;
		light[4].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[5].position = glm::vec3(0, 5, 0);
		light[5].color = glm::vec3(1, 1, 1);
		light[5].type = Light::POINT;
		light[5].power = 0;
		light[5].kC = 1.f;
		light[5].kL = 0.01f;
		light[5].kQ = 0.001f;
		light[5].cosCutoff = 45.f;
		light[5].cosInner = 30.f;
		light[5].exponent = 3.f;
		light[5].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[6].position = glm::vec3(0, 5, 0);
		light[6].color = glm::vec3(1, 1, 1);
		light[6].type = Light::POINT;
		light[6].power = 0;
		light[6].kC = 1.f;
		light[6].kL = 0.01f;
		light[6].kQ = 0.001f;
		light[6].cosCutoff = 45.f;
		light[6].cosInner = 30.f;
		light[6].exponent = 3.f;
		light[6].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[7].position = glm::vec3(0, 5, 0);
		light[7].color = glm::vec3(1, 1, 1);
		light[7].type = Light::POINT;
		light[7].power = 0;
		light[7].kC = 1.f;
		light[7].kL = 0.01f;
		light[7].kQ = 0.001f;
		light[7].cosCutoff = 45.f;
		light[7].cosInner = 30.f;
		light[7].exponent = 3.f;
		light[7].spotDirection = glm::vec3(0.f, 1.f, 0.f);
	}
	{
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], cosf(glm::radians<float>(light[0].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], cosf(glm::radians<float>(light[0].cosInner)));
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], cosf(glm::radians<float>(light[1].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], cosf(glm::radians<float>(light[1].cosInner)));
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

		glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
		glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
		glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
		glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
		glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], cosf(glm::radians<float>(light[2].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT2_COSINNER], cosf(glm::radians<float>(light[2].cosInner)));
		glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

		glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
		glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
		glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
		glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
		glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
		glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
		glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], cosf(glm::radians<float>(light[3].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT3_COSINNER], cosf(glm::radians<float>(light[3].cosInner)));
		glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);

		glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
		glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
		glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
		glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
		glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
		glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
		glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], cosf(glm::radians<float>(light[4].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT4_COSINNER], cosf(glm::radians<float>(light[4].cosInner)));
		glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);

		glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
		glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
		glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
		glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
		glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
		glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
		glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], cosf(glm::radians<float>(light[5].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT5_COSINNER], cosf(glm::radians<float>(light[5].cosInner)));
		glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);

		glUniform3fv(m_parameters[U_LIGHT6_COLOR], 1, &light[6].color.r);
		glUniform1i(m_parameters[U_LIGHT6_TYPE], light[6].type);
		glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
		glUniform1f(m_parameters[U_LIGHT6_KC], light[6].kC);
		glUniform1f(m_parameters[U_LIGHT6_KL], light[6].kL);
		glUniform1f(m_parameters[U_LIGHT6_KQ], light[6].kQ);
		glUniform1f(m_parameters[U_LIGHT6_COSCUTOFF], cosf(glm::radians<float>(light[6].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT6_COSINNER], cosf(glm::radians<float>(light[6].cosInner)));
		glUniform1f(m_parameters[U_LIGHT6_EXPONENT], light[6].exponent);

		glUniform3fv(m_parameters[U_LIGHT7_COLOR], 1, &light[7].color.r);
		glUniform1i(m_parameters[U_LIGHT7_TYPE], light[7].type);
		glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
		glUniform1f(m_parameters[U_LIGHT7_KC], light[7].kC);
		glUniform1f(m_parameters[U_LIGHT7_KL], light[7].kL);
		glUniform1f(m_parameters[U_LIGHT7_KQ], light[7].kQ);
		glUniform1f(m_parameters[U_LIGHT7_COSCUTOFF], cosf(glm::radians<float>(light[7].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT7_COSINNER], cosf(glm::radians<float>(light[7].cosInner)));
		glUniform1f(m_parameters[U_LIGHT7_EXPONENT], light[7].exponent);
	}

	// DEBUG
	meshList_riseTop[GEO_WALL] = MeshBuilder::GenerateCube("Wall", glm::vec3(1.f, 0.f, 0.f), 1.f);
	meshList_riseTop[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", glm::vec3(0.f, 1.f, 0.f), 1.f, 36, 18);

	// MODELS
	meshList_riseTop[GEO_STALL] = MeshBuilder::GenerateOBJ("stall", "Models//minigame_Stall.obj");
	meshList_riseTop[GEO_STALL]->textureID = LoadTGA("Textures//minigameStall.tga");

	meshList_riseTop[GEO_PLANK] = MeshBuilder::GenerateOBJ("Plank", "Models//WoodPlank.obj");
	meshList_riseTop[GEO_PLANK]->textureID = LoadTGA("Textures//WoodPlanks.tga");




	// INITIAL ITEM IN HAND
	addPickables("Controller", glm::vec3(0, 0, 0));
	itemInHand = pickables[0];
	amountOfItem = 2;
	previousItemInHandName = "";
	itemInUse = false;


	// INITIAL PICKABLES


	// VARIABLE INITIALIZATIONS
	plankTargetOrientation = 0.f;
	plankOrientation = 0.f;

	gameTimeElapsed = 0.f;
	changeOrientationElapsed = 0.f;
	changeOrientation = 0.f;
	lerpOrientationSpeed = 0.f;
	direction = 1;


	// PHASE DURATIONS
	part = 0;
	phase = 0;

	phaseDurations[0][0] = 3.8f;  // "Woah there!"
	phaseDurations[0][1] = 2.5f;  // "This plank has a mind of its own."
	phaseDurations[0][2] = 2.3f;  // "It won't stay balanced for long."
	phaseDurations[0][3] = 2.2f;  // "Press A to push it left."
	phaseDurations[0][4] = 2.2f;  // "Press D to push it right."
	phaseDurations[0][5] = 2.6f;  // "Counter its movement to stay balanced."
	phaseDurations[0][6] = 2.0f;  // "Keep the plank steady..."
	phaseDurations[0][7] = 2.3f;  // "Or the ball will roll off!"
	phaseDurations[0][8] = 2.4f;  // "Small corrections work best."
	phaseDurations[0][9] = 2.2f;  // "Ready to test your reflexes?"

	phaseDurations[1][0] = 100.f;  // "

	phaseDurations[2][0] = 100.f;  // "

	phaseDurations[3][0] = 3.f;  //
	phaseDurations[3][1] = 1.f;  //
	phaseDurations[3][2] = 1.f;  //
	phaseDurations[3][3] = 1.f;  //
	phaseDurations[3][4] = 1.f;  //


	// GRASS DENSITY
	targetFPS = 60.0f;
	fpsSmoothed = 0.0f;
	grassDensityMultiplier = 0.0f;
	activeGrassCount = 0;

	RegenerateGrassPositions();


	// CAMERA INIT
	camera.Init(glm::vec3(0, 0.9f, 4.f), glm::vec3(0, 0.9f, 5.f), glm::vec3(0, 1.9f, 4.f));


	// WORLD OBJECTS
	bool miscSettings[2] = { false, false }; // for gravity and drag. override in case of specific objects

	// Floor
	worldObjects[0].InitPhysicsObject(glm::vec3(0, -0.5f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(200, 1, 200), 0, glm::vec3(1, 0, 0), miscSettings);

	// Stall
	worldObjects[1].InitPhysicsObject(glm::vec3(6, 0.9f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(1.5f, 1.4f, 1.7f), 180, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[2].InitPhysicsObject(glm::vec3(0, 0.9f, -6), 0.f, BoundingBox::Type::OBB, glm::vec3(1.5f, 1.4f, 1.7f), -90, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[3].InitPhysicsObject(glm::vec3(0, 0.9f, 6), 0.f, BoundingBox::Type::OBB, glm::vec3(1.5f, 1.4f, 1.7f), 90, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[4].InitPhysicsObject(glm::vec3(-6, 0.9f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(1.5f, 1.4f, 1.7f), 0, glm::vec3(0, 1, 0), miscSettings);

	// Stall Tables
	worldObjects[5].InitPhysicsObject(glm::vec3(5.655f, 0.25f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(.475f, .5f, 1.74f), 180, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[6].InitPhysicsObject(glm::vec3(0, 0.25f, -5.655f), 0.f, BoundingBox::Type::OBB, glm::vec3(.475f, .5f, 1.74f), -90, glm::vec3(0, 1, 0), miscSettings);\
	worldObjects[7].InitPhysicsObject(glm::vec3(0, 0.25f, 5.655f), 0.f, BoundingBox::Type::OBB, glm::vec3(.475f, .5f, 1.74f), 90, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[8].InitPhysicsObject(glm::vec3(-5.655f, 0.25f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(.475f, .5f, 1.74f), 0, glm::vec3(0, 1, 0), miscSettings);
	//
	worldObjects[9].InitPhysicsObject(glm::vec3(6.1175f, 0.25f, -.86f), 0.f, BoundingBox::Type::OBB, glm::vec3(1.4f, .5f, .02f), 180, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[10].InitPhysicsObject(glm::vec3(6.1175f, 0.25f, .86f), 0.f, BoundingBox::Type::OBB, glm::vec3(1.4f, .5f, .02f), 180, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[11].InitPhysicsObject(glm::vec3(6.8075, 0.25f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(.02f, .5f, 1.74f), -90, glm::vec3(0, 1, 0), miscSettings);

	worldObjects[12].InitPhysicsObject(glm::vec3(-.86f, 0.25f, -6.1175f), 0.f, BoundingBox::Type::OBB, glm::vec3(1.4f, .5f, .02f), -90, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[13].InitPhysicsObject(glm::vec3(.86f, 0.25f, -6.1175f), 0.f, BoundingBox::Type::OBB, glm::vec3(1.4f, .5f, .02f), -90, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[14].InitPhysicsObject(glm::vec3(0, 0.25f, -6.8075), 0.f, BoundingBox::Type::OBB, glm::vec3(.02f, .5f, 1.74f), -90, glm::vec3(0, 1, 0), miscSettings);

	worldObjects[15].InitPhysicsObject(glm::vec3(-.86f, 0.25f, 6.1175f), 0.f, BoundingBox::Type::OBB, glm::vec3(1.4f, .5f, .02f), 90, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[16].InitPhysicsObject(glm::vec3(.86f, 0.25f, 6.1175f), 0.f, BoundingBox::Type::OBB, glm::vec3(1.4f, .5f, .02f), 90, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[17].InitPhysicsObject(glm::vec3(0, 0.25f, 6.8075), 0.f, BoundingBox::Type::OBB, glm::vec3(.02f, .5f, 1.74f), 90, glm::vec3(0, 1, 0), miscSettings);

	worldObjects[18].InitPhysicsObject(glm::vec3(-6.1175f, 0.25f, -.86f), 0.f, BoundingBox::Type::OBB, glm::vec3(1.4f, .5f, .02f), 0, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[19].InitPhysicsObject(glm::vec3(-6.1175f, 0.25f, .86f), 0.f, BoundingBox::Type::OBB, glm::vec3(1.4f, .5f, .02f), 0, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[20].InitPhysicsObject(glm::vec3(-6.8075, 0.25f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(.02f, .5f, 1.74f), 0, glm::vec3(0, 1, 0), miscSettings);


	// Tables
	worldObjects[21].InitPhysicsObject(glm::vec3(-3, 0, 3.6f), 0.f, BoundingBox::Type::OBB, glm::vec3(1.4f, 1.1f, 1.4f), 50, glm::vec3(0, 1, 0), miscSettings);

	// Ferris Wheel
	worldObjects[22].InitPhysicsObject(glm::vec3(-10, 0, -7), 0.f, BoundingBox::Type::OBB, glm::vec3(10.f, 5.f, 5.f), 45, glm::vec3(0, 1, 0), miscSettings);

	// Food Stand
	worldObjects[23].InitPhysicsObject(glm::vec3(-3.6, 0.5f, 5), 0.f, BoundingBox::Type::OBB, glm::vec3(1.3f, 0.7f, 1.1f), -15, glm::vec3(0, 1, 0), miscSettings);

	// Fountain
	worldObjects[24].InitPhysicsObject(glm::vec3(0, 0.3f, 0), 0.f, BoundingBox::Type::SPHERE, glm::vec3(1.45f, 0, 0), 0, glm::vec3(0, 1, 0), miscSettings);

	// Plank
	worldObjects[25].InitPhysicsObject(glm::vec3(0, 0.6f, 6.6f), 0.f, BoundingBox::Type::OBB, glm::vec3(.15f, 0.03f, 1.1f), 90.f, glm::vec3(0, 1, 0), miscSettings);

	// MATERIAL INITIALIZATIONS
	meshList_riseTop[GEO_STALL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_riseTop[GEO_STALL]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_riseTop[GEO_STALL]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_riseTop[GEO_STALL]->material.kShininess = 1.0f;

	meshList_riseTop[GEO_PLANK]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_riseTop[GEO_PLANK]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_riseTop[GEO_PLANK]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_riseTop[GEO_PLANK]->material.kShininess = 1.0f;
}

void SceneRiseTop::Update(double dt)
{
	BaseScene::Update(dt);

	// BOUNDING BOX TOGGLE
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_M)) {
		if (showBoundingBox) {
			showBoundingBox = false;
		}
		else {
			showBoundingBox = true;
		}
	}

	// INITIAL INTERACTIVES
	addInteractives("Return to Hub", 'I', glm::vec3(1, 0, 0));

	// PHASE HANDLING
	switch (part) {
	case 0:
		if (phase > 9) {	
			part++;
			for (int i = 0; i < 5; i++) {
				addPickables("PingPong Ball", glm::vec3(.5f + (rand() % 11 + 5) / 100.f, 1.f, 5.55f + (rand() % 11 + 5) / 100.f));
			}
		}
		break;

	case 1:
		addInteractives("Place PingPong Ball (Start)", 'I', worldObjects[7].position + glm::vec3(0.f, .25f, -.1f));
		break;

	case 2:
		if (itemInHand != nullptr) {
			if (itemInHand->name == "Controller") {
				part++;
			}
		}
		
		break;
		
	case 3:
		if (phase > 4) {
			part++;
			gameTimeElapsed = 0.f;
			changeOrientationElapsed = 10.f;
			direction = 1;
		}
		break;
		
	case 4:
		gameTimeElapsed += dt;
		changeOrientationElapsed += dt;

		if (changeOrientationElapsed >= changeOrientation) {
			plankTargetOrientation = 10.f + (rand() % 200) / 10.f;
			plankTargetOrientation *= direction;
			playerPlankTargetOrientation = plankTargetOrientation + ((rand() % 100) / 10.f) * direction;
			lerpOrientationSpeed = 0.5f + (rand() % 20) / 10.f;
			changeOrientationElapsed = 0.f;
			changeOrientation = 6.f - rand() % static_cast<int>(glm::clamp(static_cast<float>(gameTimeElapsed) / 4.f, 1.f, 5.f));

			if (direction == 1) {
				direction = -1;
			}
			else {
				direction = 1;
			}
		}

		if (itemInHand != nullptr) {
			if (itemInHand->name == "Controller") {
				int extraInstableStrength = 0.f;
				if (direction == -1) {
					if (playerPlankTargetOrientation < plankTargetOrientation) {
						extraInstableStrength = glm::clamp((plankTargetOrientation - playerPlankTargetOrientation) * 4.f, 0.f, 150.f);
					}
				}
				else {
					if (playerPlankTargetOrientation > plankTargetOrientation) {
						extraInstableStrength = glm::clamp((playerPlankTargetOrientation - plankTargetOrientation) * 4.f, 0.f, 150.f);
					}
				}


				if (MouseController::GetInstance()->IsButtonDown(0)) {
					playerPlankTargetOrientation += (10 + extraInstableStrength) * dt;
				}
				if (MouseController::GetInstance()->IsButtonDown(1)) {
					playerPlankTargetOrientation -= (10 + extraInstableStrength) * dt;
				}

				if (MouseController::GetInstance()->IsButtonUp(0) && MouseController::GetInstance()->IsButtonUp(1)) {
					float t = 1.f - std::exp(-0.5f * (float)dt);
					playerPlankTargetOrientation += (plankTargetOrientation - playerPlankTargetOrientation) * t;
				}
			}
		}

		break;

	default:
		break;
	}

	initializePickablesInteractives();
	getClosestInteractive();


	// INTERACTIVES EVENT HANDLER
	if (interactedIndex != -1 && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_F)) {
		if (interactivesType[interactedIndex] == 'I') {
			if (interactives[interactedIndex] == "Return to Hub" && nextScene == 0) {
				nextScene = 1;
				nextSceneDelay = 1.f;
				sceneSwitchHUD.resetScale(glm::vec2(.25f));
				sceneSwitchHUD.setTargetScale(glm::vec2(1.f));
			}
			else if (interactives[interactedIndex] == "Place PingPong Ball (Start)") {
				if (part == 1 && itemInHand != nullptr) {
					if (itemInHand->name == "PingPong Ball") {
						if (amountOfItem > 1) {
							dropItemInHand(1);
							pickables[newestPickableIndex]->body.position = glm::vec3(0, 0.8f, 6.6f);
						}
						else {
							Pickable* temp = itemInHand;
							dropItemInHand(1);
							temp->body.position = glm::vec3(0, 0.8f, 6.6f);
						}
						
						part++;
					}
				}
			}
		}
	}

	//
	float t = 1.f - std::exp(-lerpOrientationSpeed * (float)dt);
	plankOrientation += (plankTargetOrientation - plankOrientation) * t;
	plankOrientation += (playerPlankTargetOrientation - plankOrientation) * t;

	// UPDATE GRASS DENSITY
	UpdateGrassDensity(dt);
}

void SceneRiseTop::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x + m_viewBobOffset.x, camera.position.y + m_viewBobOffset.y, camera.position.z + m_viewBobOffset.z,
		camera.target.x + m_viewBobOffset.x, camera.target.y + m_viewBobOffset.y, camera.target.z + m_viewBobOffset.z,
		camera.up.x, camera.up.y, camera.up.z
	);

	// Load identity matrix into the model stack
	modelStack.LoadIdentity();

	{
		if (light[0].type == Light::DIRECTIONAL)
		{
			glm::vec3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
			glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
		}
		else if (light[0].type == Light::SPOT)
		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
			glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[0].spotDirection, 0);
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
		}
		else {
			// Calculate the light position in camera space
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		}
		if (light[1].type == Light::DIRECTIONAL)
		{
			glm::vec3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
			glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
		}
		else if (light[1].type == Light::SPOT)
		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[1].position, 1);
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
			glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[1].spotDirection, 0);
			glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
		}
		else {
			// Calculate the light position in camera space
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[1].position, 1);
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		}
		if (light[2].type == Light::DIRECTIONAL)
		{
			glm::vec3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
			glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
		}
		else if (light[2].type == Light::SPOT)
		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[2].position, 1);
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
			glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[2].spotDirection, 0);
			glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
		}
		else {
			// Calculate the light position in camera space
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[2].position, 1);
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		}
		if (light[3].type == Light::DIRECTIONAL)
		{
			glm::vec3 lightDir(light[3].position.x, light[3].position.y, light[3].position.z);
			glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
			glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
		}
		else if (light[3].type == Light::SPOT)
		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[3].position, 1);
			glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
			glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[3].spotDirection, 0);
			glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
		}
		else {
			// Calculate the light position in camera space
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[3].position, 1);
			glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		}
		if (light[4].type == Light::DIRECTIONAL)
		{
			glm::vec3 lightDir(light[4].position.x, light[4].position.y, light[4].position.z);
			glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
			glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
		}
		else if (light[4].type == Light::SPOT)
		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[4].position, 1);
			glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
			glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[4].spotDirection, 0);
			glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
		}
		else {
			// Calculate the light position in camera space
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[4].position, 1);
			glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		}
		if (light[5].type == Light::DIRECTIONAL)
		{
			glm::vec3 lightDir(light[5].position.x, light[5].position.y, light[5].position.z);
			glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
			glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
		}
		else if (light[5].type == Light::SPOT)
		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[5].position, 1);
			glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
			glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[5].spotDirection, 0);
			glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
		}
		else {
			// Calculate the light position in camera space
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[5].position, 1);
			glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		}
		if (light[6].type == Light::DIRECTIONAL)
		{
			glm::vec3 lightDir(light[6].position.x, light[6].position.y, light[6].position.z);
			glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
			glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
		}
		else if (light[6].type == Light::SPOT)
		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[6].position, 1);
			glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
			glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[6].spotDirection, 0);
			glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
		}
		else {
			// Calculate the light position in camera space
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[6].position, 1);
			glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		}
		if (light[7].type == Light::DIRECTIONAL)
		{
			glm::vec3 lightDir(light[7].position.x, light[7].position.y, light[7].position.z);
			glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
			glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
		}
		else if (light[7].type == Light::SPOT)
		{
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[7].position, 1);
			glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
			glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[7].spotDirection, 0);
			glUniform3fv(m_parameters[U_LIGHT7_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
		}
		else {
			// Calculate the light position in camera space
			glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[7].position, 1);
			glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		}
	}

	// GENERAL MODELS
	{
		PushPop axesGuard(modelStack);
		//RenderMesh(meshList[GEO_AXES], false);
	}

	{
		PushPop skybox(modelStack);
		modelStack.Scale(2.f, 2.f, 2.f);
		RenderSkybox();
	}

	{
		PushPop backgroundBuildings(modelStack);
		modelStack.Translate(0.f, -15.f, 0.f);
		modelStack.Scale(0.2f, 0.4f, 0.2f);
		RenderMesh(meshList[GEO_BACKGROUND_BUILDINGS1], true);
		RenderMesh(meshList[GEO_BACKGROUND_BUILDINGS2], true);
	}

	{
		PushPop floor(modelStack);
		modelStack.Scale(0.2f, 0.2f, 0.2f);
		RenderMesh(meshList[GEO_FLOOR], true);
	}

	{
		PushPop fence(modelStack);
		modelStack.Scale(0.2f, 0.2f, 0.2f);
		RenderMesh(meshList[GEO_FENCE], true);
	}

	{
		PushPop table(modelStack);
		modelStack.Translate(worldObjects[21].position.x, worldObjects[21].position.y, worldObjects[21].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[21].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(.13f, .13f, .13f);
		RenderMesh(meshList[GEO_TABLE], true);
	}

	{
		PushPop ferriswheel(modelStack);
		modelStack.Translate(worldObjects[22].position.x, worldObjects[22].position.y, worldObjects[22].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[22].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(.2f, .2f, .2f);
		RenderMesh(meshList[GEO_FERRISWHEEL], true);
	}

	{
		PushPop foodstand(modelStack);
		modelStack.Translate(worldObjects[23].position.x, worldObjects[23].position.y, worldObjects[23].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[23].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(.12f, .12f, .12f);
		RenderMesh(meshList[GEO_FOODSTAND], true);
	}

	{
		PushPop fountain(modelStack);
		modelStack.Translate(worldObjects[24].position.x, worldObjects[24].position.y, worldObjects[24].position.z);
		modelStack.Scale(0.22f, 0.22f, 0.22f);
		RenderMesh(meshList[GEO_FOUNTAIN], true);
	}

	for (int i = 1; i < 5; i++)
	{
		PushPop stall(modelStack);
		modelStack.Translate(worldObjects[i].position.x, worldObjects[i].position.y, worldObjects[i].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[i].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(0.2f, 0.2f, 0.2f);
		RenderMesh(meshList_riseTop[GEO_STALL], true);
	}

	// GRASS HANDLING
	{
		glm::vec3 localCamPos = camera.position;
		glm::vec3 forward = glm::normalize(camera.target - camera.position);

		glDepthMask(GL_FALSE);

		// Only render active grass count
		for (int i = 0; i < activeGrassCount; i++) {
			glm::vec3 pos = grassClumps[i];
			glm::vec3 toGrass = pos - localCamPos;
			float dist = glm::length(toGrass);

			if (dist > 15.f) continue;

			if (dist > 0.001f) {
				float dot = glm::dot(forward, toGrass / dist);
				if (dot < 0.25f) continue;
			}

			glm::vec3 dir = localCamPos - pos;
			dir.y = 0.f;
			float yaw = glm::degrees(atan2(dir.x, dir.z));

			PushPop grass(modelStack);
			modelStack.Translate(pos.x, pos.y, pos.z);
			modelStack.Rotate(yaw, 0, 1, 0);
			modelStack.Scale(.4f, .6f, .4f);
			RenderMesh(meshList[GEO_GRASS], false);
		}

		glDepthMask(GL_TRUE);
	}

	// LOCAL MODELS
	{
		PushPop plank(modelStack);
		worldObjects[25].SetOrientation(plankOrientation, 90.f, 0.f);
		modelStack.Translate(worldObjects[25].position.x, worldObjects[25].position.y, worldObjects[25].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[25].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(0.12f, 0.12f, 0.12f);
		RenderMesh(meshList_riseTop[GEO_PLANK], true);
	}

	{
		if (showBoundingBox) {
			for (int i = 0; i < TOTAL_PICKABLES; i++) {
				if (pickables[i] != nullptr) {
					PushPop yes(modelStack);
					modelStack.Translate(pickables[i]->body.position.x, pickables[i]->body.position.y, pickables[i]->body.position.z);
					glm::mat4 rotation = glm::mat4_cast(pickables[i]->body.orientation);
					modelStack.MultMatrix(rotation);
					modelStack.Scale(pickables[i]->body.boundingBox.getWidth(), pickables[i]->body.boundingBox.getHeight(), pickables[i]->body.boundingBox.getDepth());
					//modelStack.Scale(5.f, 0.1f, 5.f);

					meshList_riseTop[GEO_WALL]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
					meshList_riseTop[GEO_WALL]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
					meshList_riseTop[GEO_WALL]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
					meshList_riseTop[GEO_WALL]->material.kShininess = 1.0f;

					RenderMesh(meshList_riseTop[GEO_WALL], true);
				}
			}

			for (int i = 0; i < TOTAL_PHYSICSOBJECT; i++) {
				PushPop debug(modelStack);
				modelStack.Translate(worldObjects[i].position.x, worldObjects[i].position.y, worldObjects[i].position.z);
				glm::mat4 rotation = glm::mat4_cast(worldObjects[i].orientation);
				modelStack.MultMatrix(rotation);
				modelStack.Scale(worldObjects[i].boundingBox.getWidth(), worldObjects[i].boundingBox.getHeight(), worldObjects[i].boundingBox.getDepth());
				if (i != 0 && i > 4) {
					RenderMesh(meshList_riseTop[GEO_WALL], true);
				}
			}
		}
	}

	{
		if (itemInHand != nullptr) {
			// Camera forward, right, and up vectors
			glm::vec3 forward = glm::normalize(camera.target - camera.position);
			glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));
			glm::vec3 up = glm::normalize(glm::cross(right, forward));

			// Offsets in local camera space
			float forwardOffset = 0.45f; // in front of camera
			float rightOffset = -0.3f; // to the side, if you want
			float upOffset = -0.2f; // slightly down, like holding

			glm::vec3 itemInHandPos = camera.position
				+ forward * forwardOffset
				+ right * rightOffset
				+ up * upOffset;

			// Yaw: rotation around Y (horizontal)
			float yaw = glm::degrees(atan2(forward.x, forward.z));

			// Pitch: rotation around X (vertical)
			float pitch = glm::degrees(asin(forward.y));

			itemInHand->body.position = itemInHandPos;

			// rotation offsets when held
			if (itemInHand->name == "Pig") {
				itemInHand->body.SetOrientation(0, 180.f, 0);
			}
			else if (itemInHand->name == "RTX 5090") {
				itemInHand->body.SetOrientation(0, 100.f, 0);
			}
			else if (itemInHand->name == "Controller") {
				itemInHand->body.SetOrientation(-90.f, 0, -90.f);
			}
			else {
				itemInHand->body.SetOrientation(0, 0, 0);
			}

			itemInHand->body.RotateOrientation(-pitch, yaw, 0);
		}


		// PICKABLE ITEMS RENDERING
		for (int i = 0; i < TOTAL_PICKABLES; i++) {
			if (pickables[i] != nullptr) {
				modelStack.PushMatrix();
				modelStack.Translate(pickables[i]->body.position.x, pickables[i]->body.position.y, pickables[i]->body.position.z);
				glm::mat4 rotation = glm::mat4_cast(pickables[i]->body.orientation);
				modelStack.MultMatrix(rotation);

				if (pickables[i]->name == "Baseball") {
					modelStack.Scale(0.2f, 0.2f, 0.2f);
					RenderMesh(meshList[GEO_BASEBALL], enableLight);
				}
				else if (pickables[i]->name == "Coke") {
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					RenderMesh(meshList[GEO_CANSCOKE], enableLight);
				}
				else if (pickables[i]->name == "Mountain Dew") {
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					RenderMesh(meshList[GEO_CANSMTNDEW], enableLight);
				}
				else if (pickables[i]->name == "Spite") {
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					RenderMesh(meshList[GEO_CANSSPRITE], enableLight);
				}
				else if (pickables[i]->name == "Pepsi") {
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					RenderMesh(meshList[GEO_CANSPEPSI], enableLight);
				}
				else if (pickables[i]->name == "Figurine") {
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					RenderMesh(meshList[GEO_FIGURINE], enableLight);
				}
				else if (pickables[i]->name == "Pig") {
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					RenderMesh(meshList[GEO_PIG], enableLight);
				}
				else if (pickables[i]->name == "Plushie") {
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					RenderMesh(meshList[GEO_PLUSHIE], enableLight);
				}
				else if (pickables[i]->name == "RTX 5090") {
					modelStack.Scale(0.15f, 0.15f, 0.15f);
					RenderMesh(meshList[GEO_5090], enableLight);
				}
				else if (pickables[i]->name == "PingPong Ball") {
					modelStack.Scale(0.02f, 0.02f, 0.02f);
					RenderMesh(meshList[GEO_PINGPONGBALL], enableLight);
				}
				else if (pickables[i]->name == "Controller") {
					modelStack.Scale(0.02f, 0.02f, 0.02f);
					RenderMesh(meshList[GEO_CONTROLLER], enableLight);
				}

				modelStack.PopMatrix();
			}
		}
	}

	{
		// DIALOGUE RENDERING
		RenderMeshOnScreen(meshList[GEO_DIALOGUEFADE_GUI], dialogueFadeHUD.getPosition().x, dialogueFadeHUD.getPosition().y, 1600, 900);

		switch (part) {
		case 0: // part 1
			switch (phase) {
			case 0:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Woah there!", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			case 1:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "This plank has a mind of its own.", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			case 2:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "It won't stay balanced for long.", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			case 3:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Hold LMB to tilt it left.", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			case 4:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Press RMB to tilt it right.", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			case 5:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Counter its movement to stay balanced.", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			case 6:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Keep the plank steady...", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			case 7:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Or the ball will roll off!", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			case 8:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Small corrections work best.", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			case 9:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Ready to test your reflexes?", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			default:
				break;
			}

			break;
		case 1:
			switch (phase) {
			case 0:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Pass me a PingPong Ball and we'll start! Equip the controller to control the plank.", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			default:
				break;
			}
			break;
			
		case 2:
			switch (phase) {
			case 0:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Equip the controller. It will allow you to control the plank", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;

			default:
				break;
			}
			break;

		case 3:
			switch (phase) {
			case 0:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Aight let's go!", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;
			case 1:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "3", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;
			case 2:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "2", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;
			case 3:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "1", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;
			case 4:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Start!", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;
			default:
				break;

			}

		default:
			break;
		}
	}

	RenderUI();
}

void SceneRiseTop::RenderUI()
{
	{
		// GUI RENDERING

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (interactedIndex != -1) {
			RenderMeshOnScreen(meshList[GEO_CROSSHAIROPAQUE_GUI], crosshair.getPosition().x, crosshair.getPosition().y, 1600, 900);

			RenderMeshOnScreen(meshList[GEO_INTERACTFADE_GUI], interactPrompt.getPosition().x, interactPrompt.getPosition().y, 1600, 900);
			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], interactives[interactedIndex], glm::vec3(1, 1, 1), 20, 410 + interactPrompt.getPosition().x * 1.5f, -10 + interactPrompt.getPosition().y, 'R', .6f);

			if (itemInHand->name == "Baseball") {
				RenderMeshOnScreen(meshList[GEO_BASEBALL_TGA], itemInHandImageHUD.getPosition().x - (itemInHandImageHUD.getScale().x / 2.f), itemInHandImageHUD.getPosition().y + (itemInHandImageHUD.getScale().y / 2.f), itemInHandImageHUD.getScale().x, itemInHandImageHUD.getScale().y);
			}
			else if (itemInHand->name == "Coke" || itemInHand->name == "Mountain Dew" || itemInHand->name == "Sprite" || itemInHand->name == "Pepsi") {
				RenderMeshOnScreen(meshList[GEO_SODA_TGA], itemInHandImageHUD.getPosition().x - (itemInHandImageHUD.getScale().x / 2.f), itemInHandImageHUD.getPosition().y + (itemInHandImageHUD.getScale().y / 2.f), itemInHandImageHUD.getScale().x, itemInHandImageHUD.getScale().y);
			}
			else if (itemInHand->name == "Figurine") {
				RenderMeshOnScreen(meshList[GEO_FIGURINE_TGA], itemInHandImageHUD.getPosition().x - (itemInHandImageHUD.getScale().x / 2.f), itemInHandImageHUD.getPosition().y + (itemInHandImageHUD.getScale().y / 2.f), itemInHandImageHUD.getScale().x, itemInHandImageHUD.getScale().y);
			}
			else if (itemInHand->name == "Pig") {
				RenderMeshOnScreen(meshList[GEO_PIG_TGA], itemInHandImageHUD.getPosition().x - (itemInHandImageHUD.getScale().x / 2.f), itemInHandImageHUD.getPosition().y + (itemInHandImageHUD.getScale().y / 2.f), itemInHandImageHUD.getScale().x, itemInHandImageHUD.getScale().y);
			}
			else if (itemInHand->name == "Plushie") {
				RenderMeshOnScreen(meshList[GEO_PLUSHIE_TGA], itemInHandImageHUD.getPosition().x - (itemInHandImageHUD.getScale().x / 2.f), itemInHandImageHUD.getPosition().y + (itemInHandImageHUD.getScale().y / 2.f), itemInHandImageHUD.getScale().x, itemInHandImageHUD.getScale().y);
			}
			else if (itemInHand->name == "RTX 5090") {
				RenderMeshOnScreen(meshList[GEO_5090_TGA], itemInHandImageHUD.getPosition().x - (itemInHandImageHUD.getScale().x / 2.f), itemInHandImageHUD.getPosition().y + (itemInHandImageHUD.getScale().y / 2.f), itemInHandImageHUD.getScale().x, itemInHandImageHUD.getScale().y);

			}
			else if (itemInHand->name == "Baseball") {
				RenderMeshOnScreen(meshList[GEO_BASEBALL_TGA], itemInHandImageHUD.getPosition().x - (itemInHandImageHUD.getScale().x / 2.f), itemInHandImageHUD.getPosition().y + (itemInHandImageHUD.getScale().y / 2.f), itemInHandImageHUD.getScale().x, itemInHandImageHUD.getScale().y);

			}
			else if (itemInHand->name == "Controller") {
				RenderMeshOnScreen(meshList[GEO_CONTROLLER_TGA], itemInHandImageHUD.getPosition().x - (itemInHandImageHUD.getScale().x / 2.f), itemInHandImageHUD.getPosition().y + (itemInHandImageHUD.getScale().y / 2.f), itemInHandImageHUD.getScale().x, itemInHandImageHUD.getScale().y);

			}
			else if (itemInHand->name == "PingPong Ball") {
				RenderMeshOnScreen(meshList[GEO_PINGPONGBALL_TGA], itemInHandImageHUD.getPosition().x - (itemInHandImageHUD.getScale().x / 2.f), itemInHandImageHUD.getPosition().y + (itemInHandImageHUD.getScale().y / 2.f), itemInHandImageHUD.getScale().x, itemInHandImageHUD.getScale().y);
			}

			//meshlist[font type], text, color, size, x, y, alignment, spacing percentage
			if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_F))
			{
				RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "[F]", glm::vec3(109 / 255.f, 41 / 255.f, 34 / 255.f), 26, 440 + interactPrompt.getPosition().x, -13 + interactPrompt.getPosition().y, 'L', .6f);
			}
			else {
				RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[F]", glm::vec3(109 / 255.f, 41 / 255.f, 34 / 255.f), 26, 440 + interactPrompt.getPosition().x, -13 + interactPrompt.getPosition().y, 'L', .6f);
			}
		}
		else {
			RenderMeshOnScreen(meshList[GEO_CROSSHAIRTRANSLUCENT_GUI], crosshair.getPosition().x, crosshair.getPosition().y, 1600, 900);
		}

		RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "CASH: " + std::to_string(accumulatedCash), glm::vec3(1, 1, 1), 25, -700, 350, 'L', .6f);

		if (itemInHand != nullptr) {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			RenderMeshOnScreen(meshList[GEO_ITEMINHANDBORDER_GUI], 603 - (itemInHandHUD.getScale().x * .5f), -343.2f + (itemInHandHUD.getScale().y * .5f), 214 + itemInHandHUD.getScale().x, 33.7f + itemInHandHUD.getScale().y);
			RenderMeshOnScreen(meshList[GEO_ITEMINHANDFADEBACKGROUND_GUI], 603 - (itemInHandHUD.getScale().x * .5f), -343.2f + (itemInHandHUD.getScale().y * .5f), 214 + itemInHandHUD.getScale().x, 33.7f + itemInHandHUD.getScale().y);
			RenderMeshOnScreen(meshList[GEO_ITEMINHANDFADE_GUI], 0, 0 + itemInHandHUD.getScale().y, 1600, 900);

			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "(" + std::to_string(amountOfItem) + "x) " + itemInHand->name, glm::vec3(1, 1, 1), 20, 690, -355 + itemInHandHUD.getScale().y, 'R', .6f);

			if (itemInUse) {
				RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "[LMB]", glm::vec3(1, 1, 1), 15, 700, -300 + itemInHandHUD.getScale().y, 'R', .6f);
			}
			else {
				RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[LMB]", glm::vec3(1, 1, 1), 15, 700, -300 + itemInHandHUD.getScale().y, 'R', .6f);
			}
			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "Use", glm::vec3(1, 1, 1), 15, 655 - 10, -300 + itemInHandHUD.getScale().y, 'R', .6f);

			RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[X]", glm::vec3(1, 1, 1), 15, 700, -320 + itemInHandHUD.getScale().y, 'R', .6f);
			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "Drop", glm::vec3(1, 1, 1), 15, 673 - 10, -320 + itemInHandHUD.getScale().y, 'R', .6f);
		}

		// DEBUG
		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "PART: " + std::to_string(part) + " PHASE: " + std::to_string(phase), glm::vec3(1, 1, 1), 15, 785, 435, 'R', .6f);
		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "DT MULTIPLIER: " + std::to_string(dtMultiplier), glm::vec3(1, 1, 1), 15, 595, 435, 'R', .6f);
		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "ACTIVE GRASS COUNT: " + std::to_string(activeGrassCount), glm::vec3(1, 1, 1), 15, 375, 435, 'R', .6f);
		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "FPS: " + std::to_string(currentFPS), glm::vec3(1, 1, 1), 15, 140, 435, 'R', .6f);
	}

	{
		// EUI RENDERING
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec3 euiPos(0, 1000, 0);

		for (int i = 0; i < noOfInteractives; i++) {
			bool temp = interactivesType[i] == 'I';
			if (!temp && pickables[interactivePickablesIndex[i]] != nullptr) {
				temp = !pickables[interactivePickablesIndex[i]]->isHeld;
			}

			if (temp) {
				euiPos = interactivesPos[i] + glm::vec3(0, 0.25f, 0);

				glm::vec3 dir = camera.position - euiPos;
				dir = glm::normalize(dir);

				float yaw = glm::degrees(atan2(dir.x, dir.z));
				float pitch = glm::degrees(asin(dir.y));

				if (interactedIndex == i) {
					modelStack.PushMatrix();
					modelStack.Translate(euiPos.x, euiPos.y, euiPos.z);
					modelStack.Rotate(yaw, 0.f, 1.f, 0.f);
					modelStack.Rotate(-pitch, 1.f, 0.f, 0.f);
					modelStack.Scale(interactEUI.getScale().x, interactEUI.getScale().x, interactEUI.getScale().x);

					meshList[GEO_INTERACTED_EUI]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
					meshList[GEO_INTERACTED_EUI]->material.kDiffuse = glm::vec3(1.f, 1.f, 1.f);
					meshList[GEO_INTERACTED_EUI]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
					meshList[GEO_INTERACTED_EUI]->material.kShininess = 1.0f;

					RenderMesh(meshList[GEO_INTERACTED_EUI], false);

					modelStack.PopMatrix();
				}

				modelStack.PushMatrix();
				modelStack.Translate(euiPos.x, euiPos.y, euiPos.z);
				modelStack.Rotate(yaw, 0.f, 1.f, 0.f);
				modelStack.Rotate(-pitch, 1.f, 0.f, 0.f);
				modelStack.Scale(.02f, .02f, .02f);

				meshList[GEO_INTERACT_EUI]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
				meshList[GEO_INTERACT_EUI]->material.kDiffuse = glm::vec3(1.f, 1.f, 1.f);
				meshList[GEO_INTERACT_EUI]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
				meshList[GEO_INTERACT_EUI]->material.kShininess = 1.0f;

				RenderMesh(meshList[GEO_INTERACT_EUI], false);

				modelStack.PopMatrix();
			}
		}
	}

	{
		// SWITCH SCENE UI
		if (nextScene != 0) {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			RenderMeshOnScreen(meshList[GEO_SWITCHSCENE_GUI], 0.f, 0.f, 1200 * sceneSwitchHUD.getScale().x, 675 * sceneSwitchHUD.getScale().x);
		}
	}
}

void SceneRiseTop::Exit()
{
	BaseScene::Exit();
}

void SceneRiseTop::RegenerateGrassPositions()
{
	int index = 0;
	int targetCount = static_cast<int>(NUM_GRASSCLUMPS * grassDensityMultiplier);

	// distribute grass across 3x3 grid sections
	int grassPerSection = targetCount / 9;

	for (int x = -1; x < 2; x++) {
		for (int z = -1; z < 2; z++) {
			for (int i = 0; i < grassPerSection && index < NUM_GRASSCLUMPS; i++) {
				glm::vec3 pos(
					((rand() % 7000) - 3500) / 500.f,
					0.f,
					((rand() % 7000) - 3500) / 500.f
				);

				pos += glm::vec3(x * 7.f, 0.f, z * 7.f);
				grassClumps[index++] = pos;
			}
		}
	}

	activeGrassCount = index;
}

void SceneRiseTop::UpdateGrassDensity(double dt)
{
	float smoothingFactor = 0.9f;
	fpsSmoothed = fpsSmoothed * (1.0f - smoothingFactor) + currentFPS * smoothingFactor;

	// fps ratio
	float fpsRatio = fpsSmoothed / targetFPS;

	if (fpsRatio < 0.8f) {
		grassDensityMultiplier -= 0.1f * static_cast<float>(dt);
	}
	// increase if ratio is good
	else if (fpsRatio > 1.05f && grassDensityMultiplier < 1.0f) {
		grassDensityMultiplier += 0.02f * static_cast<float>(dt);
	}

	// clamp density multiplier
	grassDensityMultiplier = glm::clamp(grassDensityMultiplier, 0.1f, 1.0f);

	// regenerate grass positions if density changed significantly
	int targetCount = static_cast<int>(NUM_GRASSCLUMPS * grassDensityMultiplier);
	if (abs(targetCount - activeGrassCount) > NUM_GRASSCLUMPS * 0.1f) {
		RegenerateGrassPositions();
	}
}
