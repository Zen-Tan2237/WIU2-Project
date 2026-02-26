// GLM Headers
#include "GL\glew.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>

#include "SceneKnockdown.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"

#include "KeyboardController.h"
#include "MouseController.h"
#include "CollisionDetection.h"
#include <vector>

#include "audio.h"

SceneKnockdown::SceneKnockdown()
{
}

SceneKnockdown::~SceneKnockdown()
{
}

void SceneKnockdown::Init()
{
	BaseScene::Init();

	//light
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


	//models
	//meshList[GEO_STALL] = MeshBuilder::GenerateOBJMTL("Stall", "OBJ//stall.obj", "OBJ//stall.mtl");

	//debug
	meshList_hub[GEO_WALL] = MeshBuilder::GenerateCube("wall", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList_hub[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", glm::vec3(0.f, 1.f, 0.f), 1.f, 36, 18);

	meshList_hub[GEO_STALL] = MeshBuilder::GenerateOBJ("stall", "Models//minigame_Stall.obj");
	meshList_hub[GEO_STALL]->textureID = LoadTGA("Textures//minigameStall.tga");

	meshList_hub[GEO_TABLE] = MeshBuilder::GenerateOBJ("table", "Models//table.obj");
	meshList_hub[GEO_TABLE]->textureID = LoadTGA("Textures//table.tga");

	meshList_hub[GEO_FOODSTAND] = MeshBuilder::GenerateOBJ("foodstand", "Models//Hot_Dog_Stand.obj");
	meshList_hub[GEO_FOODSTAND]->textureID = LoadTGA("Textures//Hotdog.tga");

	meshList_hub[GEO_FERRISWHEEL] = MeshBuilder::GenerateOBJ("ferriswheel", "Models//FerrisWheel.obj");
	meshList_hub[GEO_FERRISWHEEL]->textureID = LoadTGA("Textures//FerrisWheel.tga");

	meshList_hub[GEO_PEPSI] = MeshBuilder::GenerateOBJ("pepsi", "Models//cans_pepsi.obj");
	meshList_hub[GEO_PEPSI]->textureID = LoadTGA("Textures//Cans_Pepsi.tga");

	meshList_hub[GEO_MAXWELL] = MeshBuilder::GenerateOBJ("basketball", "Models//Maxwell.obj");
	meshList_hub[GEO_MAXWELL]->textureID = LoadTGA("Textures//Maxwell.tga");

	meshList_hub[GEO_BOARD] = MeshBuilder::GenerateOBJ("Woodplank", "Models//WoodPlank.obj");
	meshList_hub[GEO_BOARD]->textureID = LoadTGA("Textures//WoodPlank.tga");

	// Sound init
	AudioManager::Instance().LoadSound("Ball hitting can", "SFX/canhit.mp3");
	AudioManager::Instance().LoadSound("Maxwell", "SFX/MaxwellTheCat.wav");
	AudioManager::Instance().SetLooping("Maxwell", true);
	AudioManager::Instance().SetSoundVolume("Ball hitting can", 0.5f);

	// setup initial item in hand
	addPickables("Baseball", glm::vec3(0, 0, 0));
	itemInHand = pickables[0];
	amountOfItem = 10;
	previousItemInHandName = "";
	itemInUse = false;

	// Part 1 dialogue: Intro to the minigame
	phaseDurations[1][0] = 6.7f;
	phaseDurations[1][1] = 6.7f;
	phaseDurations[1][2] = 6.7f;

	// world objects
	bool miscSettings[2] = { false, false }; // for gravity and drag. override in case of specific objects

	// Floor
	worldObjects[0].InitPhysicsObject(glm::vec3(0, -0.5f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(200, 1, 200), 0, glm::vec3(1, 0, 0), miscSettings);

	//stalls
	worldObjects[1].InitPhysicsObject(glm::vec3(6, 0.1f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(0.1f, 0.1f, 0.1f), 180, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[2].InitPhysicsObject(glm::vec3(0, 0.1f, -6), 0.f, BoundingBox::Type::OBB, glm::vec3(0.1f, 0.1f, 0.1f), -90, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[3].InitPhysicsObject(glm::vec3(0, 0.1f, 6), 0.f, BoundingBox::Type::OBB, glm::vec3(0.1f, 0.1f, 0.1f), 90, glm::vec3(0, 1, 0), miscSettings);
	worldObjects[4].InitPhysicsObject(glm::vec3(-6, 0.1f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(0.1f, 0.1f, 0.1f), 0, glm::vec3(0, 1, 0), miscSettings);

	//tables
	worldObjects[5].InitPhysicsObject(glm::vec3(-3, 0, 3.6f), 0.f, BoundingBox::Type::OBB, glm::vec3(2.2f, 1.5f, 2.2f), 50, glm::vec3(0, 1, 0), miscSettings);

	//ferris wheel
	worldObjects[6].InitPhysicsObject(glm::vec3(-10, 0, -7), 0.f, BoundingBox::Type::OBB, glm::vec3(10.f, 9.f, 5.f), 45, glm::vec3(0, 1, 0), miscSettings);

	//food stand
	worldObjects[7].InitPhysicsObject(glm::vec3(-3.6, 0.5f, 5), 0.f, BoundingBox::Type::OBB, glm::vec3(2.2f, 1.f, 1.92f), -15, glm::vec3(0, 1, 0), miscSettings);


	addPickables("Pepsi", glm::vec3(3, 1, 2));


	// Stalls invis walls
	worldObjects[8].InitPhysicsObject(glm::vec3(-5.6f, 0.2f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(0.4f, 0.6f, 1.9f), 0, glm::vec3(0, 1, 0), miscSettings);
	// Stall board for cans
	worldObjects[9].InitPhysicsObject(glm::vec3(-6.45f, 0.7f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(0.3f, 0.05f, 1.9f), 0, glm::vec3(0, 1, 0), miscSettings);
	// Stall invis walls behind
	worldObjects[10].InitPhysicsObject(glm::vec3(-6.8f, 1.5, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(0.05f, 5.f, 1.9f), miscSettings);

	// The cans for the can knockdown game 
	round = 1;
	generateCanPositions(round);

	camera.Init(glm::vec3(-4.49645f, 0.999999f, -0.0245583f), glm::vec3(-5.49018f, 0.911104f, 0.0336516));

	maxwell.InitPhysicsObject(glm::vec3(-5.6f, 0.5f, -0.8f), 0.f, BoundingBox::Type::OBB, glm::vec3(0.5f), 0, glm::vec3(0, 1, 0), miscSettings);
	int index = 0;

	// GRASS DENSITY
	targetFPS = 60.0f;
	fpsSmoothed = 0.0f;
	grassDensityMultiplier = 0.0f;
	activeGrassCount = 0;

	RegenerateGrassPositions();

	//
	meshList_hub[GEO_TABLE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_TABLE]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_TABLE]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_TABLE]->material.kShininess = 1.0f;

	meshList_hub[GEO_FOODSTAND]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_FOODSTAND]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_FOODSTAND]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_FOODSTAND]->material.kShininess = 1.0f;

	meshList_hub[GEO_FERRISWHEEL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_FERRISWHEEL]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_FERRISWHEEL]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_FERRISWHEEL]->material.kShininess = 1.0f;

	meshList_hub[GEO_STALL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_STALL]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_STALL]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_STALL]->material.kShininess = 1.0f;

	startPhysicsUpdateForCans = false;
	win = false;

	AudioManager::Instance().SetSoundPosition("Maxwell", maxwell.position.x, maxwell.position.y, maxwell.position.z);
	AudioManager::Instance().SoundPlay("Maxwell");
}

void SceneKnockdown::Update(double dt)
{
	BaseScene::Update(dt);

	// Sound update
	AudioManager::Instance().SetListenerPosition(camera.position.x,	camera.position.y, camera.position.z);
	AudioManager::Instance().SetListenerDirection(camera.target.x, camera.target.y, camera.target.z);
	AudioManager::Instance().SetListenerCone(1, 3, 0.5f);


	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_M)) {
		if (temp) {
			temp = false;
		}
		else {
			temp = true;
		}
	}

	// name of interactive, I = interactive, coords
	if (win) addInteractives("Return to SceneHub", 'I', glm::vec3(-5.6f, 0.5f, 0.8f));
	else addInteractives("Talk to monke", 'I', glm::vec3(-5.6f, 0.5f, 0.8f));
	addInteractives("2", 'I', glm::vec3(0, 0, 1));
	addInteractives("Enter SceneTester", 'I', glm::vec3(0, 0, -1));

	addInteractives("Enter Can Knockdown Game", 'I', glm::vec3(0.f, 1.f, 0.f));

	//addPickables("Halal Pork", glm::vec3(0, 0, 0));
	initializePickablesInteractives();
	getClosestInteractive();


	// handle what type of interactive, what type of event
	if (interactedIndex != -1 && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_F)) { // means got prompt, is close to and facing smth
		if (interactivesType[interactedIndex] == 'I') { // its an interactive
			// do it in actual scene instead
			if (interactives[interactedIndex] == "Return to SceneHub" && nextScene == 0 && win) {
				nextScene = 1;
				nextSceneDelay = 1.f;
				sceneSwitchHUD.resetScale(glm::vec2(.25f));
				sceneSwitchHUD.setTargetScale(glm::vec2(1.f));
			}
			else if (interactives[interactedIndex] == "Talk to monke") {
				part = 1;
				phase = 0;
			}
			else if (interactives[interactedIndex] == "2") {
				// do something
			}
			else if (interactives[interactedIndex] == "Enter SceneTester") {
				nextScene = 3;
				nextSceneDelay = 1.f;
				sceneSwitchHUD.resetScale(glm::vec2(.25f));
				sceneSwitchHUD.setTargetScale(glm::vec2(1.f));
			}
			else if (interactives[interactedIndex] == "Enter Can Knockdown Game" && nextScene == 0) {
					nextScene = 4;
					nextSceneDelay = 1.f;
					sceneSwitchHUD.resetScale(glm::vec2(.25f));
					sceneSwitchHUD.setTargetScale(glm::vec2(1.f));
			}
		}
	}

	//debug
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I)) {
		debugPos.x += 5.f * dt;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J)) {
		debugPos.z += 5.f * dt;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K)) {
		debugPos.x -= 5.f * dt;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L)) {
		debugPos.z -= 5.f * dt;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_O)) {
		debugPos.y += 5.f * dt;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_U)) {
		debugPos.y -= 5.f * dt;
	}
	//std::cout << "Debug Pos: " << debugPos.x << ", " << debugPos.y << ", " << debugPos.z << std::endl;

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_M)) {
		debugScale += 2.0f * dt;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_N)) {
		debugScale -= 2.0f * dt;
	}
	std::cout << "Debug Scale: " << debugScale << std::endl;

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_SPACE)) {
		// Shoot the item in hand
	}

	// Update grass density based on FPS
	UpdateGrassDensity(dt);

	std::cout << camera.position.x << camera.position.y << camera.position.z << std::endl;
	std::cout << camera.target.x << camera.target.y << camera.target.z << std::endl;

	// Win condition
	{
		PhysicsObject winHitbox;
		bool miscSettings[2] = { false, false };
		winHitbox.InitPhysicsObject(glm::vec3(-6, 0.1f, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(1.5f, 1.1f, 1.7f), miscSettings);
		int cansKnockedDown = 0;

		for (int i = 0; i < numOfCansInPlay; i++) {
			CollisionData cd;
			if (CheckCollision(cans[i], winHitbox, cd) && this->winCondition[i] != true) {
				winCondition[i] = true;
			}
			// Count number of cans knocked down
			if (winCondition[i]) {
				cansKnockedDown++;
			}
			if (cansKnockedDown >= numOfCansInPlay) {
				// Player wins, do something
				if (round == 3) {
					win = true;
				}
				else {
					round++;
					generateCanPositions(round);
					accumulatedCash += round * 30; // reward player with cash based on round
				}
				std::cout << "WIN CONDITION MET" << std::endl;
			}
		}
	}

	std::vector<CollisionData> contacts;
	contacts.reserve(256);

	// Baseball pickables vs cans
	for (int i = 0; i < noOfPickables; i++) {
		if (!pickables[i] || pickables[i]->name != "Baseball") continue;

		for (int j = 0; j < numOfCansInPlay; j++) {
			CollisionData cd;
			if (CheckCollision(pickables[i]->body, cans[j], cd)) {
				startPhysicsUpdateForCans = true;
				contacts.push_back(cd);
				AudioManager::Instance().SoundPlay("Ball hitting can");
			}
		}
	}

	if (startPhysicsUpdateForCans) {
		// can vs can (unique pairs only)
		for (int i = 0; i < numOfCansInPlay; i++) {
			for (int j = i + 1; j < numOfCansInPlay; j++) {
				CollisionData cd;
				if (CheckCollision(cans[i], cans[j], cd)) {
					contacts.push_back(cd);
				}
			}
		}

		// world vs can
		for (PhysicsObject& obj : worldObjects) {
			for (int i = 0; i < numOfCansInPlay; i++) {
				CollisionData cd;
				if (CheckCollision(obj, cans[i], cd)) {
					contacts.push_back(cd);
				}
			}
		}
	}

	if (startPhysicsUpdateForCans) {
		const int SOLVER_ITERS = 8; // 6–10 is fine

		for (int iter = 0; iter < SOLVER_ITERS; ++iter) {
			for (auto& cd : contacts) {
				ResolveCollision(cd);
			}
		}

		for (int i = 0; i < numOfCansInPlay; i++) {
			cans[i].UpdatePhysics(dt);
		}
	}

	// Rotate maxwell
	{
		float angleToUpdate = 1.f;
		angleToUpdate = glm::radians<float>(angleToUpdate);
		glm::quat rotationDelta = glm::angleAxis(angleToUpdate, glm::vec3(0, 1, 0));
		maxwell.orientation = rotationDelta * maxwell.orientation;

	}
}

void SceneKnockdown::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
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

	{
		PushPop axesGuard(modelStack);
		// Render objects
		//RenderMesh(meshList[GEO_AXES], false);
	}

	{
		PushPop skybox(modelStack);
		modelStack.Scale(2.f, 2.f, 2.f);
		RenderSkybox();
	}

	for (int i = 0; i < TOTAL_PHYSICSOBJECT; i++) {
		PushPop debug(modelStack);
		modelStack.Translate(worldObjects[i].position.x, worldObjects[i].position.y, worldObjects[i].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[i].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(worldObjects[i].boundingBox.getWidth(), worldObjects[i].boundingBox.getHeight(), worldObjects[i].boundingBox.getDepth());
		meshList_hub[GEO_WALL]->material.kAmbient = glm::vec3(0.2f, 0.0f, 0.0f);
		meshList_hub[GEO_WALL]->material.kDiffuse = glm::vec3(1.0f, 0.0f, 0.0f);
		meshList_hub[GEO_WALL]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_M)) {
			RenderMesh(meshList_hub[GEO_WALL], true);
		}
	}

	{
		PushPop winCondition(modelStack);
		modelStack.Translate(-6, 0.1f, 0);
		modelStack.Scale(1.5f, 1.1f, 1.7f);
		meshList_hub[GEO_WALL]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList_hub[GEO_WALL]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		meshList_hub[GEO_WALL]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
		meshList_hub[GEO_WALL]->material.kShininess = 1.0f;
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_M)) {

			RenderMesh(meshList_hub[GEO_WALL], true);
		}
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

	{
		PushPop table(modelStack);
		modelStack.Translate(worldObjects[5].position.x, worldObjects[5].position.y, worldObjects[5].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[5].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(.13f, .13f, .13f);
		RenderMesh(meshList_hub[GEO_TABLE], true);
		modelStack.Scale(worldObjects[5].boundingBox.getWidth(), worldObjects[5].boundingBox.getHeight(), worldObjects[5].boundingBox.getDepth());
		//RenderMesh(meshList_hub[GEO_WALL], true);
	}

	{
		PushPop foodstand(modelStack);
		modelStack.Translate(worldObjects[7].position.x, worldObjects[7].position.y, worldObjects[7].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[7].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(.12f, .12f, .12f);
		RenderMesh(meshList_hub[GEO_FOODSTAND], true);
		//modelStack.Scale(debugScale, debugScale, debugScale);
		//modelStack.Scale(worldObjects[7].boundingBox.getWidth(), worldObjects[7].boundingBox.getHeight(), worldObjects[7].boundingBox.getDepth());
		//RenderMesh(meshList_hub[GEO_WALL], true);
	}

	{
		PushPop ferriswheel(modelStack);
		modelStack.Translate(worldObjects[6].position.x, worldObjects[6].position.y, worldObjects[6].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[6].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(.2f, .2f, .2f);
		RenderMesh(meshList_hub[GEO_FERRISWHEEL], true);
	}

	for (int i = 1; i < 5; i++)
	{
		PushPop stall(modelStack);
		modelStack.Translate(worldObjects[i].position.x, worldObjects[i].position.y + 0.8f, worldObjects[i].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[i].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(0.2f, 0.2f, 0.2f);
		RenderMesh(meshList_hub[GEO_STALL], true);
	}

	//{
	//	PushPop fountain(modelStack);
	//	modelStack.Translate(Fountain.position.x, Fountain.position.y, Fountain.position.z);
	//	modelStack.Scale(1.5f, 1.5f, 1.5f);
	//	RenderMesh(meshList[GEO_FOUNTAIN], true);
	//}

	{
		PushPop monkey(modelStack);
		modelStack.Translate(-5.6f, 0.5f, 0.8f);
		modelStack.Rotate(90, 0.f, 1.f, 0.f);
		modelStack.Scale(0.1f, 0.1f, 0.1f);
		RenderMesh(meshList[GEO_MONKEY], true);
	}
	{
		PushPop Maxwell(modelStack);
		modelStack.Translate(maxwell.position.x, maxwell.position.y, maxwell.position.z);
		modelStack.MultMatrix(glm::mat4_cast(maxwell.orientation));
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList_hub[GEO_MAXWELL], false);
	}


	{
		PushPop wallGuard(modelStack);
		modelStack.Translate(worldObjects[0].position.x, worldObjects[0].position.y, worldObjects[0].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[0].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(worldObjects[0].boundingBox.getWidth(), worldObjects[0].boundingBox.getHeight(), worldObjects[0].boundingBox.getDepth());
		//modelStack.Scale(5.f, 0.1f, 5.f);

		meshList_hub[GEO_WALL]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList_hub[GEO_WALL]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		meshList_hub[GEO_WALL]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
		meshList_hub[GEO_WALL]->material.kShininess = 1.0f;

		//RenderMesh(meshList_hub[GEO_WALL], true);
	}
	for (int i = 10; i < 16; i++) {
		PushPop cans(modelStack);
		modelStack.Translate(worldObjects[i].position.x, worldObjects[i].position.y, worldObjects[i].position.z);
		glm::mat4 rotation = glm::mat4_cast(worldObjects[i].orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(0.15f, 0.15f, 0.15f);
		meshList_hub[GEO_PEPSI]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList_hub[GEO_PEPSI]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		meshList_hub[GEO_PEPSI]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
		meshList_hub[GEO_PEPSI]->material.kShininess = 1.0f;
		RenderMesh(meshList_hub[GEO_PEPSI], true);
	}

	{
		if (temp) {
			for (int i = 0; i < TOTAL_PICKABLES; i++) {
				if (pickables[i] != nullptr) {
					PushPop yes(modelStack);
					modelStack.Translate(pickables[i]->body.position.x, pickables[i]->body.position.y, pickables[i]->body.position.z);
					glm::mat4 rotation = glm::mat4_cast(pickables[i]->body.orientation);
					modelStack.MultMatrix(rotation);
					modelStack.Scale(pickables[i]->body.boundingBox.getWidth(), pickables[i]->body.boundingBox.getHeight(), pickables[i]->body.boundingBox.getDepth());
					//modelStack.Scale(5.f, 0.1f, 5.f);

					meshList_hub[GEO_CANSPEPSI]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
					meshList_hub[GEO_CANSPEPSI]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
					meshList_hub[GEO_CANSPEPSI]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
					meshList_hub[GEO_CANSPEPSI]->material.kShininess = 1.0f;

					RenderMesh(meshList_hub[GEO_CANSPEPSI], true);
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
			itemInHand->body.SetOrientation(-pitch, yaw, 0);
		}
		// Render cans
		for (int i = 0; i < numOfCansInPlay; i++){
			PushPop cansGuard(modelStack);
			modelStack.Translate(cans[i].position.x, cans[i].position.y, cans[i].position.z);
			glm::mat4 rotation = glm::mat4_cast(cans[i].orientation);
			modelStack.MultMatrix(rotation);
			modelStack.Scale(0.15f, 0.15f, 0.15f);
			meshList_hub[GEO_PEPSI]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
			meshList_hub[GEO_PEPSI]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			meshList_hub[GEO_PEPSI]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
			meshList_hub[GEO_PEPSI]->material.kShininess = 1.0f;
			RenderMesh(meshList_hub[GEO_PEPSI], true);
		}

		{
			// Render board
			PushPop board(modelStack);
			modelStack.Translate(worldObjects[9].position.x, worldObjects[9].position.y, worldObjects[9].position.z);
			glm::mat4 rotation = glm::mat4_cast(worldObjects[9].orientation);
			modelStack.MultMatrix(rotation);
			modelStack.Scale(0.2f, 0.2f, 0.2f);
			RenderMesh(meshList_hub[GEO_BOARD], true);
		}

		// Render pickable items
		for (int i = 0; i < TOTAL_PICKABLES; i++) {
			if (pickables[i] != nullptr) {
				modelStack.PushMatrix();
				modelStack.Translate(pickables[i]->body.position.x, pickables[i]->body.position.y, pickables[i]->body.position.z);
				glm::mat4 rotation = glm::mat4_cast(pickables[i]->body.orientation);
				modelStack.MultMatrix(rotation);

				if (pickables[i]->name == "Baseball") {
					modelStack.Scale(0.15f, 0.15f, 0.15f);
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

				modelStack.PopMatrix();
			}
		}
	}


	{
		// Render Dialogue
		//std::cout << std::to_string(dialogueFadeHUD.getPosition().y) << std::endl;
		RenderMeshOnScreen(meshList[GEO_DIALOGUEFADE_GUI], dialogueFadeHUD.getPosition().x, dialogueFadeHUD.getPosition().y, 1600, 900);

		switch (part) {
		case 1: // part 1
			switch (phase) {
			case 0:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Welcome to the can knocking challenge, here you will be knocking cans over with a baseball.", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;
			case 1:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Knock down all the cans without picking up the baseballs", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;
			case 2:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Only then you can leave the game!!", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
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

void SceneKnockdown::RenderUI()
{
	{
		// Render GUI
		//RenderMeshOnScreen(meshList[GEO_MENU_GUI], 0, 0, 1600, 900);
		//RenderMeshOnScreen(meshList[GEO_SWITCHSCENE_GUI], 0, 0, 1600, 900);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (interactedIndex != -1) {
			RenderMeshOnScreen(meshList[GEO_CROSSHAIROPAQUE_GUI], crosshair.getPosition().x, crosshair.getPosition().y, 1600, 900);

			RenderMeshOnScreen(meshList[GEO_INTERACTFADE_GUI], interactPrompt.getPosition().x, interactPrompt.getPosition().y, 1600, 900);
			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], interactives[interactedIndex], glm::vec3(1, 1, 1), 20, 410 + interactPrompt.getPosition().x * 1.5f, -10 + interactPrompt.getPosition().y, 'R', .6f);

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

		RenderTextOnScreen(meshList[GEO_CARNIVALEEFREAKSHOW_FONT], "SCORE", glm::vec3(0, 1, 0), 45, -795, 400, 'L', .6f);

		if (itemInHand != nullptr) {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			RenderMeshOnScreen(meshList[GEO_ITEMINHANDBORDER_GUI], 603 - (itemInHandHUD.getScale().x * .5f), -343.2f + (itemInHandHUD.getScale().y * .5f), 214 + itemInHandHUD.getScale().x, 33.7f + itemInHandHUD.getScale().y);
			RenderMeshOnScreen(meshList[GEO_ITEMINHANDFADEBACKGROUND_GUI], 603 - (itemInHandHUD.getScale().x * .5f), -343.2f + (itemInHandHUD.getScale().y * .5f), 214 + itemInHandHUD.getScale().x, 33.7f + itemInHandHUD.getScale().y);
			RenderMeshOnScreen(meshList[GEO_ITEMINHANDFADE_GUI], 0, 0 + itemInHandHUD.getScale().y, 1600, 900);

			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "(" + std::to_string(amountOfItem) + "x) " + itemInHand->name, glm::vec3(1, 1, 1), 20, 690, -355 + itemInHandHUD.getScale().y, 'R', .6f);

			if (itemInUse) {
				RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "[E]", glm::vec3(1, 1, 1), 15, 700, -300 + itemInHandHUD.getScale().y, 'R', .6f);
			}
			else {
				RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[E]", glm::vec3(1, 1, 1), 15, 700, -300 + itemInHandHUD.getScale().y, 'R', .6f);
			}
			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "Use", glm::vec3(1, 1, 1), 15, 660, -300 + itemInHandHUD.getScale().y, 'R', .6f);

			RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[X]", glm::vec3(1, 1, 1), 15, 700, -320 + itemInHandHUD.getScale().y, 'R', .6f);
			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "Drop", glm::vec3(1, 1, 1), 15, 660, -320 + itemInHandHUD.getScale().y, 'R', .6f);
		}

		// DEBUG
		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "PART: " + std::to_string(part) + " PHASE: " + std::to_string(phase), glm::vec3(1, 1, 1), 15, 785, 435, 'R', .6f);
		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "DT MULTIPLIER: " + std::to_string(dtMultiplier), glm::vec3(1, 1, 1), 15, 595, 435, 'R', .6f);
		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "ACTIVE GRASS COUNT: " + std::to_string(activeGrassCount), glm::vec3(1, 1, 1), 15, 375, 435, 'R', .6f);
		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "FPS: " + std::to_string(currentFPS), glm::vec3(1, 1, 1), 15, 140, 435, 'R', .6f);
	}

	// Render EUI
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

	{
		// Render Switch Scene UI
		if (nextScene != 0) {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			RenderMeshOnScreen(meshList[GEO_SWITCHSCENE_GUI], 0.f, 0.f, 1200 * sceneSwitchHUD.getScale().x, 675 * sceneSwitchHUD.getScale().x);
		}
	}
}

void SceneKnockdown::Exit()
{
	BaseScene::Exit();

	AudioManager::Instance().SoundStop("Maxwell");
}

void SceneKnockdown::RegenerateGrassPositions()
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

void SceneKnockdown::UpdateGrassDensity(double dt)
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


void SceneKnockdown::generateCanPositions(int pattern) {
	// Pattern 1 - 2 tier Pyramid
	// Pattern 2 - 3 tier Pyramid
	// Pattern 3 - 3 rows
	glm::vec3 worldPosOffset(-6.6f, 0.7f, 0);
	glm::vec3 canSize(.09f, .18f, .09f);
	bool settings[2] = { true , true };
	switch (pattern) {
	case 1:
		cans[0].InitPhysicsObject(glm::vec3(-6.4, 0.8f, 0.1f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[1].InitPhysicsObject(glm::vec3(-6.4, 0.8f, -0.1f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[2].InitPhysicsObject(glm::vec3(-6.4, 0.8f, 0.f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[3].InitPhysicsObject(glm::vec3(-6.4, 0.98f, 0.05f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[4].InitPhysicsObject(glm::vec3(-6.4, 0.98f, -0.05f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[5].InitPhysicsObject(glm::vec3(-6.4, 1.16f, 0.f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		numOfCansInPlay = 6;
		break;
	case 2:
		// Square pattern (3x3)
		cans[0].InitPhysicsObject(glm::vec3(-6.4, 0.8f, 0.1f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[1].InitPhysicsObject(glm::vec3(-6.4, 0.8f, 0.f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[2].InitPhysicsObject(glm::vec3(-6.4, 0.8f, -0.1f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[3].InitPhysicsObject(glm::vec3(-6.4, 0.98f, 0.1f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[4].InitPhysicsObject(glm::vec3(-6.4, 0.98f, 0.f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[5].InitPhysicsObject(glm::vec3(-6.4, 0.98f, -0.1f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[6].InitPhysicsObject(glm::vec3(-6.4, 1.16f, 0.1f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[7].InitPhysicsObject(glm::vec3(-6.4, 1.16f, 0.f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[8].InitPhysicsObject(glm::vec3(-6.4, 1.16f, -0.1f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		numOfCansInPlay = 9;
		break;
	case 3:
		// 2 tower pattern, 2 columns of 3 with 1 can-spacing
		cans[0].InitPhysicsObject(glm::vec3(-6.4, 0.8f, 0.2f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[1].InitPhysicsObject(glm::vec3(-6.4, 0.98f, 0.2f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[2].InitPhysicsObject(glm::vec3(-6.4, 1.16f, 0.2f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[3].InitPhysicsObject(glm::vec3(-6.4, 0.8f, -0.2f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[4].InitPhysicsObject(glm::vec3(-6.4, 0.98f, -0.2f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		cans[5].InitPhysicsObject(glm::vec3(-6.4, 1.16f, -0.2f), 5.f, BoundingBox::Type::OBB, canSize, settings);
		numOfCansInPlay = 6;
		break;
	default:
		break;
	}

	startPhysicsUpdateForCans = false;

	for (int i = 0; i < numOfCansInPlay; i++) {
		winCondition[i] = false;
	}
}