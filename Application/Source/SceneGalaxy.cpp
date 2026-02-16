#include "SceneGalaxy.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

SceneGalaxy::SceneGalaxy()
{
}

SceneGalaxy::~SceneGalaxy()
{
}

void SceneGalaxy::Init()
{
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader",
								"Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	camera.Init(45.f, 45.f, 10.f);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	//meshList[GEO_SPHERE_RED] = MeshBuilder::GenerateSphere("Sun", glm::vec3(0.9f, 0.3f, 0.f), 1.f, 12, 12);
	meshList[GEO_SPHERE_RED] = MeshBuilder::GenerateSphere("Sun", glm::vec3(0.9f, 0.3f, 0.f), 1.f, 12, 12);
	meshList[GEO_SPHERE_BLUE] = MeshBuilder::GenerateSphere("Planet", glm::vec3(0.2f, 0.3f, 0.9f), 0.5f, 12, 12);
	meshList[GEO_SPHERE_GREY] = MeshBuilder::GenerateSphere("Moon", glm::vec3(0.5f, 0.5f, 0.5f), 0.2f, 12, 12);


	// Init projection
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f,
		0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	//Init variables
}

void SceneGalaxy::Update(double dt)
{
	HandleKeyPress();
	camera.Update(dt);
	moonRotation += static_cast<float>(dt) * 20.f;
}

void SceneGalaxy::Render()
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

	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate the Model-View-Projection matrix
	glm::mat4 MVP = projectionStack.Top() * viewStack.Top() *
		modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		glm::value_ptr(MVP));

	// Render objects
	meshList[GEO_AXES]->Render();

	////Render Sun
	//modelStack.PushMatrix(); {
	//	modelStack.Rotate(moonRotation * 0.1f, 0.f, 1.f, 0.f);
	//	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	//	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
	//		glm::value_ptr(MVP));
	//	meshList[GEO_SPHERE_RED]->Render();
	//	// Render Earth
	//	modelStack.PushMatrix(); {
	//		modelStack.Translate(2.f, 0.f, 0.f);
	//		modelStack.Rotate(moonRotation * 0.3f, 0.f, 1.f, 0.f);
	//		modelStack.Scale(0.5f, 0.5f, 0.5f);

	//		// Render the moon
	//		modelStack.PushMatrix(); {
	//			modelStack.Translate(0.7f, 0.f, 0.f);
	//			modelStack.Rotate(moonRotation, 0.f, 1.f, 0.f);
	//			modelStack.Scale(0.6f, 0.6f, 0.6f);
	//			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	//			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
	//			meshList[GEO_SPHERE_GREY]->Render();
	//		}

	//		modelStack.PopMatrix();
	//		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	//		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
	//		meshList[GEO_SPHERE_BLUE]->Render();
	//	}
	//	modelStack.PopMatrix();
	//}
	//modelStack.PopMatrix();

	//Me when I discover RAII
	//So smart. I dont need to manually push and pop anymore
	struct PushPop {
		// Constructor pushes matrix, destructor pops matrix
		// Explicit keyword to avoid implicit conversions
		explicit PushPop(MatrixStack& ms) : stack(&ms) {
			stack->PushMatrix();
		}
		~PushPop() {
			stack->PopMatrix();
		}
		MatrixStack* stack;
	};

	// Render Sun
	// Maybe use lambda for the MVP calc??
	// nah
	{
		PushPop sunGuard(modelStack);
		modelStack.Rotate(moonRotation * 0.1f, 0.f, 1.f, 0.f);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
		meshList[GEO_SPHERE_RED]->Render();
		{
			PushPop earthGuard(modelStack);
			modelStack.Translate(2.f, 0.f, 0.f);
			modelStack.Rotate(moonRotation * 0.3f, 0.f, 1.f, 0.f);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			{
				PushPop moonGuard(modelStack);
				modelStack.Translate(0.7f, 0.f, 0.f);
				modelStack.Rotate(moonRotation, 0.f, 1.f, 0.f);
				modelStack.Scale(0.6f, 0.6f, 0.6f);
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
				meshList[GEO_SPHERE_GREY]->Render();
			}
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
			meshList[GEO_SPHERE_BLUE]->Render();
		}
	}
}

void SceneGalaxy::Exit()
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

void SceneGalaxy::HandleKeyPress() 
{
	if (Application::IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (Application::IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	if (Application::IsKeyPressed('P')) {
		if (projectionMode == 0) {
			projectionMode = 1;
		}
		else {
			projectionMode = 0;
		}
	}
}
