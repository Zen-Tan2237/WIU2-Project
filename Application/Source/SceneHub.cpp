#include "SceneHub.h"
#include "MeshBuilder.h"

SceneHub::SceneHub()
{
}

SceneHub::~SceneHub()
{
}

void SceneHub::Init()
{
	meshList.resize(HUB_NUM_GEOMETRY);
    BaseScene::Init();

	//models
	//meshList[GEO_STALL] = MeshBuilder::LoadOBJMTL("Stall", "OBJ//stall.obj", "OBJ//stall.mtl");
}

void SceneHub::Update(double dt)
{
	BaseScene::Update(dt);
}

void SceneHub::Render()
{
	BaseScene::Render();
}

void SceneHub::Exit()
{
	BaseScene::Exit();
}
