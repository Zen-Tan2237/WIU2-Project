#include "SceneTester.h"
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
#include "audio.h"
#include "LoadTGA.h"
#include "CollisionDetection.h"



SceneTester::SceneTester()
{
}

SceneTester::~SceneTester()
{
}

void SceneTester::Init()
{
	// Set background color to dark blue
	glClearColor(0.1f, 0.0f, 0.4f, 0.0f);

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
	setCameraOrigin(glm::vec3(0.f, 5.f, -1.f), glm::vec3(0.f, 5.f, 1.f), glm::vec3(0.f, 6.f, -1.f));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", glm::vec3(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Arm", glm::vec3(0.5f, 0.5f, 0.5f), 1.f);
	meshList[GEO_PLANE] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_PLANE]->textureID = LoadTGA("Image//nyp.tga");

	//meshList[GEO_GUI_QUAD] = MeshBuilder::GenerateQuad("GUIQUAD", glm::vec3(1.f, 1.f, 1.f), 1.f);
	//meshList[GEO_GUI_QUAD]->textureID = LoadTGA("Image//NYP.tga");

	////meshList[GEO_DOORMAN] = MeshBuilder::GenerateOBJ("Doorman", "Models//doorman.obj");
	////meshList[GEO_DOORMAN]->textureID = LoadTGA("Image//doorman.tga");

	//// Without texture
	//meshList[GEO_MODEL_MTL1] = MeshBuilder::GenerateOBJMTL("model2", "Models//house_type01.obj", "Models//house_type01.mtl");
	//// With texture
	//meshList[GEO_MODEL_MTL2] = MeshBuilder::GenerateOBJMTL("model3", "Models//cottage_obj.obj", "Models//cottage_obj.mtl");
	//meshList[GEO_MODEL_MTL2]->textureID = LoadTGA("Textures//cottage_diffuse.tga");
	//meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	//meshList[GEO_TEXT]->textureID = LoadTGA("Fonts//calibri.tga");

	meshList[GEO_SHOOT_BALL] = MeshBuilder::GenerateSphere("ball", glm::vec3(1.f, 1.f, 1.f), 1.f, 32, 32);
	meshList[GEO_WALL] = MeshBuilder::GenerateCube("wall", glm::vec3(1.f, 0.f, 0.f), 1.f);

	glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);


	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

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

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], cosf(glm::radians<float>(light[0].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], cosf(glm::radians<float>(light[0].cosInner)));
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	enableLight = true;
	bool miscSettings[2] = { false, false }; // for gravity and drag. override in case of specific objects

	miscSettings[0] = true; // enable gravity for ball
	miscSettings[1] = true; // enable drag for ball
	ball.InitPhysicsObject(glm::vec3(0, 5, 0), 0.75f, BoundingBox::Type::OBB, glm::vec3(0.5, 0.5, 0.5), miscSettings);

	miscSettings[0] = true; // disable gravity for wall
	miscSettings[1] = true; // disable drag for wall
	wall.InitPhysicsObject(glm::vec3(-5, 5, 0), 10.f, BoundingBox::Type::OBB, glm::vec3(5, 5, 1), 45, glm::vec3(1, 0, 0), miscSettings);

	floor.InitPhysicsObject(glm::vec3(0, 0, 0), 0.f, BoundingBox::Type::OBB, glm::vec3(100, 5, 100), 0, glm::vec3(1, 1, 1), miscSettings);

	player.InitPhysicsObject(camera.position, 0.f, BoundingBox::Type::OBB, glm::vec3(0.5f, 5.f, 0.5f), miscSettings);

	// init sound pop
	AudioManager::Instance().LoadSound("pop", "SFX/244657__dsg__pop-5.flac");


}

void SceneTester::Update(double dt)
{
	HandleKeyPress();

	// Move light (legacy from DXGL-FRAMEWORK)
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

	// Update spatial audio data
	//ma_engine_listener_set_position(Audio_GetEngine(), 0, camera.position.x, camera.position.y, camera.position.z);
	//ma_engine_listener_set_direction(Audio_GetEngine(), 0, camera.target.x, camera.target.y, camera.target.z);

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

	camera.Update(dt);

	float temp = 1.f / dt;
	fps = glm::round(temp * 100.f) / 100.f;

	player.position = camera.position;

	//debug
	{
		CollisionData cd;
		if (CheckCollision(ball, wall, cd))
		{
			meshList[GEO_SHOOT_BALL]->material.kAmbient = glm::vec3(0.f, 1.f, 0.f);
			ResolveCollision(cd);
		}
		else
		{
			meshList[GEO_SHOOT_BALL]->material.kAmbient = glm::vec3(1.f, 1.f, 1.f);
		}
	}
	{
		CollisionData cd;
		if (CheckCollision(ball, floor, cd))
		{
			meshList[GEO_SHOOT_BALL]->material.kAmbient = glm::vec3(0.f, 1.f, 0.f);
			ResolveCollision(cd);
		}
		else
		{
			meshList[GEO_SHOOT_BALL]->material.kAmbient = glm::vec3(1.f, 1.f, 1.f);
		}
	}
	{
		CollisionData cd;
		if (CheckCollision(wall, floor, cd))
		{
			meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.f, 1.f, 0.f);
			ResolveCollision(cd);
		}
		else
		{
			meshList[GEO_PLANE]->material.kAmbient = glm::vec3(1.f, 1.f, 1.f);
		}
	}


	ball.UpdatePhysics(dt);
}

void SceneTester::Render()
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

	{
		PushPop axesGuard(modelStack);
		// Render objects
		RenderMesh(meshList[GEO_AXES], false);
	}

	//{
	//	PushPop lightGuard(modelStack);
	//	// Render light
	//	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	//	modelStack.Scale(0.1f, 0.1f, 0.1f);
	//	RenderMesh(meshList[GEO_SPHERE], false);
	//}

	//{
	//	PushPop doormanGuard(modelStack);
	//	modelStack.Translate(0.f, 0.f, -0.f);
	//	meshList[GEO_DOORMAN]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	//	meshList[GEO_DOORMAN]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	//	meshList[GEO_DOORMAN]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
	//	meshList[GEO_DOORMAN]->material.kShininess = 1.0f;
	//	RenderMesh(meshList[GEO_DOORMAN], enableLight);
	//}
	//// Models 2 and 3
	//{
	//	PushPop idkGuard(modelStack);
	//	modelStack.Translate(5, 10, 0);
	//	RenderMesh(meshList[GEO_MODEL_MTL1], enableLight);
	//}
	//{
	//	PushPop idkGuard(modelStack);
	//	modelStack.Translate(5, -10, 0);
	//	modelStack.Scale(0.5, 0.5, 0.5);
	//	RenderMesh(meshList[GEO_MODEL_MTL2], enableLight);
	//}
	// Render GUI
	//RenderMeshOnScreen(meshList[GEO_GUI_QUAD], 50, 50, 50, 50);
	//{
	//	PushPop texturedPlaneGuard(modelStack);
	//	modelStack.Translate(0.f, 0.f, 0.f);
	//	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	//	modelStack.Scale(10.f, 10.f, 10.f);
	//	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	//	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	//	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
	//	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	//	RenderMesh(meshList[GEO_PLANE], enableLight);
	//}

	{
		PushPop wallGuard(modelStack);
		modelStack.Translate(wall.position.x, wall.position.y, wall.position.z);
		glm::mat4 rotation = glm::mat4_cast(wall.orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(wall.boundingBox.getWidth(), wall.boundingBox.getHeight(), wall.boundingBox.getDepth());
		//modelStack.Scale(5.f, 0.1f, 5.f);
		RenderMesh(meshList[GEO_WALL], true);
	}

	//shoot ball
	{
		PushPop shootBall(modelStack);
		modelStack.Translate(ball.position.x, ball.position.y, ball.position.z);
		glm::mat4 rotation = glm::mat4_cast(ball.orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(ball.boundingBox.getWidth(), ball.boundingBox.getHeight(), ball.boundingBox.getDepth());
		RenderMesh(meshList[GEO_WALL], true);
	}
	{
		PushPop floorGuard(modelStack);
		modelStack.Translate(floor.position.x, floor.position.y, floor.position.z);
		glm::mat4 rotation = glm::mat4_cast(floor.orientation);
		modelStack.MultMatrix(rotation);
		modelStack.Scale(floor.boundingBox.getWidth(), floor.boundingBox.getHeight(), floor.boundingBox.getDepth());
		meshList[GEO_WALL]->material.kAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
		meshList[GEO_WALL]->material.kDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		meshList[GEO_WALL]->material.kSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
		meshList[GEO_WALL]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_WALL], true);
	}

	// Render text
	{
		PushPop textGuard(modelStack);
		RenderText(meshList[GEO_TEXT], "Hello World", glm::vec3(0.f, 1.f, 0.f));
	}
	RenderTextOnScreen(meshList[GEO_TEXT], "Hello Screen", glm::vec3(0, 1, 0), 40, 0, 0, 'C', 1.f);
	std::string temp("FPS:" + std::to_string(fps));
	RenderTextOnScreen(meshList[GEO_TEXT], temp.substr(0, 9), glm::vec3(0, 1, 0), 40, 0, 550, 'C', 1.f);


	//{
	//	PushPop cameraTarget(modelStack);

	//	modelStack.Translate(0.f, 5.f, 5.f);
	//	modelStack.Scale(0.1f, 0.1f, 0.1f);
	//	RenderMesh(meshList[GEO_SPHERE], true);
	//}
}

void SceneTester::RenderMesh(Mesh* mesh, bool enableLight)
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


void SceneTester::Exit()
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

void SceneTester::HandleKeyPress()
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

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		// Change to black background
		// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

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

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_TAB))
	{
		if (light[0].type == Light::POINT) {
			light[0].type = Light::DIRECTIONAL;
		}
		else if (light[0].type == Light::DIRECTIONAL) {
			light[0].type = Light::SPOT;
		}
		else {
			light[0].type = Light::POINT;
		}

		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE)) {
		glm::vec3 shootDirection = glm::normalize(camera.target - camera.position);
		ball.velocity = glm::vec3(0,0,0);
		ball.AddImpulse(shootDirection * 25.f);
		ball.position = camera.position;
		ball.orientation = glm::quat(1, 0, 0, 0);
	}


	if (KeyboardController::GetInstance()->IsKeyPressed('Y')) { // test key to play pop sound
		AudioManager::Instance().SoundPlay("pop");

	}

}

void SceneTester::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizeX, float sizeY)
{
	glDisable(GL_DEPTH_TEST);
	//Change to orthographic mode
	{
		projectionStack.PushMatrix();
		glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -1000.f, 1000.f);
		projectionStack.LoadMatrix(ortho);

		// Set view and model matrix to identity
		{
			viewStack.PushMatrix();
			viewStack.LoadIdentity();
			{
				modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(400 + x, 300 + y, 0);
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

void SceneTester::HandleMouseInput() {
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

	if (!isLeftUp && MouseController::GetInstance() -> IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		// Step 3
		// transform into UI space
		double x = MouseController::GetInstance() -> GetMousePositionX();
		double y = 600 - MouseController::GetInstance() -> GetMousePositionY();
		// Check if mouse click position is within the GUI box
		// Change the boundaries as necessary
		if (x > 0 && x < 100 && y > 0 && y < 100) {
			std::cout << "GUI IS CLICKED" << std::endl;
		}
	}
}

void SceneTester::RenderText(Mesh* mesh, std::string text, glm::vec3
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

void SceneTester::RenderTextOnScreen(Mesh* mesh, std::string
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
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -
		100.f, 100.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(400 + x + alignmentOffset, 300 + y, 0);
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

void SceneTester::setCameraOrigin(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	cameraOriginPosition = position;
	cameraOriginTarget = target;
	cameraOriginUp = up;

	camera.Init(position, target, up);
}
