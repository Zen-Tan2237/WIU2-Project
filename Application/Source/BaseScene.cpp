#include "BaseScene.h"
#include "GL\glew.h"
#include <iostream>

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "LoadTGA.h"

BaseScene::BaseScene()
{
}

BaseScene::~BaseScene()
{
}

void BaseScene::Init()
{
	nextScene = 0;

	//call base scene stuff when current file is inherited from it
	// BaseScene::Init();

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);


	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	glUseProgram(m_programID);


	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");
	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
	m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
	m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
	m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
	m_parameters[U_LIGHT5_TYPE] = glGetUniformLocation(m_programID, "lights[5].type");
	m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
	m_parameters[U_LIGHT5_COLOR] = glGetUniformLocation(m_programID, "lights[5].color");
	m_parameters[U_LIGHT5_POWER] = glGetUniformLocation(m_programID, "lights[5].power");
	m_parameters[U_LIGHT5_KC] = glGetUniformLocation(m_programID, "lights[5].kC");
	m_parameters[U_LIGHT5_KL] = glGetUniformLocation(m_programID, "lights[5].kL");
	m_parameters[U_LIGHT5_KQ] = glGetUniformLocation(m_programID, "lights[5].kQ");
	m_parameters[U_LIGHT5_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[5].spotDirection");
	m_parameters[U_LIGHT5_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[5].cosCutoff");
	m_parameters[U_LIGHT5_COSINNER] = glGetUniformLocation(m_programID, "lights[5].cosInner");
	m_parameters[U_LIGHT6_TYPE] = glGetUniformLocation(m_programID, "lights[6].type");
	m_parameters[U_LIGHT6_POSITION] = glGetUniformLocation(m_programID, "lights[6].position_cameraspace");
	m_parameters[U_LIGHT6_COLOR] = glGetUniformLocation(m_programID, "lights[6].color");
	m_parameters[U_LIGHT6_POWER] = glGetUniformLocation(m_programID, "lights[6].power");
	m_parameters[U_LIGHT6_KC] = glGetUniformLocation(m_programID, "lights[6].kC");
	m_parameters[U_LIGHT6_KL] = glGetUniformLocation(m_programID, "lights[6].kL");
	m_parameters[U_LIGHT6_KQ] = glGetUniformLocation(m_programID, "lights[6].kQ");
	m_parameters[U_LIGHT6_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[6].spotDirection");
	m_parameters[U_LIGHT6_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[6].cosCutoff");
	m_parameters[U_LIGHT6_COSINNER] = glGetUniformLocation(m_programID, "lights[6].cosInner");
	m_parameters[U_LIGHT7_TYPE] = glGetUniformLocation(m_programID, "lights[7].type");
	m_parameters[U_LIGHT7_POSITION] = glGetUniformLocation(m_programID, "lights[7].position_cameraspace");
	m_parameters[U_LIGHT7_COLOR] = glGetUniformLocation(m_programID, "lights[7].color");
	m_parameters[U_LIGHT7_POWER] = glGetUniformLocation(m_programID, "lights[7].power");
	m_parameters[U_LIGHT7_KC] = glGetUniformLocation(m_programID, "lights[7].kC");
	m_parameters[U_LIGHT7_KL] = glGetUniformLocation(m_programID, "lights[7].kL");
	m_parameters[U_LIGHT7_KQ] = glGetUniformLocation(m_programID, "lights[7].kQ");
	m_parameters[U_LIGHT7_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[7].spotDirection");
	m_parameters[U_LIGHT7_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[7].cosCutoff");
	m_parameters[U_LIGHT7_COSINNER] = glGetUniformLocation(m_programID, "lights[7].cosInner");
	m_parameters[U_LIGHT7_EXPONENT] = glGetUniformLocation(m_programID, "lights[7].exponent");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);

	// Initialise camera properties
	setCameraOrigin(glm::vec3(0.f, 1.f, -1.f), glm::vec3(0.f, 1.f, 1.f), glm::vec3(0.f, 2.f, -1.f));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	//demo for obj files
	//meshList[GEO_LEFT] = meshBuilder.LoadOBJMTL("Left", "OBJ//left.obj", "OBJ//left.mtl");
	//meshList[GEO_LEFT] = MeshBuilder::LoadOBJ("Left", "OBJ//left.obj");
	//meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);

	// GUI
	meshList[GEO_MENU_GUI] = MeshBuilder::GenerateQuad("Menu GUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_MENU_GUI]->textureID = LoadTGA("Image//Menu_GUI.tga");

	meshList[GEO_SWITCHSCENE_GUI] = MeshBuilder::GenerateQuad("Switch Scene GUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_SWITCHSCENE_GUI]->textureID = LoadTGA("Image//SwitchScene_GUI.tga");

	meshList[GEO_INTERACTFADE_GUI] = MeshBuilder::GenerateQuad("Interact Fade GUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_INTERACTFADE_GUI]->textureID = LoadTGA("Image//InteractFade_GUI.tga");

	meshList[GEO_CROSSHAIRTRANSLUCENT_GUI] = MeshBuilder::GenerateQuad("Crosshair Translucent GUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_CROSSHAIRTRANSLUCENT_GUI]->textureID = LoadTGA("Image//CrosshairTranslucent_GUI.tga");
	meshList[GEO_CROSSHAIROPAQUE_GUI] = MeshBuilder::GenerateQuad("Crosshair Opaque GUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_CROSSHAIROPAQUE_GUI]->textureID = LoadTGA("Image//CrosshairOpaque_GUI.tga");

	meshList[GEO_ITEMINHANDFADE_GUI] = MeshBuilder::GenerateQuad("ItemInHand Fade GUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_ITEMINHANDFADE_GUI]->textureID = LoadTGA("Image//ItemInHandFade_GUI.tga");

	meshList[GEO_ITEMINHANDBORDER_GUI] = MeshBuilder::GenerateQuad("ItemInHand Border GUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_ITEMINHANDBORDER_GUI]->textureID = LoadTGA("Image//ItemInHandBorder_GUI.tga");

	meshList[GEO_ITEMINHANDFADEBACKGROUND_GUI] = MeshBuilder::GenerateQuad("ItemInHand FadeBackground GUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_ITEMINHANDFADEBACKGROUND_GUI]->textureID = LoadTGA("Image//ItemInHandFadeBackground_GUI.tga");


	// EUI
	meshList[GEO_INTERACT_EUI] = MeshBuilder::GenerateQuad("Interact EUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_INTERACT_EUI]->textureID = LoadTGA("Image//Interact_EUI.tga");

	meshList[GEO_INTERACTED_EUI] = MeshBuilder::GenerateQuad("Interacted EUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_INTERACTED_EUI]->textureID = LoadTGA("Image//Interacted_EUI.tga");

	// FONTS
	meshList[GEO_CARNIVALEEFREAKSHOW_FONT] = MeshBuilder::GenerateText("Carnivalee Freakshow Font", 16, 16);
	meshList[GEO_CARNIVALEEFREAKSHOW_FONT]->textureID = LoadTGA("Fonts//CarnivaleeFreakshow.tga");

	meshList[GEO_SATOSHIREGULAR_FONT] = MeshBuilder::GenerateText("Satoshi Regular Font", 16, 16);
	meshList[GEO_SATOSHIREGULAR_FONT]->textureID = LoadTGA("Fonts//Satoshi_Regular.tga");

	meshList[GEO_HOMEVIDEO_FONT] = MeshBuilder::GenerateText("Home Video Font", 16, 16);
	meshList[GEO_HOMEVIDEO_FONT]->textureID = LoadTGA("Fonts//HomeVideo.tga");

	meshList[GEO_HOMEVIDEOBOLD_FONT] = MeshBuilder::GenerateText("Home Video Bold Font", 16, 16);
	meshList[GEO_HOMEVIDEOBOLD_FONT]->textureID = LoadTGA("Fonts//HomeVideoBold.tga");

	meshList[GEO_VCROSDMONO_FONT] = MeshBuilder::GenerateText("VCROSD Mono Font", 16, 16);
	meshList[GEO_VCROSDMONO_FONT]->textureID = LoadTGA("Fonts//VCROSDMono.tga");

	meshList[GEO_MINGLIUEXTB_FONT] = MeshBuilder::GenerateText("MingLiuExtB Font", 16, 16);
	meshList[GEO_MINGLIUEXTB_FONT]->textureID = LoadTGA("Fonts//MingLiuExtB.tga");


	glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);


	glUniform1i(m_parameters[U_NUMLIGHTS], TOTAL_LIGHTS);


	enableLight = true;

	// INTERACTIVES
	for (int i = 0; i < TOTAL_INTERACTIVES; i++) {
		interactives[i] = "";
		interactivesPos[i] = glm::vec3(0, 0, 0);
		interactivePickablesIndex[i] = 0;
	}

	for (int i = 0; i < TOTAL_PICKABLES; i++) {
		pickables[i] = "";
		pickablesPos[i] = glm::vec3(0, 0, 0);
	}

	noOfInteractives = 0;
	noOfPickables = 0;

	interactedEUI_scale = 0.05f;
	interactedEUI_targetScale = 0.1f;

	interactGUI_positionOffset = glm::vec2(-25.f, 0);
	interactGUI_targetPositionOffset = glm::vec2(0, 0);

	itemInHand = "Monkey";
	amountOfItem = 10;
	previousItemInHand = "";
	itemInUse = false;

	itemInHandElapsed = 0.f;

	itemInHandGUI_scaleOffset = glm::vec3(0, 0, 0);
	itemInHandGUI_targetScaleOffset = glm::vec3(0, 0, 0);

	dropKeybindHeldElapsed = 0.f;
	droppedFirst = false;

	sceneSwitchUI_scalePercentage = 0.35f;
	sceneSwitchUI_targetScalePercentage = 0.35f;

	// DIALGOUE

	oldPart = 0;
	part = 0;
	oldPhase = 0;
	phase = 0;

	for (int k = 0; k < TOTAL_PARTS; k++) {
		for (int i = 0; i < TOTAL_PHASES; i++) {
			phaseDurations[k][i] = 0.f;
		}
	}
}

void BaseScene::Update(double dt)
{
	HandleKeyPress(dt);

	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		light[0].position.z -= static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		light[0].position.z += static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		light[0].position.x -= static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		light[0].position.x += static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		light[0].position.y -= static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		light[0].position.y += static_cast<float>(dt) * 5.f;

	// CAMERA BOBBING
	camera.position -= previousBobOffset;
	camera.target -= previousBobOffset;

	glm::vec3 currentPlayerPosition = camera.position;
	glm::vec3 delta = currentPlayerPosition - previousPlayerPosition;
	delta.y = 0.0f;

	previousPlayerPosition = currentPlayerPosition;

	float distanceMoved = glm::length(delta);
	bobDistanceAccumulated += distanceMoved;

	bool isMoving = (distanceMoved > 0.0001f);

	float targetWeight = isMoving ? 1.0f : 0.0f;
	currentBobWeight += (targetWeight - currentBobWeight) * (1.0f - exp(-10.0f * dt));

	float wave = bobDistanceAccumulated * bobFrequency;

	float verticalOffset = sinf(wave) * bobAmplitudeVertical;
	float horizontalOffset = sinf(wave * 0.5f) * bobAmplitudeHorizontal;

	verticalOffset *= currentBobWeight;
	horizontalOffset *= currentBobWeight;

	glm::vec3 forward = glm::normalize(camera.target - camera.position);
	glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));

	glm::vec3 currentBobOffset =
		camera.up * verticalOffset +
		right * horizontalOffset;

	camera.position += currentBobOffset;
	camera.target += currentBobOffset;

	previousBobOffset = currentBobOffset;

	if (nextScene == 0) {
		camera.Update(dt);
	}

	//

	float temp = 1.f / dt;
	fps = glm::round(temp * 100.f) / 100.f;

	//
	if (itemInHand != previousItemInHand) {
		previousItemInHand = itemInHand;
		if (itemInHand != "") {
			itemInHandElapsed = 0.f;
		}
	}

	if (itemInHand != "") {
		itemInHandElapsed += dt;
	}

	if (itemInHand != "" && itemInHandElapsed < 2.f) {
		itemInHandGUI_targetScaleOffset = glm::vec3(20.f, 200.f, 0);
	}
	else {
		itemInHandGUI_targetScaleOffset = glm::vec3(0, 0.f, 0);
	}

	//
	float t1 = 1.f - std::exp(-5 * dt);
	float t2 = 1.f - std::exp(-10 * dt);
	interactedEUI_scale += (interactedEUI_targetScale - interactedEUI_scale) * t1;
	interactGUI_positionOffset += (interactGUI_targetPositionOffset - interactGUI_positionOffset) * t2;
	itemInHandGUI_scaleOffset += (itemInHandGUI_targetScaleOffset - itemInHandGUI_scaleOffset) * t2;
	sceneSwitchUI_scalePercentage += (sceneSwitchUI_targetScalePercentage - sceneSwitchUI_scalePercentage) * t2;

	resetInteractives();

	//
	if (nextSceneDelay > 0.f) {
		nextSceneDelay -= dt;
	}
	else {
		nextSceneDelay = 0.f;
	}

	//
	if (oldPart != part) {
		oldPart = part;
	}
	if (oldPhase != phase) {
		oldPhase = phase;
	}

	if (currPhaseElapsed >= phaseDurations[part][phase]) {
		if (phase + 1 <= TOTAL_PHASES - 1) {
			phase++;
		}
		
		currPhaseElapsed = 0.f;
	}
	else {
		currPhaseElapsed += dt;
	}
}

void BaseScene::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
		RenderMesh(meshList[GEO_AXES], false);
	}


	{
		PushPop skybox(modelStack);
		modelStack.Translate(0, 0, 0); //later change to follow player class
		modelStack.Scale(20.f, 20.f, 20.f);
		//RenderSkybox();
	}
}

void BaseScene::RenderMesh(Mesh* mesh, bool enableLight)
{
	glm::mat4 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, glm::value_ptr(modelView));
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = glm::inverseTranspose(modelView);
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, glm::value_ptr(modelView_inverse_transpose));

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render();

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void BaseScene::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void BaseScene::HandleKeyPress(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	//if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	//{
	//	// Change to black background
	//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		// Toggle light on or off
	/*	enableLight = !enableLight;*/

		if (light[0].power <= 0.1f)
			light[0].power = 1.f;
		else
			light[0].power = 0.1f;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	}

	//////////////////////////////////////////
	// INTERACTIVES HANDLER
	if (interactedIndex != -1 && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_F)) { // means got prompt, is close to and facing smth
		if (interactivesType[interactedIndex] == 'I') { // its an interactive
			// do it in actual scene instead
		}
		else if (interactivesType[interactedIndex] == 'P') { // its a pickable
			addItemInHand();
		}
	}

	// HOLD ITEM HANDLER
	if (itemInHand != "") { // hand has stuff
		if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
		{
			if (itemInUse) {
				itemInUse = false;
			}
			else {
				itemInUse = true;
			}
		}

		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_X))
		{
			dropKeybindHeldElapsed += dt;
			
			if (dropKeybindHeldElapsed < .5f) {
				if (droppedFirst == false) {
					dropItemInHand(1);
					droppedFirst = true;
				}
			}
			else {
				dropItemInHand(1);
			}
			
		}
		else {
			dropKeybindHeldElapsed = 0.f;
			droppedFirst = false;
		}
	}
}

void BaseScene::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizeX, float sizeY)
{
	glDisable(GL_DEPTH_TEST);
	//Change to orthographic mode
	{
		projectionStack.PushMatrix();
		glm::mat4 ortho = glm::ortho(0.f, 1600.f, 0.f, 900.f, -1000.f, 1000.f);
		projectionStack.LoadMatrix(ortho);

		// Set view and model matrix to identity
		{
			viewStack.PushMatrix();
			viewStack.LoadIdentity();
			{
				modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(800 + x, 450 + y, 0);
				modelStack.Scale(sizeX, sizeY, 1);
				RenderMesh(mesh, false);
				modelStack.PopMatrix();
			}
			viewStack.PopMatrix();
		}
		projectionStack.PopMatrix();
	}
	glEnable(GL_DEPTH_TEST);

}

void BaseScene::HandleMouseInput() {
	static bool isLeftUp = false;
	static bool isRightUp = false;
	// Process Left button
	if (!isLeftUp && MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if (isLeftUp && MouseController::GetInstance()->IsButtonUp(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	// Continue to do for right button

	if (!isLeftUp && MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		// Step 3
		// transform into UI space
		double x = MouseController::GetInstance()->GetMousePositionX();
		double y = 600 - MouseController::GetInstance()->GetMousePositionY();
		// Check if mouse click position is within the GUI box
		// Change the boundaries as necessary
		if (x > 0 && x < 100 && y > 0 && y < 100) {
			std::cout << "GUI IS CLICKED" << std::endl;
		}
	}
}

void BaseScene::RenderText(Mesh* mesh, std::string text, glm::vec3
	color)
{
	if (!mesh || mesh->textureID <= 0) { //Proper error check
		return;
	}
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Disable back face culling
	glDisable(GL_CULL_FACE);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(glm::mat4(1.f), glm::vec3(i * 1.0f, 0, 0));
		glm::mat4 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void BaseScene::RenderTextOnScreen(Mesh* mesh, std::string
	text, glm::vec3 color, float size, float x, float y, char alignment, float spacingPercentage)
{
	spacingPercentage = glm::clamp(spacingPercentage, 0.0f, 1.0f);

	float spacing = 1.0f * spacingPercentage;
	float textWidth = (text.length() - 1) * spacing;

	float alignmentOffset = 0.f;
	if (alignment == 'L') {
		alignmentOffset = 0.f;
	}
	else if (alignment == 'C') {
		alignmentOffset = -textWidth * 0.5f * size;
	}
	else if (alignment == 'R') {
		alignmentOffset = -textWidth * size;
	}


	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 1600.f, 0.f, 900.f, -
		100.f, 100.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(800 + x + alignmentOffset, 450 + y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(1.f),
			glm::vec3(0.5f + i * (spacing), 0.5f, 0)
		);
		glm::mat4 MVP = projectionStack.Top() *
			viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void BaseScene::RenderSkybox()
{
	//front 
	modelStack.PushMatrix();
	// Offset in Z direction by -50 units
	modelStack.Translate(0.f, 0.f, -50.f);
	// Skybox should be rendered without light
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	//back
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 50.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	//left
	modelStack.PushMatrix();
	modelStack.Translate(-50.f, 0.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	//right
	modelStack.PushMatrix();
	modelStack.Translate(50.f, 0.f, 0.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	//top
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 50.f, 0.f);
	modelStack.Rotate(-270.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	//bottom
	modelStack.PushMatrix();
	modelStack.Translate(0.f, -50.f, 0.f);
	modelStack.Rotate(270.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

void BaseScene::RenderUI()
{
	{
		// Render GUI
		//RenderMeshOnScreen(meshList[GEO_MENU_GUI], 0, 0, 1600, 900);
		//RenderMeshOnScreen(meshList[GEO_SWITCHSCENE_GUI], 0, 0, 1600, 900);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (interactedIndex != -1) {
			RenderMeshOnScreen(meshList[GEO_CROSSHAIROPAQUE_GUI], 0, 0, 1600, 900);

			RenderMeshOnScreen(meshList[GEO_INTERACTFADE_GUI], interactGUI_positionOffset.x, interactGUI_positionOffset.y, 1600, 900);
			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], interactives[interactedIndex], glm::vec3(1, 1, 1), 20, 410 + interactGUI_positionOffset.x * 1.5f, -10 + interactGUI_positionOffset.y, 'R', .6f);

			//meshlist[font type], text, color, size, x, y, alignment, spacing percentage
			if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_F))
			{
				RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "[F]", glm::vec3(109 / 255.f, 41 / 255.f, 34 / 255.f), 26, 440 + interactGUI_positionOffset.x, -13 + interactGUI_positionOffset.y, 'L', .6f);
			}
			else {
				RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[F]", glm::vec3(109 / 255.f, 41 / 255.f, 34 / 255.f), 26, 440 + interactGUI_positionOffset.x, -13 + interactGUI_positionOffset.y, 'L', .6f);
			}
		}
		else {
			RenderMeshOnScreen(meshList[GEO_CROSSHAIRTRANSLUCENT_GUI], 0, 0, 1600, 900);
		}

		RenderTextOnScreen(meshList[GEO_CARNIVALEEFREAKSHOW_FONT], "SCORE", glm::vec3(0, 1, 0), 45, -795, 400, 'L', .6f);

		if (itemInHand != "") {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			RenderMeshOnScreen(meshList[GEO_ITEMINHANDBORDER_GUI], 603 - (itemInHandGUI_scaleOffset.x * .5f), -343.2f + (itemInHandGUI_scaleOffset.y * .5f), 214 + itemInHandGUI_scaleOffset.x, 33.7f + itemInHandGUI_scaleOffset.y);
			RenderMeshOnScreen(meshList[GEO_ITEMINHANDFADEBACKGROUND_GUI], 603 - (itemInHandGUI_scaleOffset.x * .5f), -343.2f + (itemInHandGUI_scaleOffset.y * .5f), 214 + itemInHandGUI_scaleOffset.x, 33.7f + itemInHandGUI_scaleOffset.y);
			RenderMeshOnScreen(meshList[GEO_ITEMINHANDFADE_GUI], 0, 0 + itemInHandGUI_scaleOffset.y, 1600, 900);

			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "(" + std::to_string(amountOfItem) + "x) " + itemInHand, glm::vec3(1, 1, 1), 20, 690, -355 + itemInHandGUI_scaleOffset.y, 'R', .6f);

			if (itemInUse) {
				RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "[E]", glm::vec3(1, 1, 1), 15, 700, -300 + itemInHandGUI_scaleOffset.y, 'R', .6f);
			}
			else {
				RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[E]", glm::vec3(1, 1, 1), 15, 700, -300 + itemInHandGUI_scaleOffset.y, 'R', .6f);
			}
			
			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "Use", glm::vec3(1, 1, 1), 15, 660, -300 + itemInHandGUI_scaleOffset.y, 'R', .6f);

			RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[X]", glm::vec3(1, 1, 1), 15, 700, -320 + itemInHandGUI_scaleOffset.y, 'R', .6f);
			RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "Drop", glm::vec3(1, 1, 1), 15, 660, -320 + itemInHandGUI_scaleOffset.y, 'R', .6f);
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
			modelStack.Scale(interactedEUI_scale, interactedEUI_scale, interactedEUI_scale);

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
		modelStack.Scale(.05f, .05f, .05f);

		meshList[GEO_INTERACT_EUI]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_INTERACT_EUI]->material.kDiffuse = glm::vec3(1.f, 1.f, 1.f);
		meshList[GEO_INTERACT_EUI]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
		meshList[GEO_INTERACT_EUI]->material.kShininess = 1.0f;

		RenderMesh(meshList[GEO_INTERACT_EUI], false);

		modelStack.PopMatrix();
	}

	{
		// Render Switch Scene UI
		if (nextScene != 0) {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			RenderMeshOnScreen(meshList[GEO_SWITCHSCENE_GUI], 0.f, 0.f, 1200 * sceneSwitchUI_scalePercentage, 675 * sceneSwitchUI_scalePercentage);
		}
	}
}

void BaseScene::setCameraOrigin(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	cameraOriginPosition = position;
	cameraOriginTarget = target;
	cameraOriginUp = up;

	camera.Init(position, target, up);
}

void BaseScene::resetInteractives()
{
	for (int i = 0; i < TOTAL_INTERACTIVES; i++) {
		interactives[i] = "";
		interactivesType[i] = ' ';
		interactivesPos[i] = glm::vec3(0, 0, 0);

		interactedIndexes[i] = -1;
	}

	noOfInteractives = 0;
	interactedIndex = -1;
}

void BaseScene::addInteractives(std::string name, char type, glm::vec3 position, int pickableIndex)
{
	int temp = -1;

	for (int i = 0; i < TOTAL_INTERACTIVES; i++) {
		if (interactives[i] == "") { // empty slot
			interactives[i] = name;
			interactivesType[i] = type;
			interactivesPos[i] = position;

			if (type == 'P') {
				interactivePickablesIndex[i] = pickableIndex;
			}

			temp = i;
			noOfInteractives++;
			break;
		}
	}

	if (temp != -1) {
		//std::cout << "[INTERACTIVES] Successfully added " << interactivesType[temp] << " " << interactives[temp] << " (" << interactivesPos[temp].x << ", " << interactivesPos[temp].y << ", " << interactivesPos[temp].z << ") to index " << temp << std::endl;
	}
	else {
		//std::cout << "[INTERACTIVES] Insufficient space. Operation failed" << std::endl;
	}
}

void BaseScene::addPickables(std::string name, glm::vec3 position)
{
	int temp = -1;

	for (int i = 0; i < TOTAL_PICKABLES; i++) {
		if (pickables[i] == "") { // empty slot
			pickables[i] = name;
			pickablesPos[i] = position;

			temp = i;
			noOfPickables++;
			break;
		}
	}

	if (temp != -1) {
		//std::cout << "[PICKABLES] Successfully added " << pickables[temp] << " (" << pickablesPos[temp].x << ", " << pickablesPos[temp].y << ", " << pickablesPos[temp].z << ") to index " << temp << std::endl;
	}
	else {
		//std::cout << "[PICKABLES] Insufficient space. Operation failed" << std::endl;
	}
}

void BaseScene::removePickables(int index)
{
	std::string name = pickables[index];

	if (pickables[index] != "") {
		pickables[index] = "";
		pickablesPos[index] = glm::vec3(0, 0, 0);

		noOfPickables--;

		//std::cout << "[PICKABLES] Successfully removed " << name << ". Initial index data is now " << pickables[temp] <<	" (" << pickablesPos[temp].x << ", " << pickablesPos[temp].y << ", " << pickablesPos[temp].z << ") " << std::endl;
	}
	else {
		//std::cout << "[PICKABLES] Unable to find " << name << ". Operation failed" << std::endl;
	}
}

void BaseScene::initializePickablesInteractives()
{
	for (int i = 0; i < TOTAL_PICKABLES; i++) {
		if (pickables[i] != "") { // found item
			addInteractives(pickables[i], 'P', pickablesPos[i], i);
		}
	}
}

void BaseScene::getClosestInteractive()
{
	int temp = 0;
	glm::vec3 interactivePos(interactivesPos[0].x, interactivesPos[0].y, interactivesPos[0].z);

	// distance check
	for (int i = 0; i < TOTAL_INTERACTIVES; i++) {
		if (interactives[i] != "") {
			interactivePos = glm::vec3(interactivesPos[i].x, interactivesPos[i].y, interactivesPos[i].z);

			float distance = glm::length(camera.position - interactivePos);

			if (distance <= 2.f) {
				interactedIndexes[temp] = i;
				temp++;
			}
		}
	}

	// angle check
	glm::vec3 forward = glm::normalize(camera.target - camera.position);
	glm::vec3 toItem(0, 0, 0);
	glm::vec3 itemPos(0, 0, 0);

	float closestDot = 0.f;
	float dot = 0.f;

	if (temp > 0) {
		interactedIndex = -1;
		for (int i = 0; i < temp; i++) {
			itemPos = interactivesPos[interactedIndexes[i]];
			toItem = glm::normalize(itemPos - camera.position);
			dot = glm::dot(forward, toItem);

			if (dot > 0.96f && dot > closestDot) {
				closestDot = dot;
				interactedIndex = interactedIndexes[i];
			}
		}
	}
	else {
		interactedIndex = -1;
	}

	if (interactedIndex != previousInteractedIndex) {
		previousInteractedIndex = interactedIndex;
		interactedEUI_scale = 0.05f;
		interactGUI_positionOffset = glm::vec2(-25.f, 0);
	}
}

void BaseScene::dropItemInHand(int amountToRemove)
{
	std::string itemToDrop = itemInHand;

	if (amountOfItem > amountToRemove) {
		for (int i = 0; i < amountToRemove; i++) {
			amountOfItem--;
			glm::vec3 placementPos = camera.target;
			placementPos.y = 0.f;
			placementPos += glm::vec3(((rand() % 5) - 2) / 100.f, 0, ((rand() % 5) - 2) / 100.f);

			addPickables(itemToDrop, placementPos);
		}
	}
	else {
		for (int i = 0; i < amountOfItem; i++) {
			amountOfItem--;
			glm::vec3 placementPos = camera.target;
			placementPos.y = 0.f;
			placementPos += glm::vec3(((rand() % 5) - 2) / 100.f, 0, ((rand() % 5) - 2) / 100.f);

			addPickables(itemToDrop, placementPos);
		}

		itemInHand = "";
		amountOfItem = 0;
		itemInUse = false;
	}
}

void BaseScene::addItemInHand()
{
	std::string itemToHold = interactives[interactedIndex];

	if (itemInHand != "") { // if holding smth already
		if (itemInHand == interactives[interactedIndex]) { // same item
			std::cout << interactivePickablesIndex[interactedIndex] << std::endl;
			removePickables(interactivePickablesIndex[interactedIndex]);
			amountOfItem++;
		}
		else {
			dropItemInHand(amountOfItem);
			std::cout << interactivePickablesIndex[interactedIndex] << std::endl;
			removePickables(interactivePickablesIndex[interactedIndex]);
			itemInHand = itemToHold;
			amountOfItem++;
		}
	}
	else {
		std::cout << interactivePickablesIndex[interactedIndex] << std::endl;
		removePickables(interactivePickablesIndex[interactedIndex]);
		itemInHand = itemToHold;
		amountOfItem++;
	}
}
