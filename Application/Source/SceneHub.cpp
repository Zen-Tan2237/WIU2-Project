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
		light[0].type = Light::POINT;
		light[0].power = 1;
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
		light[1].power = 1;
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
		light[6].power = 1;
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
		light[7].power = 1;
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
	meshList_hub[GEO_STALL] = MeshBuilder::GenerateOBJ("Stall", "Models//mannequin.obj"); //placeholder
	meshList_hub[GEO_STALL]->textureID = LoadTGA("Image//Menu_GUI.tga");

	// setup phase durations here ([first one is part][second one is phase]. phase means like u want a constant stream of dialgoues
	// make sure whenver u do part++, u have like (if part == <the number they should be at>) then part++
	phaseDurations[0][0] = 6.7f;
	phaseDurations[0][1] = 6.7f;
	phaseDurations[0][2] = 6.7f;
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
			if (interactives[interactedIndex] == "Enter Scene 2 (SceneHub)") {
				nextScene = 2;
				nextSceneDelay = 1.f;
				sceneSwitchUI_targetScalePercentage = 1.f;
			}
			else if (interactives[interactedIndex] == "1") {
				if (part == 0)
				{
					addPickables("Halal Pork", glm::vec3(0, 0, 0));
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
}

void SceneHub::Render()
{
	BaseScene::Render();

	{
		PushPop stall(modelStack);
		modelStack.Translate(0, 0, 0);
		modelStack.Scale(10.1f, 10.1f, 10.1f);
		meshList_hub[GEO_STALL]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList_hub[GEO_STALL]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		meshList_hub[GEO_STALL]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
		meshList_hub[GEO_STALL]->material.kShininess = 1.0f;

		RenderMesh(meshList_hub[GEO_STALL], true);
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

	BaseScene::RenderUI();
}

void SceneHub::Exit()
{
	BaseScene::Exit();
}
