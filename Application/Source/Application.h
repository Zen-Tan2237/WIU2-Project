
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "..\include\miniaudio.h"

class Application
{
public:
	enum SCENE_NUM
	{
		SCENE1 = 0,
		SCENE2,
		SCENE3,
		SCENE4,
		SCENE5,
		TOTAL_SCENE
	};

	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

private:

	//Declare a window object
	StopWatch m_timer;

	bool enablePointer = false;
	bool showPointer = true;

	SCENE_NUM sceneNum;
	bool isEnterUp = false;

	ma_engine audioEngine;
};

#endif