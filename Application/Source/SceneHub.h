
#ifndef SCENE_HUB_H
#define SCENE_HUB_H

#include "BaseScene.h"
class SceneHub : public BaseScene
{
public:
    enum GEOMETRY_TYPE_HUB
    {
        GEO_WALL,
        GEO_FOUNTAIN,
        GEO_STALL,
        GEO_TABLE,
        GEO_FOODSTAND,
        GEO_FERRISWHEEL,
        GEO_MONKEY,
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

};
#endif

//instruction 
// this is a inherited class from baseScene.
// base scene will handle ui rendering (interactives and properties will be init and edited here as well) and the hub scene will handle the 3D rendering of the hub and its unique interactives.
// base scene also handles skybox rendering 
// base scene contains all 8 lights. you must edit the properties here
// base scene has its own meshList containing the ui and skybox. the inherited scene (this scene) will have a seperate meshList, where meshList_<scene name>. make sure to use the correct one when rendering here.

