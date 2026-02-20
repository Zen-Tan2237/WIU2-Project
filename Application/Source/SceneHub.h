#ifndef SCENE_HUB_H
#define SCENE_HUB_H

#include "BaseScene.h"
class SceneHub : public BaseScene
{
public:
    enum GEOMETRY_TYPE_HUB
    {
        GEO_STALL,
        NUM_GEOMETRY,
	};

    SceneHub();
    ~SceneHub();
    void Init() override;
    void Update(double dt) override;
    void Render() override;
    void Exit() override;

	Mesh* meshList_hub[NUM_GEOMETRY];

};
#endif

