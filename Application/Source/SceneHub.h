
#ifndef SCENE_HUB_H
#define SCENE_HUB_H

#include "BaseScene.h"
class SceneHub : public BaseScene
{
public:
    static const int NUM_GRASSCLUMPS = 1;
	static const int NUM_PICKABLES = 5;

    enum GEOMETRY_TYPE_HUB
    {
        //debug
        GEO_WALL,
        GEO_SPHERE,

        NUM_GEOMETRY,
    };

    SceneHub();
    ~SceneHub();
    void Init() override;
    void Update(double dt) override;
    void Render() override;
    void RenderUI();
    void Exit() override;

    Mesh* meshList_hub[NUM_GEOMETRY];

    //UI
    AnimatedUIElement ShopGUI;
    InteractPromptUI TextUI;
	InteractPromptUI SelectionUI[NUM_PICKABLES];

	bool inShop;
    int shopSelection;

    // Grass density management
    int activeGrassCount;
    float grassDensityMultiplier;
    float fpsSmoothed;
    float targetFPS; // 60 rn

    void UpdateGrassDensity(double dt);
    void RegenerateGrassPositions();
    glm::vec3 grassClumps[NUM_GRASSCLUMPS];

    //debug
    glm::vec3 debugPos;
    float debugScale;

    bool temp = false;

};
#endif

//instruction 
// this is a inherited class from baseScene.
// base scene will handle ui rendering (interactives and properties will be init and edited here as well) and the hub scene will handle the 3D rendering of the hub and its unique interactives.
// base scene also handles skybox rendering 
// base scene contains all 8 lights. you must edit the properties here
// base scene has its own meshList containing the ui and skybox. the inherited scene (this scene) will have a seperate meshList, where meshList_<scene name>. make sure to use the correct one when rendering here.

