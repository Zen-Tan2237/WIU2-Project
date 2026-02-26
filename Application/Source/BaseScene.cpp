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
#include "CollisionDetection.h"

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

	// CAMERA INITIALIZATION
	setCameraOrigin(glm::vec3(0.f, 1.f, -1.f), glm::vec3(0.f, 1.f, 1.f), glm::vec3(0.f, 2.f, -1.f));

	bool settings[2] = { false, false };
	cameraBody.InitPhysicsObject(
		camera.position,
		1.0f,
		BoundingBox::Type::OBB,
		glm::vec3(.25f, 5.f, .25f),
		settings
	);

	bobAmplitudeVertical = 0.02f;
	bobAmplitudeHorizontal = 0.015f;
	bobFrequency = 12.0f;

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	// DEMO FOR OBJ FILES
	//meshList[GEO_LEFT] = meshBuilder.LoadOBJMTL("Left", "OBJ//left.obj", "OBJ//left.mtl");
	//meshList[GEO_LEFT] = MeshBuilder::LoadOBJ("Left", "OBJ//left.obj");
	//meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);

	// OTHERS
	meshList[GEO_GRASS] = MeshBuilder::GenerateQuad("Grass", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_GRASS]->textureID = LoadTGA("Textures//Grass.tga");

	// MODELS
	meshList[GEO_FLOOR] = MeshBuilder::GenerateOBJ("floor", "Models//Grass_base.obj");
	meshList[GEO_FLOOR]->textureID = LoadTGA("Textures//Ground_texture.tga");

	meshList[GEO_BACKGROUND_BUILDINGS1] = MeshBuilder::GenerateOBJ("Background Buildings", "Models//Buildings_Background_1.obj");
	meshList[GEO_BACKGROUND_BUILDINGS1]->textureID = LoadTGA("Textures//skyscrapers.tga");

	meshList[GEO_BACKGROUND_BUILDINGS2] = MeshBuilder::GenerateOBJ("Background Buildings", "Models//Buildings_Background_2.obj");
	meshList[GEO_BACKGROUND_BUILDINGS2]->textureID = LoadTGA("Textures//skyscrapers.tga");

	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJ("Fence", "Models//Fence.obj");

	meshList[GEO_FOUNTAIN] = MeshBuilder::GenerateOBJ("fountain", "Models//Fountain.obj");
	meshList[GEO_FOUNTAIN]->textureID = LoadTGA("Textures//Fountain.tga");

	meshList[GEO_MONKEY] = MeshBuilder::GenerateOBJ("monkey", "Models//Monkey.obj");
	meshList[GEO_MONKEY]->textureID = LoadTGA("Textures//Monke.tga");

	meshList[GEO_STALL] = MeshBuilder::GenerateOBJ("stall", "Models//minigame_Stall.obj");
	meshList[GEO_STALL]->textureID = LoadTGA("Textures//minigameStall.tga");

	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJ("table", "Models//table.obj");
	meshList[GEO_TABLE]->textureID = LoadTGA("Textures//table.tga");

	meshList[GEO_FOODSTAND] = MeshBuilder::GenerateOBJ("foodstand", "Models//Hot_Dog_Stand.obj");
	meshList[GEO_FOODSTAND]->textureID = LoadTGA("Textures//Hotdog.tga");

	meshList[GEO_FERRISWHEEL] = MeshBuilder::GenerateOBJ("ferriswheel", "Models//FerrisWheel.obj");
	meshList[GEO_FERRISWHEEL]->textureID = LoadTGA("Textures//FerrisWheel.tga");

	//pickables
	meshList[GEO_BASEBALL] = MeshBuilder::GenerateOBJ("Baseball", "Models//baseball.obj");
	meshList[GEO_BASEBALL]->textureID = LoadTGA("Textures//baseball.tga");

	meshList[GEO_CANSCOKE] = MeshBuilder::GenerateOBJ("Coke Can", "Models//Cans_Coke.obj");
	meshList[GEO_CANSCOKE]->textureID = LoadTGA("Textures//Cans_Coke.tga");

	meshList[GEO_CANSMTNDEW] = MeshBuilder::GenerateOBJ("Mtn Dew Can", "Models//Cans_Coke.obj");
	meshList[GEO_CANSMTNDEW]->textureID = LoadTGA("Textures//Cans_MtnDew.tga");

	meshList[GEO_CANSSPRITE] = MeshBuilder::GenerateOBJ("Sprite Can", "Models//Cans_Sprite.obj");
	meshList[GEO_CANSSPRITE]->textureID = LoadTGA("Textures//Cans_Sprite.tga");

	meshList[GEO_CANSPEPSI] = MeshBuilder::GenerateOBJ("Pepsi Can", "Models//Cans_Pepsi.obj");
	meshList[GEO_CANSPEPSI]->textureID = LoadTGA("Textures//Cans_Pepsi.tga");

	meshList[GEO_FIGURINE] = MeshBuilder::GenerateOBJ("figurine", "Models//Figurine.obj");
	meshList[GEO_FIGURINE]->textureID = LoadTGA("Textures//Figurine.tga");

	meshList[GEO_PIG] = MeshBuilder::GenerateOBJ("pig", "Models//Pig.obj");
	meshList[GEO_PIG]->textureID = LoadTGA("Textures//Pig.tga");

	meshList[GEO_PLUSHIE] = MeshBuilder::GenerateOBJ("plushie", "Models//Plushie.obj");
	meshList[GEO_PLUSHIE]->textureID = LoadTGA("Textures//Frieren_Plushie.tga");

	meshList[GEO_5090] = MeshBuilder::GenerateOBJ("5090", "Models//RTX5090_BOX.obj");
	meshList[GEO_5090]->textureID = LoadTGA("Textures//RTX5090_BOX.tga");

	meshList[GEO_PINGPONGBALL] = MeshBuilder::GenerateOBJ("5090", "Models//RTX5090_BOX.obj");
	meshList[GEO_5090]->textureID = LoadTGA("Textures//RTX5090_BOX.tga");

	meshList[GEO_PINGPONGBALL] = MeshBuilder::GenerateOBJ("Ping Pong Ball", "Models//PingPongBall.obj");
	meshList[GEO_PINGPONGBALL]->textureID = LoadTGA("Textures//Table_Tennis.tga");

	// SKYBOX
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Front", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Textures//Skybox//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Back", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Textures//Skybox//back.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Left", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Textures//Skybox//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Right", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Textures//Skybox//right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Textures//Skybox//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Textures//Skybox//bottom.tga");

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

	meshList[GEO_DIALOGUEFADE_GUI] = MeshBuilder::GenerateQuad("Dialogue Fade GUI", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_DIALOGUEFADE_GUI]->textureID = LoadTGA("Image//DialogueFade_GUI.tga");

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

	//
	glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	glUniform1i(m_parameters[U_NUMLIGHTS], TOTAL_LIGHTS);
	enableLight = true;

	// INTERACTIVES
	noOfInteractives = 0;

	for (int i = 0; i < TOTAL_INTERACTIVES; i++) {
		interactives[i] = "";
		interactivesType[i] = '!';
		interactivesPos[i] = glm::vec3(0, 0, 0);
		interactivePickablesIndex[i] = 0;

		interactedIndexes[i] = -1;
	}

	interactedIndex = -1;
	previousInteractedIndex = -1;

	noOfPickables = 0;

	accumulatedCash = 25; //$25 to start with but will be overriden when this is not the first scene to be started up (i.e. when coming from another scene)

	// ITEM IN HAND
	itemInHand = nullptr;
	amountOfItem = 0;
	previousItemInHandName = "";
	itemInHandElapsed = 0.f;
	itemInUse = false;
	itemUseHeldElapsed = 0.f;

	dropKeybindHeldElapsed = 0.f;
	droppedFirst = false;

	settings[0] = true;
	settings[1] = false;

	for (int i = 0; i < TOTAL_PICKABLES; i++) {
		pickables[i] = nullptr;
	}
	newestPickableIndex = 0;

	// UI
	interactPrompt.setTargetPosition(glm::vec2(0, 0));
	interactEUI.setTargetScale(glm::vec2(0.05f));
	dialogueFadeHUD.resetPosition(glm::vec2(0, -414));
	dialogueFadeHUD.setTargetPosition(glm::vec2(0, -414));

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

	// DEBUG
	dtMultiplier = 1.f;

	// world objects --- remove this bad stuff
	bool miscSettings[2] = { false, false }; // for gravity and drag. override in case of specific objects

	Fountain.InitPhysicsObject(glm::vec3(0, 0, 0), 0.f, BoundingBox::Type::SPHERE, glm::vec3(10.f, 0.f, 0.f), miscSettings);

	// MESH MATERIAL INITIALIZATION
	meshList[GEO_GRASS]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_GRASS]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_GRASS]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_GRASS]->material.kShininess = 1.f;

	meshList[GEO_BASEBALL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_BASEBALL]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_BASEBALL]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_BASEBALL]->material.kShininess = 1.0f;

	meshList[GEO_CANSCOKE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_CANSCOKE]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_CANSCOKE]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_CANSCOKE]->material.kShininess = 1.0f;

	meshList[GEO_CANSSPRITE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_CANSSPRITE]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_CANSSPRITE]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_CANSSPRITE]->material.kShininess = 1.0f;

	meshList[GEO_CANSPEPSI]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_CANSPEPSI]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_CANSPEPSI]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_CANSPEPSI]->material.kShininess = 1.0f;

	meshList[GEO_CANSMTNDEW]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_CANSMTNDEW]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_CANSMTNDEW]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_CANSMTNDEW]->material.kShininess = 1.0f;

	meshList[GEO_FLOOR]->material.kAmbient = glm::vec3(0.01f, 0.01f, 0.01f);
	meshList[GEO_FLOOR]->material.kDiffuse = glm::vec3(.35f, .35f, .35f);
	meshList[GEO_FLOOR]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_FLOOR]->material.kShininess = 1.0f;

	meshList[GEO_BACKGROUND_BUILDINGS1]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_BACKGROUND_BUILDINGS1]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_BACKGROUND_BUILDINGS1]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_BACKGROUND_BUILDINGS1]->material.kShininess = 1.0f;

	meshList[GEO_BACKGROUND_BUILDINGS2]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_BACKGROUND_BUILDINGS2]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_BACKGROUND_BUILDINGS2]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_BACKGROUND_BUILDINGS2]->material.kShininess = 1.0f;

	meshList[GEO_FENCE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_FENCE]->material.kShininess = 1.0f;

	meshList[GEO_MONKEY]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_MONKEY]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_MONKEY]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_MONKEY]->material.kShininess = 1.0f;

	meshList[GEO_FOUNTAIN]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_FOUNTAIN]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_FOUNTAIN]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_FOUNTAIN]->material.kShininess = 1.0f;

	meshList[GEO_FIGURINE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_FIGURINE]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_FIGURINE]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_FIGURINE]->material.kShininess = 1.0f;

	meshList[GEO_PIG]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_PIG]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_PIG]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_PIG]->material.kShininess = 1.0f;

	meshList[GEO_PLUSHIE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_PLUSHIE]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_PLUSHIE]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_PLUSHIE]->material.kShininess = 1.0f;

	meshList[GEO_5090]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_5090]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_5090]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_5090]->material.kShininess = 1.0f;

	meshList[GEO_PINGPONGBALL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_PINGPONGBALL]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_PINGPONGBALL]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_PINGPONGBALL]->material.kShininess = 1.0f;

	meshList[GEO_TABLE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_TABLE]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_TABLE]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_TABLE]->material.kShininess = 1.0f;

	meshList[GEO_FOODSTAND]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_FOODSTAND]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_FOODSTAND]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_FOODSTAND]->material.kShininess = 1.0f;

	meshList[GEO_FERRISWHEEL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_FERRISWHEEL]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_FERRISWHEEL]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_FERRISWHEEL]->material.kShininess = 1.0f;

	meshList[GEO_STALL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_STALL]->material.kDiffuse = glm::vec3(.5f, .5f, .5f);
	meshList[GEO_STALL]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	meshList[GEO_STALL]->material.kShininess = 1.0f;
}

void BaseScene::Update(double dt)
{
	// DEBUG
	currentFPS = static_cast<float>(1.0 / dt);

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_Q)) {
		dtMultiplier -= 0.005f;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_E)) {
		dtMultiplier += 0.005f;
	}

	//std::cout << dtMultiplier << std::endl;

	dt *= dtMultiplier;
	//

	HandleKeyPress(dt);

	//if (nextScene == 0) {
	//	camera.Update(dt);
	//}

	////if (KeyboardController::GetInstance()->IsKeyDown('I'))
	////	light[0].position.z -= static_cast<float>(dt) * 5.f;
	////if (KeyboardController::GetInstance()->IsKeyDown('K'))
	////	light[0].position.z += static_cast<float>(dt) * 5.f;
	////if (KeyboardController::GetInstance()->IsKeyDown('J'))
	////	light[0].position.x -= static_cast<float>(dt) * 5.f;
	////if (KeyboardController::GetInstance()->IsKeyDown('L'))
	////	light[0].position.x += static_cast<float>(dt) * 5.f;
	////if (KeyboardController::GetInstance()->IsKeyDown('O'))
	////	light[0].position.y -= static_cast<float>(dt) * 5.f;
	////if (KeyboardController::GetInstance()->IsKeyDown('P'))
	////	light[0].position.y += static_cast<float>(dt) * 5.f;


	// SET ISHELD WHEN PICKABLE IS ITEM IN HAND
	for (int i = 0; i < TOTAL_PICKABLES; i++) {
		if (pickables[i] != nullptr && itemInHand != nullptr) {
			if (pickables[i] == itemInHand) {
				pickables[i]->isHeld = true;
			}
			else {
				pickables[i]->isHeld = false;
			}
		}
	}

	// UPDATE CAMERA INPUT
	camera.Update(dt);

	// SYNC PHYSICS BODY
	cameraBody.position = camera.position;
	cameraBody.velocity = glm::vec3(0);

	// DO COLLISIONS
	int index = 0;
	for (auto& obj : worldObjects) {
		CollisionData cd;
		if (CheckCollision(cameraBody, obj, cd) && index != 0) {
			ResolveCollision(cd);
		}

		for (int i = 0; i < TOTAL_PICKABLES; ++i) {	// Pickables - World Objects
			if (pickables[i] != nullptr) {
				if (!pickables[i]->isHeld && (index < 1 || index > 4)) {
					CollisionData cd;
					if (CheckCollision(pickables[i]->body, obj, cd)) {
						ResolveCollision(cd);
					}
				}
			}
		}

		index++;
	}

	for (int i = 0; i < TOTAL_PICKABLES; ++i) {	// Pickables - World Objects
		for (auto& obj : worldObjects) {
			
		}
	}

	for (int i = 0; i < TOTAL_PICKABLES - 1; ++i) {
		if (pickables[i] != nullptr) {
			for (int o = i + 1; o < TOTAL_PICKABLES; ++o) {
				if (pickables[o] != nullptr) {
					if (!pickables[i]->isHeld && !pickables[o]->isHeld) {
						CollisionData cd;
						if (CheckCollision(pickables[i]->body, pickables[o]->body, cd)) {
							ResolveCollision(cd);
						}
					}
				}
			}
		}
	}

	// SYNC CAMERA BACK
	if (!glm::any(glm::isnan(cameraBody.position))) {
		camera.position = cameraBody.position;
	}

	// CALCULATE VISUAL BOBBING 
	glm::vec3 currentPlayerPosition = camera.position;
	glm::vec3 delta = currentPlayerPosition - previousPlayerPosition;
	delta.y = 0.0f;
	previousPlayerPosition = currentPlayerPosition;

	float distanceMoved = glm::length(delta);
	bobDistanceAccumulated += distanceMoved;

	float targetWeight = (distanceMoved > 0.0001f) ? 1.0f : 0.0f;
	currentBobWeight += (targetWeight - currentBobWeight) * (1.0f - exp(-10.0f * dt));

	float wave = bobDistanceAccumulated * bobFrequency;
	float vOffset = sinf(wave) * bobAmplitudeVertical * currentBobWeight;
	float hOffset = sinf(wave * 0.5f) * bobAmplitudeHorizontal * currentBobWeight;

	glm::vec3 forward = glm::normalize(camera.target - camera.position);
	glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));

	// STORE GLOBABLY
	m_viewBobOffset = (camera.up * vOffset) + (right * hOffset);

	for (int i = 0; i < TOTAL_PICKABLES; ++i) {
		if (pickables[i] != nullptr) {
			if (!pickables[i]->isHeld) {
				pickables[i]->body.UpdatePhysics(dt);
			}
		}
	}

	/*for (auto& obj : worldObjects) {
		obj.UpdatePhysics(dt);
	}*/

	// OTHER HANDLING
	if (itemInHand != nullptr)
	{
		if (itemInHand->name != previousItemInHandName) {
			previousItemInHandName = itemInHand->name;
			itemInHandElapsed = 0.f;
		}

		itemInHandElapsed += dt;
	}
	else {
		previousItemInHandName = "";
		itemInHandElapsed = 0.f;
	}

	if (itemInHand != nullptr && itemInHandElapsed < 2.f) {
		itemInHandHUD.setTargetScale(glm::vec2(20.f, 200.f));
	}
	else {
		itemInHandHUD.setTargetScale(glm::vec2(0, 0));
	}

	// UI UPDATING
	interactEUI.Update(dt);
	interactPrompt.Update(dt);
	itemInHandHUD.Update(dt);
	sceneSwitchHUD.Update(dt);
	dialogueFadeHUD.Update(dt);

	//
	resetInteractives();

	// NEXT SCENE HANDLING
	if (nextSceneDelay > 0.f) {
		nextSceneDelay -= dt;
	}
	else {
		nextSceneDelay = 0.f;
	}

	// DIALOGUE HANDLING
	if (oldPart != part) {
		oldPart = part;
		phase = 0;
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

	if (phaseDurations[part][phase] > 0.f) {
		dialogueFadeHUD.setTargetPosition(glm::vec2(0, 0));
	}
	else {
		dialogueFadeHUD.setTargetPosition(glm::vec2(0, -414));
	}

	// FPS
	fps = glm::round(currentFPS * 100.f) / 100.f;
}

void BaseScene::Render()
{
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

	//////////////////////////////////////////

	// INTERACTIVES HANDLER
	if (interactedIndex != -1 && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_F)) { // means got prompt, is close to and facing smth
		if (interactivesType[interactedIndex] == 'I') {

		}
		else if (interactivesType[interactedIndex] == 'P')
		{
			int pickIndex = interactivePickablesIndex[interactedIndex];

			addItemInHand(pickIndex);
			//removePickables(pickIndex);
		}
	}

	// HOLD ITEM HANDLER
	if (itemInHand != nullptr) {
		if (MouseController::GetInstance()->IsButtonDown(0) && !itemInUse)
		{
			itemInUse = true;
			itemUseHeldElapsed = 0.f;
		}
		else if (itemInUse) {
			if (!MouseController::GetInstance()->IsButtonDown(0)) {
				useItemInHand();
				itemInUse = false;
			}
			else {
				itemUseHeldElapsed += dt;
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
		//// Render GUI
		////RenderMeshOnScreen(meshList[GEO_MENU_GUI], 0, 0, 1600, 900);
		////RenderMeshOnScreen(meshList[GEO_SWITCHSCENE_GUI], 0, 0, 1600, 900);

		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//if (interactedIndex != -1) {
		//	RenderMeshOnScreen(meshList[GEO_CROSSHAIROPAQUE_GUI], crosshair.getPosition().x, crosshair.getPosition().y, 1600, 900);

		//	RenderMeshOnScreen(meshList[GEO_INTERACTFADE_GUI], interactPrompt.getPosition().x, interactPrompt.getPosition().y, 1600, 900);
		//	RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], interactives[interactedIndex], glm::vec3(1, 1, 1), 20, 410 + interactPrompt.getPosition().x * 1.5f, -10 + interactPrompt.getPosition().y, 'R', .6f);

		//	//meshlist[font type], text, color, size, x, y, alignment, spacing percentage
		//	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_F))
		//	{
		//		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "[F]", glm::vec3(109 / 255.f, 41 / 255.f, 34 / 255.f), 26, 440 + interactPrompt.getPosition().x, -13 + interactPrompt.getPosition().y, 'L', .6f);
		//	}
		//	else {
		//		RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[F]", glm::vec3(109 / 255.f, 41 / 255.f, 34 / 255.f), 26, 440 + interactPrompt.getPosition().x, -13 + interactPrompt.getPosition().y, 'L', .6f);
		//	}
		//}
		//else {
		//	RenderMeshOnScreen(meshList[GEO_CROSSHAIRTRANSLUCENT_GUI], crosshair.getPosition().x, crosshair.getPosition().y, 1600, 900);
		//}

		//RenderTextOnScreen(meshList[GEO_CARNIVALEEFREAKSHOW_FONT], "SCORE", glm::vec3(0, 1, 0), 45, -795, 400, 'L', .6f);

		//if (itemInHand != nullptr) {
		//	glDisable(GL_DEPTH_TEST);
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//	RenderMeshOnScreen(meshList[GEO_ITEMINHANDBORDER_GUI], 603 - (itemInHandHUD.getScale().x * .5f), -343.2f + (itemInHandHUD.getScale().y * .5f), 214 + itemInHandHUD.getScale().x, 33.7f + itemInHandHUD.getScale().y);
		//	RenderMeshOnScreen(meshList[GEO_ITEMINHANDFADEBACKGROUND_GUI], 603 - (itemInHandHUD.getScale().x * .5f), -343.2f + (itemInHandHUD.getScale().y * .5f), 214 + itemInHandHUD.getScale().x, 33.7f + itemInHandHUD.getScale().y);
		//	RenderMeshOnScreen(meshList[GEO_ITEMINHANDFADE_GUI], 0, 0 + itemInHandHUD.getScale().y, 1600, 900);
		//	
		//	RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "(" + std::to_string(amountOfItem) + "x) " + itemInHand->name, glm::vec3(1, 1, 1), 20, 690, -355 + itemInHandHUD.getScale().y, 'R', .6f);

		//	if (itemInUse) {
		//		RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "[E]", glm::vec3(1, 1, 1), 15, 700, -300 + itemInHandHUD.getScale().y, 'R', .6f);
		//	}
		//	else {
		//		RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[E]", glm::vec3(1, 1, 1), 15, 700, -300 + itemInHandHUD.getScale().y, 'R', .6f);
		//	}
		//	RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "Use", glm::vec3(1, 1, 1), 15, 660, -300 + itemInHandHUD.getScale().y, 'R', .6f);

		//	RenderTextOnScreen(meshList[GEO_HOMEVIDEO_FONT], "[X]", glm::vec3(1, 1, 1), 15, 700, -320 + itemInHandHUD.getScale().y, 'R', .6f);
		//	RenderTextOnScreen(meshList[GEO_VCROSDMONO_FONT], "Drop", glm::vec3(1, 1, 1), 15, 660, -320 + itemInHandHUD.getScale().y, 'R', .6f);
		//}

		//// Debug
		//RenderTextOnScreen(meshList[GEO_HOMEVIDEOBOLD_FONT], "PART: " + std::to_string(part) + " PHASE: " + std::to_string(phase), glm::vec3(1, 1, 1), 15, 0, 435, 'C', .6f);
	}

	//// Render EUI
	//glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glm::vec3 euiPos(0, 1000, 0);

	//for (int i = 0; i < noOfInteractives; i++) {
	//	bool temp = interactivesType[i] == 'I';
	//	if (!temp && pickables[interactivePickablesIndex[i]] != nullptr) {
	//		temp = !pickables[interactivePickablesIndex[i]]->isHeld;
	//	}

	//	if (temp) {
	//		euiPos = interactivesPos[i] + glm::vec3(0, 0.25f, 0);

	//		glm::vec3 dir = camera.position - euiPos;
	//		dir = glm::normalize(dir);

	//		float yaw = glm::degrees(atan2(dir.x, dir.z));
	//		float pitch = glm::degrees(asin(dir.y));

	//		if (interactedIndex == i) {
	//			modelStack.PushMatrix();
	//			modelStack.Translate(euiPos.x, euiPos.y, euiPos.z);
	//			modelStack.Rotate(yaw, 0.f, 1.f, 0.f);
	//			modelStack.Rotate(-pitch, 1.f, 0.f, 0.f);
	//			modelStack.Scale(interactEUI.getScale().x, interactEUI.getScale().x, interactEUI.getScale().x);

	//			meshList[GEO_INTERACTED_EUI]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	//			meshList[GEO_INTERACTED_EUI]->material.kDiffuse = glm::vec3(1.f, 1.f, 1.f);
	//			meshList[GEO_INTERACTED_EUI]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	//			meshList[GEO_INTERACTED_EUI]->material.kShininess = 1.0f;

	//			RenderMesh(meshList[GEO_INTERACTED_EUI], false);

	//			modelStack.PopMatrix();
	//		}

	//		modelStack.PushMatrix();
	//		modelStack.Translate(euiPos.x, euiPos.y, euiPos.z);
	//		modelStack.Rotate(yaw, 0.f, 1.f, 0.f);
	//		modelStack.Rotate(-pitch, 1.f, 0.f, 0.f);
	//		modelStack.Scale(.02f, .02f, .02f);

	//		meshList[GEO_INTERACT_EUI]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	//		meshList[GEO_INTERACT_EUI]->material.kDiffuse = glm::vec3(1.f, 1.f, 1.f);
	//		meshList[GEO_INTERACT_EUI]->material.kSpecular = glm::vec3(0.f, 0.f, 0.f);
	//		meshList[GEO_INTERACT_EUI]->material.kShininess = 1.0f;

	//		RenderMesh(meshList[GEO_INTERACT_EUI], false);

	//		modelStack.PopMatrix();
	//	}
	//}

	//{
	//	// Render Switch Scene UI
	//	if (nextScene != 0) {
	//		glDisable(GL_DEPTH_TEST);
	//		glEnable(GL_BLEND);
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//		RenderMeshOnScreen(meshList[GEO_SWITCHSCENE_GUI], 0.f, 0.f, 1200 * sceneSwitchHUD.getScale().x, 675 * sceneSwitchHUD.getScale().x);
	//	}
	//}
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
		interactivePickablesIndex[i] = 0;
		interactedIndexes[i] = -1;
	}

	noOfInteractives = 0;
	interactedIndex = -1;
}

void BaseScene::addInteractives(std::string name, char type, glm::vec3 position, int pickableIndex)
{
	int temp = -1;

	for (int i = 0; i < TOTAL_INTERACTIVES; i++) {
		if (interactives[i] == "") {
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
}

void BaseScene::addPickables(std::string name, glm::vec3 position)
{
	int temp = -1;

	for (int i = 0; i < TOTAL_PICKABLES; i++) {
		if (pickables[i] == nullptr) {
			pickables[i] = new Pickable;
			pickables[i]->body.ResetPhysicsProperties();
			pickables[i]->name = name;
			pickables[i]->isHeld = false;

			bool settings[2] = { true, true };

			if (name == "Baseball") {
				pickables[i]->body.InitPhysicsObject(
					position,
					5.0f,
					BoundingBox::Type::SPHERE,
					glm::vec3(.05f, .05f, .05f),
					settings
				);
			}
			else if (name == "Coke" || name == "Mountain Dew" || name == "Sprite" || name == "Pepsi") {
				pickables[i]->body.InitPhysicsObject(
					position,
					5.0f,
					BoundingBox::Type::OBB,
					glm::vec3(.09f, .18f, .09f),
					settings
				);
			}
			else if (pickables[i]->name == "Figurine") {
				pickables[i]->body.InitPhysicsObject(
					position,
					5.0f,
					BoundingBox::Type::OBB,
					glm::vec3(.11f, .19f, .11f),
					settings
				);
			}
			else if (pickables[i]->name == "Halal Pork") {
				pickables[i]->body.InitPhysicsObject(
					position,
					5.0f,
					BoundingBox::Type::OBB,
					glm::vec3(.09f, .15f, .17f),
					settings
				);
			}
			else if (pickables[i]->name == "Plushie") {
				pickables[i]->body.InitPhysicsObject(
					position,
					5.0f,
					BoundingBox::Type::OBB,
					glm::vec3(.11f, .13f, .05f),
					settings
				);
			}
			else if (pickables[i]->name == "RTX 5090") {
				pickables[i]->body.InitPhysicsObject(
					position,
					5.0f,
					BoundingBox::Type::OBB,
					glm::vec3(.52f, .3f, .18f),
					settings
				);
			}
			else if (pickables[i]->name == "PingPong Ball") {
				pickables[i]->body.InitPhysicsObject(
					position,
					5.0f,
					BoundingBox::Type::SPHERE,
					glm::vec3(.05f, .05f, .05f),
					settings
				);
			}
			

			temp = i;
			noOfPickables++;
			newestPickableIndex = i;
			break;
		}
	}
}

void BaseScene::removePickables(int index)
{
	delete pickables[index];
	pickables[index] = nullptr;

	noOfPickables--;
}

void BaseScene::initializePickablesInteractives()
{
	for (int i = 0; i < TOTAL_PICKABLES; i++) {
		if (pickables[i] != nullptr) {
			addInteractives(pickables[i]->name, 'P', pickables[i]->body.position, i);
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
		interactEUI.resetScale(glm::vec2(0.02f));
		interactPrompt.resetPosition(glm::vec2(-25.f, 0));
	}
}

void BaseScene::dropItemInHand(int amountToRemove)
{
	std::string itemToDropName = itemInHand->name;

	int removeCount;

	if (amountToRemove > amountOfItem) {
		removeCount = amountOfItem;
	}
	else {
		removeCount = amountToRemove;
	}

	for (int i = 0; i < removeCount; i++) {
		amountOfItem--;

		glm::vec3 placementPos = camera.position + (camera.target - camera.position) * .5f;

		if (amountOfItem > 0) {
			placementPos += glm::vec3(((rand() % 5) - 2) / 100.f, 0,
				((rand() % 5) - 2) / 100.f);
			addPickables(itemToDropName, placementPos);
		}
		else {
			itemInHand->body.ResetPhysicsProperties();
			itemInHand->body.position = placementPos;
		}
	}

	if (amountOfItem == 0) {
		itemInHand->isHeld = false;
		itemInHand = nullptr;
		itemInUse = false;
	}
}

void BaseScene::addItemInHand(int index)
{
	if (itemInHand == nullptr)
    {
        itemInHand = pickables[index];
        amountOfItem = 1;
    }
    else if (itemInHand->name == pickables[index]->name)
    {
		removePickables(index);
        amountOfItem++;
    }
	else {
		Pickable* newItem = pickables[index];
		dropItemInHand(amountOfItem);
		itemInHand = newItem;
		amountOfItem = 1;
	}
}

void BaseScene::useItemInHand()
{
	if (itemInHand != nullptr) {
		if (itemInHand->name == "Baseball") {
			std::string itemToDropName = itemInHand->name;
			glm::vec3 placementPos = itemInHand->body.position;

			amountOfItem--;

			glm::vec3 forward = glm::normalize(camera.target - camera.position);
			float strength = 10.f + glm::clamp(static_cast<float>(itemUseHeldElapsed / 1.f), 0.f, 1.f) * 40.f;

			if (amountOfItem > 0) {
				addPickables(itemToDropName, placementPos);
				pickables[newestPickableIndex]->body.AddImpulse(glm::normalize((camera.position + forward * 5.f + glm::vec3(0, 1.f, 0)) - placementPos) * strength);
			}
			else {
				itemInHand->body.ResetPhysicsProperties();
				itemInHand->body.position = placementPos;
				itemInHand->isHeld = false;
				itemInHand->body.AddImpulse(glm::normalize((camera.position + forward * 5.f + glm::vec3(0, 1.f, 0)) - placementPos) * strength);
			}

			if (amountOfItem == 0) {
				itemInHand = nullptr;
			}
		}
		else if (itemInHand->name == "Coke" || itemInHand->name == "Mountain Dew" || itemInHand->name == "Sprite" || itemInHand->name == "Pepsi") {
		}
	}
}
