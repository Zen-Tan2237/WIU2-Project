#ifndef SCENE_HUB_H
#define SCENE_HUB_H

#include "BaseScene.h"
class SceneHub : public BaseScene
{
public:
    enum HUB_GEOMETRY_TYPE
    {
        BOX = BaseScene::NUM_GEOMETRY,
        GEO_STALL,
        HUB_NUM_GEOMETRY // sentinel: equals BaseScene::NUM_GEOMETRY + number_of_hub_items
    };

    SceneHub();
    ~SceneHub();
    void Init() override;
    void Update(double dt) override;
    void Render() override;
    void Exit() override;
};
#endif

