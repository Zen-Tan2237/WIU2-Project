
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneHub.h"
#include "SceneRiseTop.h"
#include "SceneFPCamera.h"
#include "SceneKnockdown.h"
#include "SceneBB.h"

#include "KeyboardController.h"
#include "MouseController.h"
#include "audio.h"

#include <cstdlib>
#include <ctime>
//#include <iostream>

//debug
#include "SceneTester.h"
#include "BaseScene.h"

GLFWwindow* m_window;
const unsigned char FPS = 120; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define the mouse button callback
static void mousebtn_callback(GLFWwindow* window, int button, int action,
	int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}
//Define the mouse scroll callback
static void mousescroll_callback(GLFWwindow* window, double xoffset,
	double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}


//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	 _fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	KeyboardController::GetInstance()->Update(key, action);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h); //update opengl the new window size
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	sceneNum = SCENE1;

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(1600, 900, "OPENGL FRAMEWORK", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	glfwSetKeyCallback(m_window, key_callback);

	//Sets the mouse button callback
	glfwSetMouseButtonCallback(m_window, mousebtn_callback);
	//Sets the mouse scroll callback
	glfwSetScrollCallback(m_window, mousescroll_callback);
	if (enablePointer == false)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else if (showPointer == false)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//Sets the resize callback to handle window resizing
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// init AudioManager from audio
	if (!AudioManager::Instance().Init()) {
		fprintf(stderr, "Failed to initialize AudioManager.\n");
	}

	srand(time(0));
}

void Application::Run()
{
	//Main Loop
	Scene* scene1 = new SceneHub(); 
	Scene* scene2 = new SceneRiseTop();
	Scene* scene3 = new SceneBB();
	Scene* scene4 = new SceneKnockdown();
	Scene* scene5 = new SceneTester();
	
	Scene* scene = scene1;
	scene->Init();


	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		scene->Update(m_timer.getElapsedTime());

		// Scene switching system
		if (scene->nextScene != 0 && scene->nextSceneDelay == 0.f)
		{
			Scene* newScene = nullptr;

			switch (scene->nextScene)
			{
			case 1:
				newScene = scene1;
				break;
			case 2:
				newScene = scene2;
				break;
			case 3:
				newScene = scene3;
				break;
			case 4:
				newScene = scene4;
			default:
				break;
			}

			if (newScene)
			{
				int accumulatedCash = scene->accumulatedCash; //all scenes share cash $$$monners
				scene->Exit();
				newScene->Init();
				scene = newScene;
				scene->accumulatedCash = accumulatedCash;
			}

			scene->nextScene = 0; // reset trigger
		}

		scene->Render();
		glfwSwapBuffers(m_window);

		KeyboardController::GetInstance()->PostUpdate();
		MouseController::GetInstance()->PostUpdate();

		double mouse_x, mouse_y;
		glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
		MouseController::GetInstance()->UpdateMousePosition(mouse_x, mouse_y);

		glfwPollEvents();
		m_timer.waitUntil(frameTime);
	}
	scene->Exit();
	if (scene1 != nullptr) {
		delete scene1;
	}
	if (scene2 != nullptr) {
		delete scene2;
	}
	if (scene3 != nullptr) {
		delete scene3;
	}
	if (scene4 != nullptr) {
		delete scene4;
	}
}

void Application::Exit()
{
	KeyboardController::DestroyInstance();

	AudioManager::Instance().Shutdown();

	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
