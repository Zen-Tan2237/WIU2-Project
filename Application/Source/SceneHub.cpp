// GLM Headers
#include "GL\glew.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>

#include "SceneHub.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"

#include "KeyboardController.h"
#include "MouseController.h"

SceneHub::SceneHub()
{
}

SceneHub::~SceneHub()
{
}

void SceneHub::Init()
{
	BaseScene::Init();

	//light
	{
		light[0].position = glm::vec3(0, 5, 0);
		light[0].color = glm::vec3(1, 1, 1);
		light[0].type = Light::DIRECTIONAL;
		light[0].power = 0.3f;
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
		light[1].power = 0;
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
		light[2].power = 1;
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
		light[3].power = 1;
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
		light[4].power = 1;
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
		light[5].power = 1;
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

	meshList_hub[GEO_WALL] = MeshBuilder::GenerateCube("wall", glm::vec3(1.f, 0.f, 0.f), 1.f);

	meshList_hub[GEO_FOUNTAIN] = MeshBuilder::GenerateOBJ("fountain", "Models//Fountain.obj");
	meshList_hub[GEO_FOUNTAIN]->textureID = LoadTGA("Textures//Fountain.tga");

	meshList_hub[GEO_STALL] = MeshBuilder::GenerateOBJ("stall", "Models//minigame_Stall.obj");
	meshList_hub[GEO_STALL]->textureID = LoadTGA("Textures//minigameStall.tga");

	meshList_hub[GEO_TABLE] = MeshBuilder::GenerateOBJ("table", "Models//table.obj");
	meshList_hub[GEO_TABLE]->textureID = LoadTGA("Textures//table.tga");

	meshList_hub[GEO_FOODSTAND] = MeshBuilder::GenerateOBJ("foodstand", "Models//Hot_Dog_Stand.obj");
	meshList_hub[GEO_FOODSTAND]->textureID = LoadTGA("Textures//Hotdog.tga");

	meshList_hub[GEO_FERRISWHEEL] = MeshBuilder::GenerateOBJ("ferriswheel", "Models//FerrisWheel.obj");
	meshList_hub[GEO_FERRISWHEEL]->textureID = LoadTGA("Textures//FerrisWheel.tga");

	meshList_hub[GEO_MONKEY] = MeshBuilder::GenerateOBJ("monkey", "Models//Monkey.obj");

	//collectibles
	meshList_hub[GEO_FIGURINE] = MeshBuilder::GenerateOBJ("figurine", "Models//Figurine.obj");
	meshList_hub[GEO_FIGURINE]->textureID = LoadTGA("Textures//Figurine.tga");

	meshList_hub[GEO_PIG] = MeshBuilder::GenerateOBJ("pig", "Models//Pig.obj");
	meshList_hub[GEO_PIG]->textureID = LoadTGA("Textures//Pig.tga");

	meshList_hub[GEO_PLUSHIE] = MeshBuilder::GenerateOBJ("plushie", "Models//Plushie.obj");
	meshList_hub[GEO_PLUSHIE]->textureID = LoadTGA("Textures//Frieren_Plushie.tga");	

	meshList_hub[GEO_5090] = MeshBuilder::GenerateOBJ("5090", "Models//RTX5090_BOX.obj");
	meshList_hub[GEO_5090]->textureID = LoadTGA("Textures//RTX5090_BOX.tga");


	// setup initial item in hand
	addPickables("Baseball", glm::vec3(0, 0, 0));
	itemInHand = pickables[0];
	amountOfItem = 10;
	previousItemInHandName = "";
	itemInUse = false;

	// setup phase durations here ([first one is part][second one is phase]. phase means like u want a constant stream of dialgoues
	// make sure whenver u do part++, u have like (if part == <the number they should be at>) then part++
	phaseDurations[0][0] = 6.7f;
	phaseDurations[0][1] = 6.7f;
	phaseDurations[0][2] = 6.7f;

	// world objects
	bool miscSettings[2] = { false, false }; // for gravity and drag. override in case of specific objects
	PhysicsObject wall;

	wall.InitPhysicsObject(glm::vec3(0, 0, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(1, 1, 1), 45, glm::vec3(1, 0, 0), miscSettings);

	worldObjects[0] = wall;

	addPickables("Pepsi", glm::vec3(3, 1, 2));

	meshList_hub[GEO_FOUNTAIN]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_FOUNTAIN]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_FOUNTAIN]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_FOUNTAIN]->material.kShininess = 1.0f;

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

	/*meshList_hub[GEO_MONKEY]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_MONKEY]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_MONKEY]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_MONKEY]->material.kShininess = 1.0f;*/

	meshList_hub[GEO_FIGURINE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_FIGURINE]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_FIGURINE]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_FIGURINE]->material.kShininess = 1.0f;

	meshList_hub[GEO_PIG]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_PIG]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_PIG]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_PIG]->material.kShininess = 1.0f;

	meshList_hub[GEO_FERRISWHEEL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_FERRISWHEEL]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_FERRISWHEEL]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_FERRISWHEEL]->material.kShininess = 1.0f;

	meshList_hub[GEO_PLUSHIE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_PLUSHIE]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_PLUSHIE]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_PLUSHIE]->material.kShininess = 1.0f;

	meshList_hub[GEO_5090]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList_hub[GEO_5090]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList_hub[GEO_5090]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList_hub[GEO_5090]->material.kShininess = 1.0f;
}

void SceneHub::Update(double dt)
{
	BaseScene::Update(dt);

	// name of interactive, I = interactive, coords
	addInteractives("Enter Scene 2 (SceneHub)", 'I', glm::vec3(1, 0, 0));
	addInteractives("1", 'I', glm::vec3(-1, 0, 0));
	addInteractives("2", 'I', glm::vec3(0, 0, 1));
	addInteractives("3", 'I', glm::vec3(0, 0, -1));

	addInteractives("4", 'I', glm::vec3(0.f, 1.f, 0.f));

	//addPickables("Halal Pork", glm::vec3(0, 0, 0));
	initializePickablesInteractives();
	getClosestInteractive();


	// handle what type of interactive, what type of event
	if (interactedIndex != -1 && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_F)) { // means got prompt, is close to and facing smth
		if (interactivesType[interactedIndex] == 'I') { // its an interactive
			// do it in actual scene instead
			if (interactives[interactedIndex] == "Enter Scene 2 (SceneHub)" && nextScene == 0) {
				nextScene = 2;
				nextSceneDelay = 1.f;
				sceneSwitchHUD.resetScale(glm::vec2(.25f));
				sceneSwitchHUD.setTargetScale(glm::vec2(1.f));
			}
			else if (interactives[interactedIndex] == "1") {
				if (part == 0)
				{
					addPickables("Pepsi", glm::vec3(.5, 5, 0));
				}
			}
			else if (interactives[interactedIndex] == "2") {
				// do something
			}
			else if (interactives[interactedIndex] == "3") {
				// do something
			}
			else if (interactives[interactedIndex] == "4") {
				// do something
			}
		}
	}

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
	std::cout << "Debug Pos: " << debugPos.x << ", " << debugPos.y << ", " << debugPos.z << std::endl;
}

void SceneHub::Render()
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

	{
		PushPop multi(modelStack);
		modelStack.Scale(0.2f, 0.2f, 0.2f);

		{
			PushPop backgroundBuildings(modelStack);
			modelStack.Translate(0.f, -15.f, 0.f);
			modelStack.Scale(1.f, 2.f, 1.f);
			RenderMesh(meshList[GEO_BACKGROUND_BUILDINGS1], true);
			RenderMesh(meshList[GEO_BACKGROUND_BUILDINGS2], true);
		}

		{
			PushPop floor(modelStack);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_FLOOR], true);
		}

		{
			PushPop fence(modelStack);
			RenderMesh(meshList[GEO_FENCE], true);
		}

		{
			PushPop table(modelStack);
			RenderMesh(meshList_hub[GEO_TABLE], true);
		}

		{
			PushPop foodstand(modelStack);
			RenderMesh(meshList_hub[GEO_FOODSTAND], true);
		}

		{
			PushPop ferriswheel(modelStack);
			modelStack.Translate(debugPos.x, debugPos.y, debugPos.z);
			RenderMesh(meshList_hub[GEO_FERRISWHEEL], true);
		}

		{
			PushPop stall(modelStack);
			modelStack.Translate(-30.f, 0.f, 0.f);
			RenderMesh(meshList_hub[GEO_STALL], true);
		}
		{
			PushPop stall(modelStack);
			modelStack.Translate(30.f, 0.f, 0.f);
			modelStack.Rotate(90, 0.f, 1.f, 0.f);
			RenderMesh(meshList_hub[GEO_STALL], true);
		}
		{
			PushPop stall(modelStack);
			modelStack.Translate(0.f, 0.f, -30.f);
			RenderMesh(meshList_hub[GEO_STALL], true);
		}
		{
			PushPop stall(modelStack);
			modelStack.Translate(0.f, 0.f, 30.f);
			RenderMesh(meshList_hub[GEO_STALL], true);
		}

		{
			PushPop monkey(modelStack);
			RenderMesh(meshList_hub[GEO_MONKEY], true);
		}

		{
			PushPop shift(modelStack);
			modelStack.Translate(5.f, 3.f, 0.f);
			{
				PushPop figurine(modelStack);
				RenderMesh(meshList_hub[GEO_FIGURINE], true);
			}

			{
				PushPop pig(modelStack);
				//RenderMesh(meshList_hub[GEO_PIG], true);
			}

			{
				PushPop plushie(modelStack);
				//RenderMesh(meshList_hub[GEO_PLUSHIE], true);
			}

			{
				PushPop rtx(modelStack);
				//RenderMesh(meshList_hub[GEO_5090], true);
			}
		}
	}


	{
		PushPop fountain(modelStack);
		//RenderMesh(meshList_hub[GEO_FOUNTAIN], true);
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
		switch (part) {
		case 0: // part 1
			switch (phase) {
			case 0:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "Welcome to our crooked carnival, today you will be earning points through our various minigames and challenges.", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;
			case 1:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "you can redeem your points for fantastic prizes such as, a anime figure or A GEFORCE RTX 5090!", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
				break;
			case 2:
				RenderTextOnScreen(meshList[GEO_MINGLIUEXTB_FONT], "But be careful, if you lose all your points, you will be trapped here forever!", glm::vec3(1, 1, 1), 20, 0, -380, 'C', .6f);
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

void SceneHub::RenderUI()
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

		// Debug
		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "PART: " + std::to_string(part) + " PHASE: " + std::to_string(phase), glm::vec3(1, 1, 1), 15, 0, 435, 'C', .6f);
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

void SceneHub::Exit()
{
	BaseScene::Exit();
}